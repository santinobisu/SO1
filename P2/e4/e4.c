#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

/* Ej. 4. ¿Puede fallar la siguiente aserci´on (wr y rd corren en un thread cada uno)? Explique. Si puede
fallar, arregle el programa.
 */

volatile int x = 0;
volatile int y = 0;
void * wr(void *arg) { x = 123; y = 1; }
void * rd(void *arg) {
    while (!y);
    assert(x == 123);
}

// Si, puede fallar si se ejecuta primero wr y la cpu reorganiza el codigo ejecutando y = 1, luego cambiando de contexto a rd y
// ejecutando el codigo de rd antes de que x = 123.