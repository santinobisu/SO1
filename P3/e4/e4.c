/*
Ej. 4 (Productor-Consumidor, Dijkstra). En un sistema, hay M procesos llamados “productores”
que generan valores (digamos, de tipo int) y N procesos “consumidores” que los leen y realizan alguna
computaci´on con ellos. Se debe garantizar que:
cada valor producido sea consumido, es decir, que no se pierda
cada valor producido no sea tomado por m´as de un consumidor
Los procesos se comunican mediante un buffer compartido, insertando y tomando elementos del mismo.
En este ejemplo (ver prod-cons.c), los productores alocan un entero con malloc() y los consumidores
los liberan con free(). Perder elementos producidos implica una fuga de memoria, y consumirlos dos
veces implica un doble-free.

a) Implemente una soluci´on usando sem´aforos para llevar las cantidades de elementos en el buffer, y la
cantidad de elementos libres.

b) Implemente una soluci´on usando variables de condici´on.
*/

// a-
