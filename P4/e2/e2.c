/*
Ej. 2 (Suma Paralela). Escriba utilizando OpenMP un algoritmo que calcule la suma de un arreglo
de N = 5 × 108 doubles. Compare la performance con la implementaci´on secuencial usando distintos
n´umeros de hilos. Compare tambi´en con una versi´on paralela que usa un mutex para proteger la variable
que lleva la suma.

La performance en pocos hilos es mas lenta, pero no tanto en comparacion a tener todos los hilos disponibles (para la cantidad de
5 * 10^8 numeros).
La performance si se usa mutex es muy mala; el programa tarda muchisimo en procesar tantos datos si cada proceso tiene que esperar
un mutex para sumar, aparte de que cada nucleo se pone al maximo.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

int num;
double *arr;

double suma = 0;

int main()
{
    omp_set_num_threads(1);

    num = pow(10,8);
    num = 5 * num;

    arr = malloc(sizeof(double)*num);

    for(int i = 0; i < num; i++)
    {
        arr[i] = 1;
    }

    
    #pragma omp parallel for reduction(+: suma)
    for(int i = 0; i < num; i++)
    {
        suma += arr[i];
    }

    printf("Suma: %f\n", suma);

    free(arr);

}   