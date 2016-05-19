#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

//Compilar: gcc -fopenmp -o ejer2 ejer2.c
//Ejecular: ./ejer2 1000000

int main(int argc, char **argv)
{
	srand(time(NULL));
	const long long int top=200000;
	int number_of_tosses=atoi(argv[1]);
	int thread_count,local_sum=0;
	#pragma omp parallel num_threads(thread_count)
	{
		int my_rank = omp_get_thread_num();
		int comm_sz = omp_get_num_threads();
		int size = number_of_tosses/comm_sz;
		int number_in_circle=0;	
		for(int i=0;i<size;i++)
		{
			double x=((double)(rand()%top))/(top/2)-1;
			double y=((double)(rand()%top))/(top/2)-1;
			double distance_square=(x*x)+(y*y);
			if(distance_square <=1) 
			{
				#pragma omp critical
				local_sum++;
			}
		}
	}
	float pi_estimate = (4.0*local_sum)/(number_of_tosses);
	printf("Valor de pi(estimado): %f\n", pi_estimate);
	return 0;
}