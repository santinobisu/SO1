/*
Ej. 5 (Multiplicaci´on de Matrices). Implemente en OpenMP la multiplicaci´on de dos matrices en
paralelo.
a) Compare la performance con la soluci´on secuencial para matrices cuadradas de tama˜no 200x200,
500x500 y 1000x1000. ¿Qu´e relaci´on aproximada puede inferir entre los tiempos en uno y otro caso?
Se puede inferir que la paralela es mucho más rápida y eficiente.
b) Si se cambia el orden de los ´ındices, ¿se puede mejorar el rendimiento? ¿Por qu´e?
Al cambiar el orden de los índices a i, k, j, se aprovecha la localidad espacial de la memoria caché. Como el lenguaje C almacena las matrices por filas, 
hacer que el bucle más interno recorra las columnas (j) garantiza que los accesos a las matrices B y C sean contiguos en memoria, 
reduciendo drásticamente los cache misses (fallos de caché) y acelerando el programa de forma masiva (a veces hasta 10 veces más rápido).
c) Si tuviese que computar la multiplicaci´on de A × BT
, ¿se puede mejorar el rendimiento? ¿Por qu´e?
Si se puede mejorar el rendimiento debido a como la cache almacena los datos de la matriz en la RAM, haciendo el acceso efectivo más rápido.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define N 4000
int A[N][N], B[N][N], Bt[N][N], C[N][N];
/*
void mult(int A[N][N], int B[N][N], int C[N][N])
{
    int i, j, k;
        #pragma omp parallel for 
        for (i = 0; i < N; i++)
            #pragma omp parallel for 
            for (j = 0; j < N; j++)
                #pragma omp parallel for 
                for (k = 0; k < N; k++)
                    C[i][j] += A[i][k] * B[j][k];
    

} */

void mult(int A[N][N], int B[N][N], int BT[N][N], int C[N][N]) {
    // 1. Transponer B en paralelo
    int j = 0;
    #pragma omp parallel for private(j)
    for (int i = 0; i < N; i++) {
        for (; j < N; j++) {
            BT[j][i] = B[i][j];
        }
    }
  
    int k = 0;
    // 2. Multiplicar usando BT (Fila de A x Fila de BT) con orden tradicional
    #pragma omp parallel for private(j,k)
    for (int i = 0; i < N; i++) {
        for (; j < N; j++) {
            for (; k < N; k++) {
                C[i][j] += A[i][k] * BT[j][k]; // Fíjate en el [j][k], ¡ambos usan k al final!
            }
        }
    }
}



/* void mult_secuencial(int A[N][N], int B[N][N], int C[N][N])
{
    int i, j, k;
    for (i = 0; i < N; i++)
        for (k = 0; k < N; k++)
            for (j = 0; j < N; j++)
                C[i][j] += A[i][k] * B[k][j];
} */


void mult_secuencial(int A[N][N], int B[N][N], int BT[N][N], int C[N][N]) {
    // 1. Transponer B en paralelo
   
    for (int i = 0; i < N; i++) {
        for ( int j = 0; j < N; j++) {
            BT[j][i] = B[i][j];
        }
    }
  
   
    // 2. Multiplicar usando BT (Fila de A x Fila de BT) con orden tradicional
    for (int i = 0; i < N; i++) {
        for (int j = 0 ; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * BT[j][k]; // Fíjate en el [j][k], ¡ambos usan k al final!
            }
        }
    }
}

int main()
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            A[i][j] = random() % 1000;
            B[i][j] = random() % 1000;
        }
    }

    double inicio, fin, t_sec;

    inicio = omp_get_wtime();
    mult(A, B,  Bt, C);
    fin = omp_get_wtime();
    t_sec = fin - inicio;
    printf("[Paralelo] | Tiempo: %f seg\n", t_sec);

    inicio = omp_get_wtime();
    mult_secuencial(A, B,Bt, C);
    fin = omp_get_wtime();
    t_sec = fin - inicio;
    printf("[Secuencial] | Tiempo: %f seg\n", t_sec);

    
    return 0;
}
