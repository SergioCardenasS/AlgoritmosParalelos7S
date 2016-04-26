#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>

//compilar: mpic++ ejercicio3.c -o ejercicio3.out 
//ejecutar: mpirun -np 5 ./ejercicio3.out 20

int main(int argc, char **argv)
{
    //Sacar el number_random del tamaño que ingresamos
    int n = atoi(argv[1]);
    int cont=1;
    int rec;

    MPI_Init(NULL, NULL);
    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    srand(time(NULL)+my_rank);

    int number_random = rand() % n;

    printf("Threads %d = %d\n", my_rank, number_random);
    MPI_Barrier(MPI_COMM_WORLD);
    
    while(cont<comm_sz)
    {
        if(my_rank%(cont<<1)==0)
        {
            if((my_rank+cont<comm_sz))
            {
                MPI_Recv(&rec, 1, MPI_INT, my_rank+cont,cont, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                number_random+=rec;
            }
        }
        else if(my_rank%(cont<<1)==cont)
        {
            rec=number_random;
            MPI_Send(&rec, 1, MPI_INT, my_rank-cont, cont,MPI_COMM_WORLD);
        }
        else
            break;
        cont=cont<<1;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(my_rank==0)
    {
        rec=number_random;
        for(unsigned int i=1;i<comm_sz;++i)
            MPI_Send(&rec, 1, MPI_INT, i, 0,MPI_COMM_WORLD);
        printf("global sum: %d\n",number_random);
    }
    else
    {
        MPI_Recv(&rec, 1, MPI_INT,0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        number_random=rec;
    }
    MPI_Finalize();
    return 0;
}
