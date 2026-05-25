/*
Ej. 11 (Canales S´ıncronos). Un canal es una primitiva que permite el env´ıo de un valor entre threads.
Que sea s´ıncrono implica que no s´olo el lector espera al escritor (obviamente) sino que el escritor no
avanza hasta que haya aparecido un lector. Implemente canales s´ıncronos con la siguiente interfaz:

struct channel { ... };
void channel_init(struct channel *c);
void chan_write(struct channel *c, int v);
int chan_read(struct channel *c);

Use su implementaci´on para implementar una soluci´on al problema de los (m´ultiples) productores y
consumidores. ¿Qu´e ventajas y desventajas tiene?
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "channels.h"

void channel_init(channel* canal)
{
    pthread_mutex_init(&(canal->lock), NULL);
    pthread_cond_init(&(canal->puede_seguir_escritor), NULL);
    pthread_cond_init(&(canal->puede_seguir_lector), NULL);
    canal->lector_intencion = 0;
    canal->escritor_termino = 0;
}

void channel_write(channel *canal, int v)
{
    pthread_mutex_lock(&(canal->lock));
    while(!(canal->lector_intencion))
        pthread_cond_wait(&(canal->puede_seguir_escritor), &(canal->lock));
    canal->valor = v;
    canal->escritor_termino = 1;
    canal->lector_intencion = 0;
    pthread_mutex_unlock(&(canal->lock));
    pthread_cond_signal(&(canal->puede_seguir_lector));
}

int channel_read(channel *canal)
{   
    pthread_mutex_lock(&(canal->lock));
    int retorno;
    canal->lector_intencion = 1;
    pthread_cond_signal(&(canal->puede_seguir_escritor));
    while(!(canal->escritor_termino))
        pthread_cond_wait(&(canal->puede_seguir_lector), &(canal->lock));
    retorno = canal->valor;
    canal->escritor_termino = 0;
    pthread_mutex_unlock(&(canal->lock));
    return retorno;
}