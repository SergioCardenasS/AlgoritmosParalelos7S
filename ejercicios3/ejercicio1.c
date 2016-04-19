#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>


//compilar: mpic++ ejercicio1.c -o ejercicio1.out
//ejecutar: mpirun -np 4 ./ejercicio1.out 1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3, 4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9


double max = -999999999;
double min = 999999999;
int bins = 5;

int FindBin(double data, double* bin_maxes, int bins);

int main(int argc, char **argv) 
{
	int n = argc-1;
	int comm_sz, my_rank,bin, i, j;
    double bin_width;
    double* send;
    double* bin_maxes;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    if (my_rank == 0) 
    {    
        
        double data[n];
        int n_maxes = bins+1;
        bin_maxes = new double[bins+1];
        send = new double[n/bins+1];

        int* local_bin_counts= new int[bins];
        local_bin_counts = (int*) malloc((bins)*sizeof(int));
        for(j = 0; j < bins; ++j)
                local_bin_counts[j] = 0;

      
        int* bin_counts = new int[bins];
        bin_counts = (int*) malloc((bins)*sizeof(int));
        for(j = 0; j < bins; ++j)
                bin_counts[j] = 0;

        for (i=0; i<n; i++)
        {
            data[i] = atof(argv[i+1]);
            if (data[i]>max)
                max = data[i];
            if (data[i]<min)
                min = data[i];
        }

        bin_width = (max - min) / bins;
        for (i=0; i<bins+1;i++)
            bin_maxes[i] = min + bin_width*(i);

        for(i = 1; i < comm_sz; ++i) 
        {
            for (j = 0; j < n/(comm_sz-1); j++) 
                send[j] = data[((i-1)*n/(comm_sz-1))+j];

            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&bins, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&bin_width, 1, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
            MPI_Send(bin_maxes, n_maxes, MPI_DOUBLE, i, 3, MPI_COMM_WORLD);
            MPI_Send(send, n/bins, MPI_DOUBLE, i, 4, MPI_COMM_WORLD);
        }

        for(i = 1; i < comm_sz; ++i)
        {
            MPI_Recv(local_bin_counts, bins, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for(j = 0; j < bins; ++j)
                bin_counts[j] += local_bin_counts[j];
        }
        for (i=0;i<bins;i++)
            printf("subinter vals %d : %d vals \n", i, bin_counts[i]);
    }
    else 
    {
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&bins, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&bin_width, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  

        bin_maxes = (double *) malloc((bins+1)*sizeof(double));
        int n_data = n/bins+1,n_maxes=bins+1;

        MPI_Recv(bin_maxes, n_maxes, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        send = new double[n/bins+1];
        MPI_Recv(send, n_data, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int* local_bin_counts= new int[bins];
        local_bin_counts = (int *) malloc((bins)*sizeof(int));
        for(j = 0; j < bins; ++j)
                local_bin_counts[j] = 0;

        for (i=0;i<n_data;i++)
        {
            bin = FindBin(send[i],bin_maxes,n_maxes);
            local_bin_counts[bin]++;
        }
        MPI_Send(local_bin_counts, bins, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize(); 

    return 0;
}

int FindBin(double data, double* bin_maxes, int bins)
{
    for (int i=0;i<bins-1;i++)
    {
        if ((bin_maxes[i] <= data) & (data <= bin_maxes[i+1]))
            return i;
    }
}