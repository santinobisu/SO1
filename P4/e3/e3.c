/*
Ej. 3 (B´usqueda del M´ınimo). Escriba utilizando OpenMP un algoritmo que dado un arreglo de
N = 5 × 108
enteros busque el m´ınimo. Compare la performance con la implementaci´on secuencial con
distinto n´umero de hilos. 
La performance con distinta cantidad de hilos no cambia.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int num;
double *arr;

double minimo;

int main()
{
    srand(time(NULL));
    omp_set_num_threads(24);

    num = pow(10,8);
    num = 5 * num;

    arr = malloc(sizeof(double)*num);

    for(int i = 0; i < num; i++)
    {
        arr[i] = rand();
    }

    minimo = arr[0];
    
    #pragma omp parallel for reduction(min: minimo)
    for(int i = 0; i < num; i++)
    {
        if(arr[i] < minimo) minimo = arr[i];
    }

    printf("Minimo: %f\n", minimo);

    free(arr);

}   