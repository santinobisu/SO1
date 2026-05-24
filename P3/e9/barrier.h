#ifndef __BARRRIER_H__
#define __BARRIER_H__
#include <pthread.h>

typedef struct _barrier
{
    pthread_cond_t puede_seguir;
    pthread_mutex_t lock;
    int contador;
    int procesos;
    int generacion;
} barrier;

void barrier_init(barrier *b, int n);
void barrier_wait(barrier *b);

#endif