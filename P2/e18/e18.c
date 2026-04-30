// recursive_mutex.c
#include "e18.h"
#include <assert.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>

// Obtiene el TID real del thread (único entre todos los threads del sistema)
static pid_t get_tid(void) {
    return (pid_t) syscall(SYS_gettid);
}

void rmutex_init(rmutex_t *m) {
    pthread_mutex_init(&m->lock, NULL);
    pthread_cond_init(&m->cond, NULL);
    m->owner = -1;
    m->count = 0;
}

void rmutex_lock(rmutex_t *m) {
    pid_t me = get_tid();

    pthread_mutex_lock(&m->lock);

    if (m->owner == me) {
        // Caso recursivo: ya soy el dueño, solo incremento
        m->count++;
    } else {
        // Espero hasta que el mutex esté libre
        while (m->owner != -1) {
            pthread_cond_wait(&m->cond, &m->lock);
        }
        // Lo tomo por primera vez
        m->owner = me;
        m->count = 1;
    }

    pthread_mutex_unlock(&m->lock);
}

void rmutex_unlock(rmutex_t *m) {
    pid_t me = get_tid();

    pthread_mutex_lock(&m->lock);

    assert(m->owner == me);  // solo el dueño puede hacer unlock

    m->count--;
    if (m->count == 0) {
        // Liberación real: otro thread puede entrar
        m->owner = -1;
        pthread_cond_signal(&m->cond);
    }
    // Si count > 0, el mutex sigue siendo mío

    pthread_mutex_unlock(&m->lock);
}

void rmutex_destroy(rmutex_t *m) {
    pthread_mutex_destroy(&m->lock);
    pthread_cond_destroy(&m->cond);
}

