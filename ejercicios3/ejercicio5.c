#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#define TAM 4

//Compilar: mpicc ejercicio5.c -o ejercicio5.out 
//Ejecutar: mpirun -n 5 ./ejercici5.o

int main(void)
{

	int matrix[TAM][TAM] = {{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}};
	int rvector[TAM]={1,2,3,4};
	int SIZE = sizeof(int);
	int comm_sz, my_rank,cont;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int real_size=comm_sz-1;
	if(real_size!=TAM)
	{
		if (my_rank == 0)
			printf("Es necesario de %d procesos esclavos\n",TAM);
		MPI_Finalize();
		return 0;
	}
	if (my_rank < real_size)
	{
		cont=0;
		for (int i = 0; i < TAM; ++i)
			cont+=matrix[my_rank][i]*rvector[i];
		MPI_Send(&cont, SIZE, MPI_INT, real_size, my_rank,MPI_COMM_WORLD);
	}
	else
	{
		for (int k=0; k<real_size; ++k)
		{
			MPI_Recv(&cont, SIZE, MPI_INT, k,k, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("%d\n",cont);
		}
	}
	MPI_Finalize();
	return 0;
} 