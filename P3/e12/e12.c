/*
Ej. 12. Implemente una librer´ıa de sem´aforos usando variables de condici´on.+
*/

#include "e12.h"
#include <stdio.h>
#include <stdlib.h>


int semaforo_init(Semaforo *sem, unsigned int v)
{
    sem->contador = v;
    int c1 = pthread_cond_init(&(sem->cond),NULL); // si es 0, esta bien
    int c2 = pthread_mutex_init(&(sem->lock),NULL); // si es 0 esta bien
    if(c1 != 0 || c2 != 0)
        return -1;
    return 0; 
}

int semaforo_wait(Semaforo *sem)
{
    pthread_mutex_lock(&(sem->lock));
    while(!(sem->contador))
        pthread_cond_wait(&(sem->cond), &(sem->lock));
    int c = --sem->contador;
    pthread_mutex_unlock(&(sem->lock));
    if(c < 0) return -1;
    return 0; 
}

int semaforo_post(Semaforo *sem)
{
    pthread_mutex_lock(&(sem->lock));
    int c = ++sem->contador;
    pthread_mutex_unlock(&(sem->lock));
    pthread_cond_broadcast(&(sem->cond));
    if(c == 0) return -1;
    return 0; 
}