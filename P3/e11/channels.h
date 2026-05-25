#ifndef __CHANNELS_H__
#define __CHANNELS_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct _channel
{
    pthread_mutex_t lock;
    pthread_cond_t puede_seguir_escritor;
    pthread_cond_t puede_seguir_lector;
    int valor;
    int lector_intencion;
    int escritor_termino;
} channel;

void channel_init(channel *canal);
void channel_write(channel *canal, int v);
int channel_read( channel *canal);



#endif