#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define TAM 4

//compilar: mpicc -std=c99  ejercicio6.c -o ejercicio6.out    
//ejecutar: mpirun -n 5 ./ejercicio6.out

int main(void)
{
	int matrix[TAM][TAM] = {{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}};
	int rvector[TAM]={1,1,1,1};
	int SIZE = sizeof(int);
	int comm_sz, my_rani;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rani);
	int real_size=comm_sz-1;
	int size_matriz=sqrt(TAM);
	int rec, i,j,l;
	if (my_rani < real_size)
	{
		l=0;
		for (i = size_matriz*((my_rani/size_matriz)%size_matriz); i < size_matriz*(((my_rani/size_matriz)%size_matriz)+1); ++i)
		{
			rec = 0.0;
			for (j = size_matriz*(my_rani%size_matriz); j < size_matriz*((my_rani%size_matriz)+1); j++)
				rec += matrix[i][j]*rvector[j];
			MPI_Send(&rec, SIZE, MPI_INT, real_size, my_rani*size_matriz+l,MPI_COMM_WORLD);
			++l;
		}
	}
	else
	{
		int result_vector[TAM*size_matriz];
		for (i=0; i<TAM*size_matriz; ++i)
		{
			int owner=i/size_matriz;
			MPI_Recv(&rec, SIZE, MPI_INT, owner,i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			result_vector[i]=rec;
		}
		printf("%d\n%d\n%d\n%d\n", result_vector[0]+result_vector[2],result_vector[1]+result_vector[3],result_vector[4]+result_vector[6],result_vector[5]+result_vector[7]);
	}
	MPI_Finalize();
	return 0;
}
