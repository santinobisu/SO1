% Ej. 6 (Servidor de Difusi´on). Implemente un proceso servidor que distribuya los mensajes que recibe
% entre todos sus suscriptores. El servidor tiene las siguientes operaciones:
% Suscribirse: El proceso llamado es incluido en el conjunto de suscriptores.
% Enviar mensaje: El mensaje recibido debe ser reenviado a todos los suscriptores.
% Desuscribirse: El proceso llamado es eliminado del conjunto de suscriptores.
% Cada operaci´on debe tener una funci´on que la implemente, por ejemplo suscribir/1. El servidor puede
% iniciarse con una funci´on que retorne un descriptor del mismo, o puede registrarse globalmente (en cuyo
% caso, suscribir tiene aridad cero). Si hay paso de mensajes, el mismo debe estar abstra´ıdo detr´as de
% esa interfaz. En cada difusi´on, los suscriptores deben recibir el mensaje una ´unica vez. Una vez suscrito,
% no deber´ıa tener efecto suscribirse nuevamente, y desuscribirse siempre tiene efecto inmediato (i.e. las
% suscripciones no son recursivas).

-module(e6).

%% Librería
% Funciones control
-export([start/0,stop/1]).
% Funciones interacción
-export([broadcast/2,subscribe/1,unsubscribe/2]).
%%%
-export([loopBroadcast/1]).

-export([test/0,client/1]).

start() ->
  spawn(?MODULE, loopBroadcast, [[]]).

stop(Broadcast) ->
  Broadcast ! stop.

broadcast(Broadcast, Msg) ->
  Broadcast ! {brod, Msg}.

subscribe(Broadcast) ->
  Broadcast ! {sub, self()}.

unsubscribe(Broadcast, Pid) ->
    Broadcast ! {unsub, Pid}.

loopBroadcast(RegisteredPids) ->
    receive 
        {sub, Pid}  -> io:fwrite("Se subscribio el ~p al servicio de broadcast ~n", [Pid]),
                      loopBroadcast([Pid | RegisteredPids]);
        {brod, Msg} -> lists:foreach(fun (Pid) -> Pid ! Msg end , RegisteredPids),
                       loopBroadcast(RegisteredPids);
        {unsub, Pid} -> io:fwrite("Se desuscribio el ~p al servicio de broadcast ~n", [Pid]),
                        Nueva = lists:delete(Pid, RegisteredPids),
                        loopBroadcast(Nueva);
        stop         -> ok
    end.

client(Broadcast) ->
  subscribe(Broadcast),
  client_loop().

client_loop() -> 
                receive
                  unsub -> chau;
                  stop -> ok;
                  Msg -> io:fwrite("El cliente ~p recibio el mensaje ~p ~n", [self(), Msg]),
                         client_loop()
                end.


test() ->
  BroadcastPid = start(),
  C1 = spawn(?MODULE, client, [BroadcastPid]),
  C2 = spawn(?MODULE, client, [BroadcastPid]),
  C3 = spawn(?MODULE, client, [BroadcastPid]),
  timer:sleep(1000),
  broadcast(BroadcastPid, "Mensaje para cliente"),
  timer:sleep(1000),
  unsubscribe(BroadcastPid, C1),
  unsubscribe(BroadcastPid, C2),
  timer:sleep(1000),
  broadcast(BroadcastPid, "Mensaje para cliente 2"),
  timer:sleep(1000),
  stop(BroadcastPid),
  C1 ! stop,
  C2 ! stop,
  C3 ! stop,
  ok.







