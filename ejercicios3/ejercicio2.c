#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>


//Compilar: mpic++ ejercicio2.c -o ejercicio2.out 
//Ejecular: mpirun -n 4 ./ejercicio2.out 1000000 

int main(int argc, char **argv)
{
	srand(time(NULL));
	const long long int top=200000;
	int comm_sz, my_rank;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int number_of_tosses=atoi(argv[1]);
	if(number_of_tosses%comm_sz!=0)
	{
		if(my_rank == 0)
			printf("error\n");
		MPI_Finalize();
		return 0;
	}
	int size = number_of_tosses/comm_sz;
	int local_sum=0;
	int number_in_circle=0;
	
	for(int i=0;i<size;i++)
	{
		double x=((double)(rand()%top))/(top/2)-1;
		double y=((double)(rand()%top))/(top/2)-1;
		double distance_square=(x*x)+(y*y);
		if(distance_square <=1) local_sum++;
	}
	MPI_Reduce(&local_sum,&number_in_circle,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	if (my_rank == 0)
	{
		float pi_estimate = (4.0*number_in_circle)/(number_of_tosses);
		printf("Valor de pi(estimado): %f\n", pi_estimate);
	}
	MPI_Finalize();
	return 0;
}

