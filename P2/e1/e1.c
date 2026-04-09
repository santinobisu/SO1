#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
/*
Ej. 1. Suponga que queremos detectar si un array A contiene el entero 42 iterando por el mismo, y si es
as´ı, prender la bandera encontrado. ¿Hay diferencia entre los dos fragmentos siguientes? ¿En qu´e casos?

if (A[i] == 42)
    encontrado = true;

encontrado = encontrado || (A[i] == 42);

La diferencia entre estos dos fragmentos de código, es que en el primero solo se toca la variable "encontrado" cuando la condición evaluada ´
es verdadera, mientras que en el otro a la variable "encontrado" siempre se le va a "acumulando" un valor con cada iteración.
En el caso de que "encontrado" sea erroneamente inicializado (con un valor distinto de 0), el segundo fragmento fallará.

*/
