% Ej. 5 (Lanzar Procesos en Anillos). Escriba un programa que lance N procesos en anillos. Cada
% proceso recibir´a dos clases de mensajes:
% • {msg, N} donde N es un entero. Deber´a decrementarlo y enviarlo al siguiente proceso en el anillo si
% N es mayor que cero. En caso contrario deber´a enviar un mensaje exit y terminar cuando todos los
% dem´as lo hayan hecho.
% • exit cuando el proceso debe terminar.
% Modifique el programa para que el mensaje enviado gire una vez alrededor del anillo y sea descartado por
% el que inici´o el env´ıo.

-module(e5).
-export([server/1]).

% inicio(N) ->


% crear_proceso(N, PidPrimero) ->
%     if N > 1 ->
%         PID = spawn(proceso(), [crear_proceso(N - 1, PidPrimero)]);
%     true -> 
%         spawn(proceso(), [PidPrimero])
%     end

    

% primero() ->
%     receive
%         {mensaje, N} -> ok,


% proceso(SigPID) ->
%     receive
%         {mensaje, N} -> ok,
%         if N > 0 ->
%             % PID del siguiente ! {mensaje, N - 1}
%         true ->
%             % PID del siguiente ! {"exit", 0}
%             % terminar
%     end.


% --- COMIENZO DEL SERVIDOR ---
server(N) ->
    if
        N < 2 ->
            io:format("El anillo debe tener al menos 2 procesos.~n");
        true ->
            % 1. Creamos el primer proceso en espera de conocer a su vecino
            PrimerPID = spawn(fun() -> esperar_siguiente() end),
            
            % 2. Creamos el resto de los procesos en cadena
            SegundoPID = crear_procesos(N - 1, PrimerPID),
            
            % 3. Cerramos el anillo conectando el primero con el segundo
            PrimerPID ! {iniciar, SegundoPID},
            
            % 4. Le ordenamos al primer proceso que INICIE el envío en el anillo
            PrimerPID ! {kickstart, 4}, 
            ok
    end.


% --- CREACIÓN DE PROCESOS EN CADENA ---
crear_procesos(0, ProcesoDestino) ->
    ProcesoDestino;
crear_procesos(Restantes, ProcesoDestino) ->
    NuevoPid = spawn(fun() -> proceso(ProcesoDestino) end),
    crear_procesos(Restantes - 1, NuevoPid).


% --- ESTADO INICIAL DEL PRIMER PROCESO ---
esperar_siguiente() ->
    receive
        {iniciar, SigPid} ->
            proceso(SigPid)
    end.


% --- COMPORTAMIENTO DE LOS PROCESOS DEL ANILLO ---
proceso(SigPid) ->
    receive
        % Mensaje especial para que ESTE proceso sea el que "inicie el envío"
        {kickstart, ValorInicial} ->
            io:format("Proceso ~p: Yo inicio el envío con N=~p~n", [self(), ValorInicial]),
            SigPid ! {msg, self(), ValorInicial},
            proceso(SigPid);

        % Recepción del mensaje circulante en el anillo
        {msg, PidIniciador, N} ->
            if
                % Si el iniciador soy yo mismo, el mensaje dio la vuelta completa. ¡Se descarta!
                PidIniciador == self() ->
                    io:format("Proceso ~p (Iniciador): El mensaje dio la vuelta completa. ¡DESCARTADO!~n", [self()]),
                    proceso(SigPid);

                % Si no soy el iniciador y N es mayor a 0, decrementamos y pasamos
                N > 0 ->
                    io:format("Proceso ~p: Pasando mensaje de ~p (N=~p) hacia ~p~n", [self(), PidIniciador, N, SigPid]),
                    SigPid ! {msg, PidIniciador, N - 1},
                    proceso(SigPid);

                % Si N llega a 0 antes de dar la vuelta, se detiene el anillo con un exit
                true ->
                    io:format("Proceso ~p: N llegó a 0. Enviando 'exit' a ~p~n", [self(), SigPid]),
                    SigPid ! exit,
                    proceso(SigPid)
            end;

        % Recepción del mensaje de finalización
        exit ->
            io:format("Proceso ~p: Recibió 'exit'. Pasándolo a ~p y terminando.~n", [self(), SigPid]),
            SigPid ! exit,
            ok % El proceso muere aquí
    end.

