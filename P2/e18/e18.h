// recursive_mutex.h
#ifndef RECURSIVE_MUTEX_H
#define RECURSIVE_MUTEX_H

#include <pthread.h>
#include <sys/types.h>

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t  cond;
    pid_t           owner;   // gettid() del dueño actual, o -1 si libre
    int             count;   // cantidad de veces que el dueño lo adquirió
} rmutex_t;

void rmutex_init(rmutex_t *m);
void rmutex_lock(rmutex_t *m);
void rmutex_unlock(rmutex_t *m);
void rmutex_destroy(rmutex_t *m);

#endif