/*
Ej. 9 (Barreras). Una barrera para n threads tiene una ´unica operaci´on barrier_wait() que causa
que los threads se pausen hasta que todos lleguen a la barrera. Son usadas, generalmente, para asegurar
que las iteraciones de varios bucles en paralelo proceden a un mismo paso. Implemente una librer´ıa de
barreras, exponiendo las funciones:

void barrier_init(struct barrier *b, int n);
void barrier_wait(struct barrier *b);

Uselas para corregir el siguiente fragmento. La funci´on ´ calor() simula la transferencia de calor en un
material, haciendo que cada elemento del array se “acerque” a sus vecinos, dejando el resultado de la
transformaci´on en un nuevo array.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include "queue.h"
#include "barrier.h"

#define N_ASIENTOS 10
#define N_CLIENTES 150

int asientos_ocupados = 0;
int sillas[N_ASIENTOS] = {0};
int barbero_dormido = 1;
pthread_cond_t cliente_disponible;
pthread_cond_t sillas_cond[N_ASIENTOS];
pthread_mutex_t lock_barber = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_cliente = PTHREAD_MUTEX_INITIALIZER;
Queue *fila_clientes;
barrier barrera;

void clientef()
{
    // funcion que simula el cliente llegando y fijandose si hay lugar
    // en caso que haya, que mande una signal al barbero para que se despierte wacho
    // si no, se va
    // lock
    usleep(random() % 10000000);
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
    barrier_wait(&barrera);

    pagando(asiento_elegido);
    barrier_wait(&barrera);

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
        usleep(random() % 500000);
        while(asientos_ocupados == 0)
        {
            barbero_dormido = 1;
            pthread_cond_wait(&cliente_disponible, &lock_barber);
        }

        barbero_dormido = 0;

        int silla_a_atender = desencolar(fila_clientes);
        pthread_cond_signal(&(sillas_cond[silla_a_atender]));
        
        cortando(silla_a_atender);
        barrier_wait(&barrera);
        
        me_pagan(silla_a_atender);
        barrier_wait(&barrera);
        
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
    barrier_init(&barrera, 2);
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