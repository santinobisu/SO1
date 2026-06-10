% ej. 2 (Temporizaci´on).
% a) Implemente una funci´on wait/1 que tome como argumento una cantidad de milisegundos y espere
% ese tiempo.
% b) Implemente un cron´ometro que reciba tres argumentos, Fun, Hasta y Periodo y ejecute Fun/0 cada
% Periodo milisegundos hasta que hayan pasado Hasta milisegundos sin bloquear el int´erprete.
% Un caso de prueba ser´ıa:
% cronometro(fun () -> io:fwrite("Tick~n") end, 60000, 5000).
% que imprimir´ıa Tick cada 5 segundos durante un minuto.

-module(e2).
-export([wait/1, cronometrar/3, cronometro/3]).

wait(S) -> timer:sleep(S).

cronometrar(Fun, Hasta, Periodo) ->
    if
        Hasta > 0 ->
            Fun(),
            wait(Periodo),
            cronometrar(Fun, Hasta - Periodo, Periodo);
        true ->
            fin
    end.

cronometro(Fun, Hasta, Periodo) -> spawn(e2, cronometrar, [Fun, Hasta, Periodo]).