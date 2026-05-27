/*
Ej. 1. Para calentar motores, adapte a OpenMP su soluci´on del jard´ın ornamental usando el Algoritmo
de la Panader´ıa de Lamport.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <omp.h>
#define VISITANTE 5000
#define NUM_HILOS 24
int visitantes = 0;
int numero[NUM_HILOS] = {0};
int eligiendo[NUM_HILOS] = {0};

int buscarMax(int arr[])
{
    int max = 0;
    for (int i = 0; i < NUM_HILOS; i++) if (arr[i] > max) max = arr[i];
    return max;
}

void lock(int i)
{
    //
    eligiendo[i] = 1;
    #pragma omp flush
    numero[i] = 1 + buscarMax(numero);
    #pragma omp flush
    eligiendo[i] = 0;
    #pragma opm flush

    for (int j = 0; j < NUM_HILOS; j++)
    {
        while(eligiendo[j]);
        while((numero[j] != 0) && ((numero[j] < numero[i]) || (numero[j] == numero[i]) && (j < i)));
    }
}

void unlock(int i)
{
    numero[i] = 0;
}

void molinete(int c){
    for(int i = 0; i < VISITANTE; i++){

        lock(c);
        visitantes++;
        unlock(c);
    }
}


int main(){
    
    
    pthread_t molinetes[NUM_HILOS];
    int i;

    #pragma omp parallel 
    {
        int id = omp_get_thread_num();
        molinete(id);
    }

    printf("%d\n", visitantes);
    return 0;

}