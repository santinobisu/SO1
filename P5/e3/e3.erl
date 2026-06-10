% Ej. 3 (Balanceo de Carga). La siguiente funci´on implementa un balanceo de carga sobre una lista de
% servidores. Al recibir un pedido de la forma {req, Arg, Pid}, lo reenv´ıa aleatoriamente a uno de los
% servidores y espera su respuesta. Luego, reenv´ıa la respuesta al cliente que origin´o el pedido.
% Explique el problema con esta implementaci´on. Proponga una soluci´on e implem´entela.
% El problema con esta implementacion es que puede tocar muchas veces el mismo servidor debido a que 
% la eleccion es completamente aleatoria. Una solucion seria balancearla linealmente en orden, de 
% forma que los servidores van a estar proporcionalmente balanceados entre sí.

bal(Servs, Num) ->
    if
        Num > length(Servs) ->
            Num = 1
    end.

    receive
        {req, Arg, Pid} ->
            Proc = lists:nth(Num, Servs),
            Proc ! { req, Arg, self() },
            receive
                Reply -> Pid ! Reply
            end
    end,
    bal(Servs, Num + 1).