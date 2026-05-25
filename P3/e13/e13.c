/*
Ej. 13. Implemente una librer´ıa de variables de condici´on usando sem´aforos.
*/

#include "e13.h"

int vcondicion_init(Vcondicion *vcond)
{
    vcond->contador = 0;
    int c1 = pthread_mutex_init(&(vcond->lock), NULL);
    int c2 = sem_init(&(vcond->semaforo), 0 , 0);
    if (c1 != 0 || c2 == -1)
        return -1;
    return 0;
}

void vcondicion_wait(Vcondicion *vcond, pthread_mutex_t *lock)
{
    pthread_mutex_unlock(lock);
    pthread_mutex_lock(&(vcond->lock));
    vcond->contador++;
    pthread_mutex_unlock(&(vcond->lock));
    sem_wait(&(vcond->semaforo));
    pthread_mutex_lock(lock);
}

int vcondicion_signal(Vcondicion *vcond)
{
    pthread_mutex_lock(&(vcond->lock));
    if(vcond->contador > 0)
    {
        vcond->contador--;
        pthread_mutex_unlock(&(vcond->lock));
        sem_post(&(vcond->semaforo));
        return 0;
    }
    pthread_mutex_unlock(&(vcond->lock));
    return -1;
}

void vcondicion_broadcast(Vcondicion *vcond)
{
    pthread_mutex_lock(&(vcond->lock));
    for(int i = 0; i < vcond->contador; i++)
    {
        vcond->contador--;
        sem_post(&(vcond->semaforo));
    }
    pthread_mutex_unlock(&(vcond->lock));
}

