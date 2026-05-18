#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

Queue *crear_cola()
{
    Queue *cola = malloc(sizeof(Queue));
    cola->primero = NULL;
    cola->ultimo = NULL;
    pthread_mutex_init(&(cola->lock), NULL);
    return cola;
}

void encolar(Queue *cola, int id)
{
    Nodo *nodo = malloc(sizeof(Nodo));
    nodo->id = id;
    nodo->next = NULL;
    if(cola->primero == NULL)
    {
        cola->primero = nodo;
        cola->ultimo= nodo;
    }
    else 
    {
        cola->ultimo->next = nodo;
        cola->ultimo = nodo;
    }
}

int desencolar(Queue *cola)
{
    if (cola->primero != NULL)
    {
        Nodo *nuevo_primero = cola->primero->next;
        int id_desencolado = cola->primero->id;
        free(cola->primero);
        cola->primero = nuevo_primero;
        return id_desencolado;
    }

    return -1;
}

void destruir_cola(Queue *cola)
{
    while(cola->primero != NULL)
        desencolar(cola);
    free(cola);
}