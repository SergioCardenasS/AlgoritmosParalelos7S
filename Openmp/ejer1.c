#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//compilar : gcc -fopenmp -o ejer1 ejer1.c
//ejercutar : ./ejer1

int histogram(void);

int bin = 6;
int tam = 21;
int histo[6];
float lista[21] = {1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3, 4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9,5.3};

int main(int argc, char** argv) 
{
	int thread_count = 6;
	int n,i;
	#pragma omp parallel num_threads(thread_count)
	histogram();
	for(i=0;i<bin;i++)
	{
		printf("%d\n",histo[i]);
	}
	return 0;
}

int histogram(void) 
{
	int my_rank = omp_get_thread_num();
	int thread_count = omp_get_num_threads();
	int num = my_rank+1;
	int i;
	int cont = 0;
	for(i = 0;i<tam;++i)
	{
		if(lista[i]<num && lista[i]>=my_rank)
			cont++;
	}
	histo[my_rank] = cont;
}