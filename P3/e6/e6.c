/* Ej. 6 (Problema del Barbero, Dijkstra). Una barber´ıa tiene una sala de espera con N sillas y
un barbero. Si no hay clientes para atender, el barbero se pone a dormir. Si un cliente llega y todas
las sillas est´an ocupadas, se va. Si el barbero est´a ocupado pero hay sillas disponibles, se sienta en
una y espera a ser atendido. Si el barbero est´a dormido, despierta al barbero. El cliente y el barbero
deben ejecutar concurrentemente las funciones me_cortan() y cortando() y al terminar los dos ejecutar
concurrentemente pagando() y me_pagan().
Escriba un programa que coordine el comportamiento del barbero y los clientes y expl´ıquelo. */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include "queue.h"

#define N_ASIENTOS 5
#define N_CLIENTES 15

int asientos_ocupados = 0;
int sillas[N_ASIENTOS] = {0};
int barbero_dormido = 1;
pthread_cond_t cliente_disponible;
pthread_cond_t sillas_cond[N_ASIENTOS];
pthread_mutex_t lock_barber = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_cliente = PTHREAD_MUTEX_INITIALIZER;
Queue *fila_clientes;
pthread_barrier_t barrera;

void clientef()
{
    // funcion que simula el cliente llegando y fijandose si hay lugar
    // en caso que haya, que mande una signal al barbero para que se despierte wacho
    // si no, se va
    // lock
    usleep(random() % 5000000);
    pthread_mutex_lock(&lock_cliente);

    if(asientos_ocupados == N_ASIENTOS)
    {
        printf("No hay lugar\n");
        pthread_mutex_unlock(&lock_cliente);
        return;
    }

    int asiento_elegido = 0;
    while(sillas[asiento_elegido] == 1) asiento_elegido++;
    sillas[asiento_elegido] = 1;
    printf("Cliente eligió asiento %d\n", asiento_elegido);

    encolar(fila_clientes, asiento_elegido);
    asientos_ocupados++;

    if(barbero_dormido) pthread_cond_signal(&cliente_disponible);

    pthread_cond_wait(&(sillas_cond[asiento_elegido]), &lock_cliente);

    me_cortan(asiento_elegido);
    pthread_barrier_wait(&barrera);

    pagando(asiento_elegido);
    pthread_barrier_wait(&barrera);

    sillas[asiento_elegido] = 0;
    
    pthread_mutex_unlock(&lock_cliente);
}

void barberof()
{
    // funcion que simula el barbero empezando su jornada
    // primero me fijo si hay clientes para atender
    // si no hay, me duermo y si no me despierto y empiezo a atender
    while(1)
    {
        usleep(random() % 5000000);
        while(asientos_ocupados == 0)
        {
            barbero_dormido = 1;
            pthread_cond_wait(&cliente_disponible, &lock_barber);
        }

        barbero_dormido = 0;

        int silla_a_atender = desencolar(fila_clientes);
        pthread_cond_signal(&(sillas_cond[silla_a_atender]));
        
        cortando(silla_a_atender);
        pthread_barrier_wait(&barrera);
        
        me_pagan(silla_a_atender);
        pthread_barrier_wait(&barrera);
        
        asientos_ocupados--;
    }
}

void me_cortan(int silla_ocupada)
{
    printf("Silla %d: me estan cortando\n", silla_ocupada);
}

void cortando(int silla_ocupada)
{
    printf("Barbero: estoy cortando a Silla %d\n", silla_ocupada);

}

void pagando(int silla_ocupada)
{
    printf("Silla %d: estoy pagando\n", silla_ocupada);

}

void me_pagan(int silla_ocupada)
{
    printf("Barbero: Silla %d me está pagando\n", silla_ocupada);
}

int main()
{
    fila_clientes = crear_cola();

    pthread_t barbero, clientes[N_CLIENTES];

    pthread_barrier_init(&barrera, NULL, 2);
    int i;
    pthread_create(&barbero, NULL, barberof, NULL);
    for (i = 0; i < N_CLIENTES; i++)
        pthread_create(&clientes[i], NULL, clientef, NULL);
    
    pthread_join(barbero, NULL);
    for (i = 0; i < N_CLIENTES; i++)
        pthread_join(clientes[i], NULL);

    return 0;
}