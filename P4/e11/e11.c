/*
Ej. 11 (Producto distribuido). El siguiente fragmento de c´odigo permite calcular el producto de una
matriz cuadrada por un vector, ambos de la misma dimensi´on
a) Escriba un programa MPI que realice el producto en paralelo, teniendo en cuenta que el proceso 0 lee
la matriz A y el vector v, realiza una distribuci´on de A por bloques de filas consecutivas sobre todos los
procesos y env´ıa v a todos los procesos. Asimismo, al final el proceso 0 debe obtener el resultado.
b) Calcular el speed up y la eficiencia.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 200

int **crear_matriz()
{
    int **m = malloc(N * sizeof(int *));
    int *datos = malloc(N * N * sizeof(int));

    for (int i = 0; i < N; i++)
        m[i] = datos + i * N;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            m[i][j] = j + 1;

    return m;
}
void destruir_matriz(int **m)
{
    free(m[0]); // bloque de datos
    free(m);    // filas
}


int main(int argc, char **argv)
{
    int i, j;
    int **A = crear_matriz();
    int v[N];

    for(int i = 0; i < N; i++)
        v[i] = 1;

    int x[N];
    
    int rank, nprocs;
    int inicio, fin;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if(rank == 0)
    {
        inicio = 0;
        fin = 0;
        for(int i = 1; i < nprocs; i++)
        {
        
            int chunk = N / (nprocs - 1);

            inicio = (i - 1) * chunk;
            fin = i * chunk;

            if (i == nprocs - 1)
                fin = N;
            MPI_Send(&inicio, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&fin, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        int arr[2];
        for(int i = 0; i < N; i++)
        {
            
            MPI_Recv(arr, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            x[arr[0]] = arr[1];
        }
        printf("Vector final: ");
        for(int i = 0; i < N; i++) printf("%d ", x[i]);
        puts("");
    }
    else
    {
        MPI_Recv(&inicio, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&fin, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // multiplicar
        for (i = inicio; i < fin; i++)
        {
            x[i] = 0;
            for (j = 0; j < N; j++)
                x[i] += A[i][j] * v[j];
        }
        int arr[2];
        while(inicio < fin)
        {
            arr[0] = inicio;
            arr[1] = x[inicio];
            MPI_Send(arr, 2, MPI_INT, 0, rank, MPI_COMM_WORLD);
            inicio++;
        }
    }
    


    MPI_Finalize();
    return 0;
}