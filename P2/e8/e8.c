/* 
Ej. 8. En la siguiente implementaci´on del jard´ın ornamental (asumiendo dos molinetes), agregue
estrat´egicamente algunos sleep() para obtener el m´ınimo valor posible de visitantes. Puede usar
condicionales.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#define VISITANTE 100000

int visitantes = 0;

void sleep_ns(long nanosegundos) {
    struct timespec ts;

    ts.tv_sec = nanosegundos / 1000000000L;      // segundos
    ts.tv_nsec = nanosegundos % 1000000000L;     // resto en ns

    nanosleep(&ts, NULL);
}

void *proc(void *arg)
{
    int i;
    int id = arg - (void *)0;
    for (i = 0; i < VISITANTE; i++)
    {
        int c;
        /* sleep? */
        c = visitantes;
        sleep_ns(1);
        visitantes = c + 1;
        /* sleep? */
    }
    return NULL;
}

int main(){

pthread_t h1,h2;
pthread_create(&h1,NULL,proc,NULL);
pthread_create(&h2,NULL,proc,NULL);
pthread_join(h1,NULL);
pthread_join(h2,NULL);

printf("%d\n", visitantes);
return 0;

}
