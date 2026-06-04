/*
Ej. 6 (Quicksort). Recordemos el algoritmo de ordenamiento Quicksort:

Dado que las llamadas recursivas para ordenar las “mitades” del arreglo son independientes, son un claro
candidato para paralelizar.
Como primer intento, escriba una versi´on que use pthread_create para paralelizar las llamadas
recursivas. Compare el rendimiento con la versi´on secuencial para distintos tama˜nos del array. ¿Hay
alg´un problema? Explique.
Hay un problema dado que en cada llamada recursiva cada hilo crea otro hilo haciendo que muchos de ellos tengan que esperar a que se libere un
procesador para poder ejecutarse
Escriba una versi´on que paralelice las llamadas usando sections de OpenMP. ¿Mejora la performance? ¿Cu´anto? Puede usar el servidor labdcc para probar en un quad-core.
Si, mejora la performance hasta 10 veces aprox. 
Escriba una versi´on usando tasks de OpenMP y mida el cambio en rendimiento.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>

#define N 30000000

typedef struct _tupla
{
    int * arr;
    int largo;
}tupla;
 
/* Particion de Lomuto, tomando el primer elemento como pivote */

void swap(int *a, int *b)
{
    int valor = *b;
    *b = *a;
    *a = valor;
}

int particionar(int a[], int n)
{
    int i, j = 0;
    int p = a[0];
    swap(&a[0], &a[n - 1]);
    for (i = 0; i < n - 1; i++)
    {
        if (a[i] <= p)
            swap(&a[i], &a[j++]);
    }
    swap(&a[j], &a[n - 1]);
    return j;
}

void qsort1(tupla *t)
{
    if (t->largo < 2)
        return;

    int p = particionar(t->arr, t->largo);

    tupla t1;
    t1.arr= t->arr;
    t1.largo = p;

    tupla t2;
    t2.arr = t->arr + p + 1;
    t2.largo = t->largo - p - 1;

    pthread_t h1,h2;

    pthread_create(&h1, NULL, qsort1, &t1);
    pthread_create(&h2, NULL, qsort1, &t2);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
}


void qsort2(int a[], int n)
{
    if (n < 2)
        return;

    int p = particionar(a, n);

    #pragma omp parallel sections
    {
        #pragma omp section
        qsort2(a, p);

        #pragma omp section
        qsort2(a + p + 1, n - p - 1);
    }
}

void qsort3(int a[], int n)
{
    if (n < 1000) {
        qsort2(a, n);   // o una versión secuencial
        return;
    }

    int p = particionar(a, n);

    #pragma omp task
    qsort3(a, p);

    #pragma omp task
    qsort3(a + p + 1, n - p - 1);

    #pragma omp taskwait
}

/* void qsort3(int a[], int n)
{
    if (n < 2)
        return;

    int p = particionar(a, n);

    
    #pragma omp task
    qsort3(a, p);

    #pragma omp task
    qsort3(a + p + 1, n - p - 1);

    #pragma omp taskwait
} */

int main()
{   


    int *arr1 = malloc(N * sizeof(int));
    int *arr2 = malloc(N * sizeof(int));
    int *arr3 = malloc(N * sizeof(int));
    srand(42);

    for (int i = 0; i < N; i++) 
    {
        int j = rand();
        arr1[i] = j;
        arr2[i] = j;
        arr3[i] = j;
    }

    tupla t;
    t.arr = arr1;
    t.largo = N;

    /*  qsort3(arr3,13);
    for(int i = 0; i < 13; i++){
        printf("%d ", t.arr[i]);
    }
    puts(" "); */

    double inicio, fin, t_sec;
    
/*     inicio = omp_get_wtime();
    qsort1(&t);
    fin = omp_get_wtime();
    t_sec = fin - inicio;
    printf("[qsort 1] | Tiempo: %f seg\n", t_sec);
 */
    inicio = omp_get_wtime();
    
    qsort2(arr2, N);
    fin = omp_get_wtime();
    t_sec = fin - inicio;
    printf("[qsort 2] | Tiempo: %f seg\n", t_sec);

    inicio = omp_get_wtime();
    #pragma omp parallel 
    {   
        #pragma omp single
        {
            qsort3(arr3, N);    
        }
    }
    fin = omp_get_wtime();
    t_sec = fin - inicio;
    printf("[qsort 3] | Tiempo: %f seg\n", t_sec);

    
}