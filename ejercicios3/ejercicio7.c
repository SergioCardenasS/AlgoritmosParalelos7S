#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// Compilar: mpicc ejercicio7.c -o ejercicio7.out
// Ejecutar: mpirun -n 2 ./ejercicio7.out 50

int main(int argc, char** argv)
{
	double LIMIT = atoi(argv[1]);
	double inicio, fin, tiempo, average;
	int comm_sz, i, my_rank;
	int temp = 0;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if(comm_sz != 2)
	{
		if(my_rank==0)
			printf("Solo 2 threads\n");
		MPI_Finalize();
		return 0;
	}
	if(my_rank == 0)
	{
		average=0;
		for(i=0 ; i<LIMIT; i++)
		{
			inicio = MPI_Wtime();
			MPI_Send(&temp, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(&temp, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			fin = MPI_Wtime();
			tiempo = fin - inicio;
			average+=tiempo;
		}
		average=average/LIMIT;
		printf("Tiempo: %f\n",average);
	}
	else if(my_rank == 1)
	{
		for(i=0 ; i<LIMIT; i++)
		{
			MPI_Recv(&temp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Send(&temp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return 0;
}