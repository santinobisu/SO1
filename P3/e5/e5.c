/*
Ej. 5 (Lectores y Escritores, Parnas). El problema de los lectores y escritores consiste en M hilos
lectores y N escritores tratando de acceder a un arreglo en memoria compartida con las siguientes
restricciones:

No puede haber un lector accediendo al arreglo al mismo tiempo que un escritor.
Varios lectores pueden acceder al arreglo simult´aneamente.
S´olo puede haber un escritor a la vez.

La soluci´on a este problema es esencialmente un read-write lock. Los siguientes apartados piden implementar
distintas variantes. Las mismas deben ser reusables: el read-write lock no debe estar atado de ninguna
forma a este problema en particular (e.g. no debe saber las cantidades de escritores y lectores, ni asumir
que un proceso es siempre lector o siempre escritor). Organice su respuesta separando claramente la
implementaci´on del lock de su uso en el problema (idealmente en archivos separados). La interfaz deber´ıa
ser similar a la ofrecida por los mutex de pthread.

a) Implemente una soluci´on y expl´ıquela. Para este apartado, siempre que el lock est´e tomado por un
lector y aparezca un segundo lector, el segundo debe poder entrar inmediatamente (read-preferring).

b) Si hay varios lectores que continuamente intentan leer el arreglo, esto puede llevar a starvation de
los escritores (explique por qu´e). Modifique su soluci´on para que, cuando un escritor desee entrar a
su RC, ning´un lector pueda entrar hasta que ese escritor termine (write-preferring).

c) Ahora, podemos tener el problema inverso: si muchos escritores intentan entrar, pueden dejar fuera
a los lectores. Implemente una variante justa que respete el orden en el que los threads pidieron
entrar a la RC.
*/

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
    int v, i;
    int num = arg - (void *)0;
    
    while (1)
    {
        sleep(random() % 3);
        pthread_mutex_lock(&lock);

        lectores++;
        if (lectores == 1)
            sem_wait(&sem);

        pthread_mutex_unlock(&lock);
    
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

        pthread_mutex_lock(&lock);

        lectores--;
        if(lectores == 0)
            sem_post(&sem);

        pthread_mutex_unlock(&lock);

    }
    
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

/* int arr[ARRLEN];
sem_t sem; 
int lectores = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t puede_leer;
pthread_cond_t puede_escribir;

int lectores_activos = 0;
int escritores_activos = 0;
int escritores_esperando = 0;

void *escritor(void *arg)
{
    int i;
    int num = arg - (void *)0;
    while (1)
    {   
        sleep(random() % 3);
        pthread_mutex_lock(&lock);
        escritores_esperando++;
        while(lectores_activos > 0 || escritores_activos > 0)
        pthread_cond_wait(&puede_leer,&lock);
        escritores_esperando--;
        escritores_activos= 1;
        pthread_mutex_unlock(&lock);

        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++)
        arr[i] = num;
        pthread_mutex_lock(&lock);
        escritores_activos = 0;
        if (escritores_esperando > 0)
            pthread_cond_signal(&puede_escribir);
        else
            pthread_cond_broadcast(&puede_leer);
        pthread_mutex_unlock(&lock);

    }
  
    return NULL;
}

void *lector(void *arg)
{
    int v, i;
    int num = arg - (void *)0;
    
    while (1)
    {
        sleep(random() % 3);
        pthread_mutex_lock(&lock);
        while(escritores_activos > 0 || escritores_esperando > 0 )
        pthread_cond_wait(&puede_leer, &lock);
        pthread_mutex_unlock(&lock);
    
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

        pthread_mutex_lock(&lock);

        lectores--;
        if(lectores_activos == 0)
            pthread_cond_signal(&puede_escribir);

        pthread_mutex_unlock(&lock);

    }
    
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
    pthread_join(lectores[0], NULL); 
    return 0;
} */