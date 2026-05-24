/* 
Ej. 8. El siguiente programa intenta corregir con gracia una divisi´on por cero, atrapando la se˜nal
correspondiente (SIGFPE) y modificando el denominador de la divisi´on en ese caso.
¿Qu´e pasa al correr el programa? ¿Por qu´e?
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

int denom = 0;
void handler(int s) { printf("ouch!\n"); denom = 1; }
int main()
{
    int r;
    signal(SIGFPE, handler);
    r = 1 / denom;
    printf("r = %d\n", r);
    return 0;
}

// Lo que pasa al correr el programa es que por mas que tengamos un handler que cambia la variable en el .data, 
// no cambia la variable metida en un registro la cual usa el main, por lo que el programa se queda en un bucle infinito.