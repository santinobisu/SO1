% Ej. 1 (Intro). Dado el siguiente c´odigo
% a) Justifique cu´ales match de la funci´on match_test deber´ıan ser v´alidos y cu´ales no.
% b) Implemente las funciones nombre/1 y apellido/1 para que devuelvan esos campos de las tuplas
% que obtienen como argumento utilizando pattern matching.
% c) Explique el resultado de cada una de las comparaciones de la funci´on string_test/0 (es decir por
% qu´e dan true o false).
% d) Implemente la funci´on filtrar_por_apellido/2 para que devuelva los nombres (sin el apellido)
% de las personas de la lista Personas cuyo apellido coincide con Apellido utilizando comprensi´on de
% listas.

-module(e1).
-export([init/0,apellido/1,nombre/1,string_test/0]).

match_test () ->
    {A,B} = {5,4},
    % {C,C} = {5,4}, esta asignacion es incorrecta ya que le queremos dar 2 valores distintos a la variable C 
    {B,A} = {4,5},
    {D,D} = {5,5}.

string_test () -> [
    helloworld == 'helloworld', %true: las comillas simples tienen la misma funcion para declarar atomos
    "helloworld" < 'helloworld', %false: son tipos de datos distintos
    helloworld == "helloworld", %false: son tipos de datos distintos
    [$h,$e,$l,$l,$o,$w,$o,$r,$l,$d] == "helloworld", %true: es la misma palabra
    [104,101,108,108,111,119,111,114,108,100] < {104,101,108,108,111,119,111,114,108,100}, %false: es lo mismo
    [104,101,108,108,111,119,111,114,108,100] > 1, %true: compara caracter a caracter en ascii
    [104,101,108,108,111,119,111,114,108,100] == "helloworld"]. %true: es lo mismo, solo que en formati ascii

tuple_test (P1, P2) ->
    io:fwrite("El nombre de P1 es ~p y el apellido de P2 es ~p~n", [nombre(P1), apellido(P2)]).

apellido ({persona, {nombre, _}, {apellido, A}}) -> A. 

nombre ({persona, {nombre, N}, {apellido, _}}) -> N.

filtrar_por_apellido([], Apellido) -> [];
filtrar_por_apellido([P|R], Apellido) ->
    case apellido(P) == Apellido of 
        true -> [nombre(P)|filtrar_por_apellido(R,Apellido)];
        false -> filtrar_por_apellido(R,Apellido)
    end.

init () ->
    P1 = {persona, {nombre, "Juan"}, {apellido, "Gomez"}},
    P2 = {persona, {nombre, "Carlos"}, {apellido, "Garcia"}},
    P3 = {persona, {nombre, "Javier"}, {apellido, "Garcia"}},
    P4 = {persona, {nombre, "Rolando"}, {apellido, "Garcia"}},
    match_test(),
    tuple_test(P1, P2),
    string_test(),
    Garcias = filtrar_por_apellido([P4, P3, P2, P1], "Garcia").