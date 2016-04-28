#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


#define TAM 30
void *print_message_function( void *ptr );

main()
{
	int inicio,fin;
	double av=0;
	pthread_t lista[TAM];
	for(int i=0;i<TAM;i++)
	{
		inicio = clock();
		pthread_create( &lista[i], NULL, &print_message_function, NULL);
		fin = clock();
		av = av+(double)(fin-inicio)/ CLOCKS_PER_SEC;
	}
	av = av/TAM;
	printf("%f/n",av);
     exit(EXIT_SUCCESS);
}

void *print_message_function(void *ptr)
{
	pthread_exit(NULL);	
}