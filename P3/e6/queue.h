#ifndef __QUEUE__H__
#define __QUEUE__H__
#include <pthread.h>

typedef struct nodo
{
    int id;
    struct nodo *next;
} Nodo;

typedef struct queue
{   
    pthread_mutex_t lock;
    Nodo *primero;
    Nodo *ultimo;
} Queue;

Queue *crear_cola();
void encolar(Queue *cola, int id);
int desencolar(Queue *cola);
void destruir_cola(Queue *cola);

#endif