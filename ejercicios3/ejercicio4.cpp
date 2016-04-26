#include <iostream>
#include <mpi.h>

using namespace std;

//Compilar: mpicxx --std=c++11 ejercicio4.cpp -o ejercicio4.out
//Ejecutar: mpiexec -np 4 ./ejercicio4.out 12 

int abs_(int n){if(n<0)return -n;return n;}

int main(int argc, char * argv[])
{
	int comm_size, current_rank;
	int * my_vector = NULL, total = 0;
	int parts, length, odd;
	length = atof(argv[1]);

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);

	if(current_rank == 0)
	{		
		my_vector = new int[length];
		parts = length/comm_size;
		cout<<"List : ";
		for(size_t i=0 ; i<length ; ++i)
		{
			my_vector[i] =random() % length;
			cout<<my_vector[i]<<" ";
		}
		cout<<endl;
	}
	
	MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&parts, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(my_vector == NULL) my_vector = new int[length];

	MPI_Bcast(my_vector, length, MPI_INT, 0, MPI_COMM_WORLD);

	for(size_t i=current_rank*parts ; i<((current_rank+1)*parts) ; ++i)
		total += my_vector[i];

	for(size_t j=1,f ; j<comm_size ; j<<=1)
	{		
		if(!((current_rank/j)&1)) MPI_Sendrecv(&total, 1, MPI_INT, current_rank+j, 0, &odd, 1, MPI_INT, current_rank+j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	
		else MPI_Sendrecv(&total, 1, MPI_INT, abs_(current_rank-j), 0, &odd, 1, MPI_INT, abs_(current_rank-j), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		total += odd;
	}

	if(current_rank == 0)
		cout << "Global sum : " << total << endl;

	MPI_Finalize();
	return 0;
}