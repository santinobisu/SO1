#ifndef __VCONDICION_H__
#define __VCONDICION_H__
#include <pthread.h>
#include <semaphore.h>

typedef struct _vcondicion
{
    sem_t semaforo;
    pthread_mutex_t lock;
    int contador;
    
} Vcondicion;


int vcondicion_init(Vcondicion *vcond);

void vcondicion_wait(Vcondicion *vcond, pthread_mutex_t *lock);

int vcondicion_signal(Vcondicion *vcond);

void vcondicion_broadcast(Vcondicion *vcond);

#endif