/*
Ej. 9 (Suma y Consenso por Rotaci´on). Considere en MPI un anillo de N procesos (con N
configurable) en el que cada proceso tiene alg´un valor privado (ej. su rango). Queremos computar la suma
de todos los valores, y que la misma resulte disponible en cada proceso. Implemente esto haciendo que
Como primer paso, cada proceso env´ıa su valor hacia el siguiente proceso del anillo.
Cada proceso recibe el valor y lo agrega a su suma.
Cada proceso reenv´ıa el mismo valor que recibi´o hacia el siguiente.
Al hacer esto, luego de N − 1 pasos, cada proceso deber´ıa tener la suma total computada.
*/
#include <stdio.h>
#include <mpi.h>

int main(int argc , char ** argv)
{
    int process_Rank, size_Of_Comm;

    MPI_Init(&argc, &argv); // needed
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Comm);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    int suma = process_Rank;
    int recibido = 0;

    if(process_Rank > 0)
    {
        MPI_Recv(&recibido, 1, MPI_INT, process_Rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        suma += recibido;
        printf("Proceso %d: Mi suma parcial es %d\n", process_Rank, suma);
    }

    if(process_Rank < size_Of_Comm - 1)
    {
        MPI_Send(&suma, 1, MPI_INT, process_Rank + 1, process_Rank, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Send(&suma, 1, MPI_INT, 0, process_Rank, MPI_COMM_WORLD);
    }

    if(process_Rank == 0)
    {
        MPI_Recv(&suma, 1, MPI_INT, size_Of_Comm - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Proceso %d: Mi suma es %d\n", process_Rank, suma);
    }

    MPI_Bcast(&suma, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    printf("Proceso %d: Mi suma final es %d\n", process_Rank, suma);

    MPI_Finalize(); // also needed
    return 0;
}