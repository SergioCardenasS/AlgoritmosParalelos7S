#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

//compilar: mpic++ ejercicio3.c -o ejercicio3.out 
//ejecutar: mpirun -np 10 ./ejercicio3.out 50

unsigned bitmask = (unsigned) 1;
int Global_total(int cont, int my_rank, int comm_sz, MPI_Comm comm);

int main(int argc, char **argv)
{
    
    MPI_Init(NULL, NULL);
    int limit = atof(argv[1]);
    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm comm = MPI_COMM_WORLD;;
    int cont;
    int total;

    srandom(my_rank+1);
    cont = random() % limit;
    printf("thread %d = %d\n", my_rank, cont);

    total = Global_total(cont, my_rank, comm_sz, comm);

    if (!my_rank)
        printf("Total: %d\n", total);
    MPI_Finalize();
    return 0;
}

int Global_total(int cont, int my_rank, int comm_sz, MPI_Comm comm) 
{
    int total = cont;
    int tmp,partner;
    int done = 0;
    
    while (bitmask < comm_sz) 
    {
        partner = my_rank^bitmask;
        if (my_rank < partner) 
        {
            if (partner < comm_sz) 
            {
                MPI_Recv(&tmp, 1, MPI_INT, partner, 0, comm, MPI_STATUS_IGNORE);
                total += tmp;
            }
            bitmask <<= 1;
        } 
        else {MPI_Send(&total, 1, MPI_INT, partner, 0, comm); break;}
    }
    return total;
}  