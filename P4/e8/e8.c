/*
Ej. 8 (Suma Distribuida). Implemente en MPI un programa distribuido que compute la suma de un
array distribuyendo segmentos del mismo. Su soluci´on debe ser robusta si var´ıa el tama˜no del array y/o la
cantidad de procesos involucrados.
Corra en programa con distintas cantidad de procesos: 4, 8, y 16.
*/

#include <stdio.h>
#include <mpi.h>

#define TAM_ARR 100

int main(int argc, char **argv)
{
    int process_Rank, size_Of_Comm;
    int arr[TAM_ARR];
    int root_rank = 0;
    for (int i = 0; i < TAM_ARR; i++) arr[i] = i;

    MPI_Init(&argc, &argv); // needed
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Comm);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    int suma = 0; // reduction
    int suma_parcial = 0; // suma local de cada proceso
    
    int inicio = (TAM_ARR / size_Of_Comm) * process_Rank;
    int final = inicio + (TAM_ARR / size_Of_Comm);

   if (process_Rank == size_Of_Comm - 1) final = TAM_ARR;

    for(;inicio < final; inicio++)
    {
        suma_parcial += arr[inicio];
    }

    MPI_Reduce(&suma_parcial, &suma, 1, MPI_INT, MPI_SUM, root_rank, MPI_COMM_WORLD);
    if(process_Rank == root_rank)
    {
        printf("The sum is %d.\n", suma);
    }
    MPI_Finalize(); // also needed

    return 0;
}