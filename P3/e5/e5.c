#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define M 5
#define N 5
#define ARRLEN 10240

int arr[ARRLEN];
sem_t sem; 
int lectores = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *escritor(void *arg)
{
    int i;
    int num = arg - (void *)0;
    while (1)
    {
        sleep(random() % 3);
        sem_wait(&sem);  
        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++)
            arr[i] = num;
        sem_post(&sem);
    }
  
    return NULL;
}

void *lector(void *arg)
{
    // toma el lock con trylock, variable de condicion para ver si el lock fue tomado por un lector o escritor
    // si no puede tomarlo pero fue tomado por un lector, lee igual, en otro caso no lee
    int v, i;
    int num = arg - (void *)0;
    pthread_mutex_lock(&lock);

        lectores++;
        if (lectores == 1)
            sem_wait(&sem);

        pthread_mutex_unlock(&lock);
    
    while (1)
    {
        sleep(random() % 3);
        v = arr[0];
        for (i = 1; i < ARRLEN; i++)
        {
            if (arr[i] != v)
                break;
        }
        if (i < ARRLEN)
            printf("Lector %d, error de lectura\n", num);
        else
            printf("Lector %d, dato %d\n", num, v);
    }
    pthread_mutex_lock(&lock);
    lectores--;
    if(lectores == 0)
    sem_post(&sem);
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main()
{
    sem_init(&sem, 0, 1);
    pthread_t lectores[M], escritores[N];
    int i;
    for (i = 0; i < M; i++)
        pthread_create(&lectores[i], NULL, lector, i + (void *)0);
    for (i = 0; i < N; i++)
        pthread_create(&escritores[i], NULL, escritor, i + (void *)0);
    pthread_join(lectores[0], NULL); /* Espera para siempre */
    return 0;
}