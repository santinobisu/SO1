#ifndef __SEMAFORO_H__
#define __SEMAFORO_H__
#include <pthread.h>

typedef struct _semaforo
{
    unsigned contador;
    pthread_cond_t cond;
    pthread_mutex_t lock;
} Semaforo;


int semaforo_init(Semaforo *sem, unsigned int v);

int semaforo_wait(Semaforo *sem);

int semaforo_post(Semaforo *sem);


#endif