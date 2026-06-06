/*
Ej. 7 (Mergesort). Siguiendo la misma idea del ejercicio anterior, implemente un mergesort (sobre
enteros) paralelo y compare su performance con la versi´on secuencial. Puede usar tasks, o escribir una
versi´on bottom-up usando solamente parallel for. Su soluci´on deber´ıa manejar arreglos de 500 millones
de enteros sin problema, y ser lo m´as eficiente posible.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>
#define N 500000000

void merge(int arr[], int inicio, int medio, int fin) {
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;

    int *izq = malloc(n1 * sizeof(int));
    int *der = malloc(n2 * sizeof(int));

    
        for (int i = 0; i < n1; i++)
        izq[i] = arr[inicio + i];

       
        for (int i = 0; i < n2; i++)
        der[i] = arr[medio + 1 + i];

        int i = 0, j = 0, k = inicio;

        
        while (i < n1 && j < n2) {
        if (izq[i] <= der[j]) {
            arr[k] = izq[i];
            i++;
        } else {
            arr[k] = der[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = izq[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = der[j];
        j++;
        k++;
    }

free(izq);
free(der);
}
    
void mergesort(int arr[], int inicio, int fin) {
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;
        
        mergesort(arr, inicio, medio);
        mergesort(arr, medio + 1, fin);
        
        merge(arr, inicio, medio, fin);
    }
}

void parallel_mergesort(int arr[], int inicio, int fin) {
    if (inicio >= fin)
        return;

    int medio = inicio + (fin - inicio) / 2;

    if (fin - inicio < 10000) {
        mergesort(arr, inicio, fin);
        return;
    }

    #pragma omp task
    parallel_mergesort(arr, inicio, medio);

    #pragma omp task
    parallel_mergesort(arr, medio + 1, fin);

    #pragma omp taskwait

    merge(arr, inicio, medio, fin);
}

int main() {
    int *arr1 = malloc(N * sizeof(int));
    int *arr2 = malloc(N * sizeof(int));
    srand(42);

     for (int i = 0; i < N; i++) 
    {
        int j = rand();
        arr1[i] = j;
        arr2[i] = j;
    }


    double inicio, fin, t_sec;

    inicio = omp_get_wtime();
    mergesort(arr1, 0, N - 1);
    fin = omp_get_wtime();
    t_sec = fin - inicio;
    printf("[Secuencial] | Tiempo: %f seg\n", t_sec);

    inicio = omp_get_wtime();
    #pragma omp parallel
    {
    #pragma omp single
    {
        parallel_mergesort(arr2, 0, N - 1);
    }
    }
    fin = omp_get_wtime();
    t_sec = fin - inicio;
    printf("[Paralelo] | Tiempo: %f seg\n", t_sec);

    return 0;
}