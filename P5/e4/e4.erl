% Ej. 4 (Servidor de Turnos). Reimplemente el servidor de tickets de la Pr´actica 1 en Erlang. Puede
% usar el siguiente esqueleto para manejar conexiones TCP en Erlang. El mismo acepta conexiones TCP
% en modo activo, haciendo que el proceso que realizar el accept de una conexi´on reciba mensajes con
% los datos recibidos por la misma. Tambi´en puede usar el modo pasivo si as´ı lo desea, cambiando las
% opciones pasadas a listen. Aseg´urese tambi´en de que el servidor es robusto: debe manejar correctamente
% conexiones cerradas por el cliente y tambi´en tener en cuenta que los pedidos pueden llegar fragmentados o
% “pegados” (TCP no tiene concepto de mensaje ni de “borde”), entre otras cosas.
% a) Compare el servidor en PThreads y el actual con el cliente dado anteriormente, para 200, 2000 y
% 20000 conexiones simult´aneas. Puede usar el cliente turno cliente.c.
% b) ¿Ve una diferencia importante en el consumo de memoria de los dos servidores? ¿A qu´e cree que se
% puede deber?
% c) ¿Puede cada servidor aceptar 50000 conexiones simult´aneas?
% Nota: para conseguir aceptar tantas conexiones, seguramente tenga que aumentar el ulimit de FDs
% abiertos que impone el sistema operativo. Correr ulimit -n 1000000 deber´ıa bastar. Ver tambi´en
% help ulimit.

-module(e4).
-export([server/0]).

server() ->
    {ok, ListenSocket} = gen_tcp:listen(8000, [{reuseaddr, true}]),
    wait_connect(ListenSocket, 0).

wait_connect(ListenSocket, N) ->
    {ok, Socket} = gen_tcp:accept(ListenSocket),
    spawn (fun () -> wait_connect (ListenSocket, N+1) end),
    get_request(Socket).

get_request(Socket) ->
    io:fwrite("Esperando mensajes de ~p~n", [Socket]),
    receive
        _X -> ok,
            get_request(Socket)
    end.