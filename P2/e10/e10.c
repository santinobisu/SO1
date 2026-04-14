/* Implementar un programa con dos threads que suman los elementos de un arreglo compartido
usando pthread mutex t para evitar race conditions. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#define TAM 10

int sarr[TAM] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int suma = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *sumar_pmitad(void *arr)
{
    for(int i = 0; i < TAM/2; i++)
    {
        pthread_mutex_lock(&lock);
        suma += sarr[i];
        pthread_mutex_unlock(&lock);
    }

}

void *sumar_smitad(void *arr)
{
    for(int i = TAM/2; i < TAM; i++)
    {
        pthread_mutex_lock(&lock);
        suma += sarr[i];
        pthread_mutex_unlock(&lock);
    }
}



int main()
{
    pthread_t h1, h2;
    pthread_create(&h1, NULL, sumar_pmitad, sarr);
    pthread_create(&h2, NULL, sumar_smitad, sarr);
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);

    printf("Suma: %d\n", suma);

    pthread_mutex_destroy(&lock);

    return 0;
}