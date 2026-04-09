#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <stdatomic.h>

/* Ej. 3. ¿Puede fallar la siguiente aserci´on? ¿Bajo qu´e condiciones? Explique. Si puede fallar, arregle el
programa.

} */

atomic_int x = 0, y = 0, a = 0, b = 0;
void * foo(void *arg) { x = 1; a = y; return NULL; }
void * bar(void *arg) { y = 1; b = x; return NULL; }
int main() {

    pthread_t t0, t1;
    pthread_create(&t0, NULL, foo, NULL);
    pthread_create(&t1, NULL, bar, NULL);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    assert (a || b);

return 0;
}

// El programa puede fallar, si a la hora de ejecutar el scheduler decide reorganizar las asignaciones, ejecutando primero foo en a = y, cambi de
// contexto a bar, b = x, y luego x = 1, y = 1.
