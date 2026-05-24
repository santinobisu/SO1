#include "barrier.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void barrier_init(barrier *b, int n)
{
    b->procesos = n;
    b->contador = n;
    b->generacion = 0;
    pthread_mutex_init(&(b->lock), NULL);
    pthread_cond_init(&(b->puede_seguir), NULL);

}

void barrier_wait(barrier *b)
{
    pthread_mutex_lock(&(b->lock));

    int gen = b->generacion;

    b->contador--;

    if (b->contador == 0)
    {
        b->generacion++;
        b->contador = b->procesos;

        pthread_cond_broadcast(&(b->puede_seguir));
    }
    else
    {
        while (gen == b->generacion)
            pthread_cond_wait(&(b->puede_seguir), &(b->lock));
    }

    pthread_mutex_unlock(&(b->lock));
}