/*
Ej. 10 (Suma y Consenso). En el ejercicio anterior, logramos que N procesos sumen sus variables
privadas y todos conozcan el resultado en N − 1 pasos (paso = env´ıo/recepci´on de un mensaje). Si N es
muy grande (ej. miles de procesos) esto puede introducir una latencia muy alta. Dise˜ne una manera de
realizarlo en lg2 N pasos (puede asumir que N es potencia de 2). Todo proceso debe usar memoria O(1).
Verifique que su soluci´on es robusta.
*/

#include <stdio.h>
#include <mpi.h>

/* int potencia_dos(int i)
{
    int resultado = 1;
    for (int j = 0; j < i; j++)
        resultado *= 2;
    return resultado;
}

int  main(int argc, char ** argv)
{
    int process_Rank, size_Of_Comm;

    MPI_Init(&argc, &argv); // needed
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Comm);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    int suma = process_Rank;
    int recibido = 0;
    int i = 1;

    while(potencia_dos(i) < size_Of_Comm)
    {
        int j = potencia_dos(i);
        i++;
        if(!(process_Rank % j) && process_Rank < size_Of_Comm - i)
        {
            MPI_Send(&suma, 1, MPI_INT, process_Rank + i, process_Rank, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Recv(&recibido, 1, MPI_INT, process_Rank - i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            suma += recibido;
        }
        
    }

    MPI_Bcast(&suma, 1, MPI_INT, size_Of_Comm - 1, MPI_COMM_WORLD);

    printf("Proceso %d: Mi suma final es %d\n", process_Rank, suma);

    MPI_Finalize(); // also needed
    return 0; 
} */

int main(int argc, char **argv)
{
    int rank, nprocs;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int suma = rank;   // dato local
    int recibido;

    /* REDUCCION */
    for (int dist = 1; dist < nprocs; dist *= 2)
    {
        if (rank % (2 * dist) == 0)
        {
            MPI_Recv(&recibido, 1, MPI_INT,
                     rank + dist, 0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            suma += recibido;
        }
        else if (rank % dist == 0)
        {
            MPI_Send(&suma, 1, MPI_INT,
                     rank - dist, 0,
                     MPI_COMM_WORLD);

            break;
        }
    }

    /* BROADCAST */
    for (int dist = nprocs / 2; dist >= 1; dist /= 2)
    {
        if (rank % (2 * dist) == 0)
        {
            if (rank + dist < nprocs)
            {
                MPI_Send(&suma, 1, MPI_INT,
                         rank + dist, 0,
                         MPI_COMM_WORLD);
            }
        }
        else if (rank % dist == 0)
        {
            MPI_Recv(&suma, 1, MPI_INT,
                     rank - dist, 0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        }
    }

    printf("Proceso %d: suma = %d\n", rank, suma);

    MPI_Finalize();
    return 0;
}