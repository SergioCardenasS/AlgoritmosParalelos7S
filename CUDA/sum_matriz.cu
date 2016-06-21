#include <stdio.h>
#include <cuda.h>
#include <time.h>

__global__
void sum_mat(float* A,float* B,float* C,const int N,const int M)
{
    int id = blockDim.x * blockIdx.x + threadIdx.x;
    if (id < M*N)
        C[id] = A[id] + B[id];
}

__global__
void sum_mat_r(float* A,float* B,float* C,int N,int M)
{
    int id = blockDim.x * blockIdx.x + threadIdx.x;
    if(id<N)
    {
        int i;
        for(i = 0;i < M; ++i)
            C[M*id+i]=A[M*id+i]+B[M*id+i];
    }
}

__global__
void sum_mat_c(float* A,float* B,float* C,int N,int M)
{
    int id = blockDim.x * blockIdx.x + threadIdx.x;
    if (id < M)
    {
        int i;
        for(i=0;i<N;++i)
            C[M*i+id]=A[M*i+id]+B[M*i+id];
    }
}

// debuging functions
void init_array(float *a, const int N);
void init_mat(float *a, const int N, const int M);
void init_mat_c(float *a, const int N, const int M);
void print_array(float *a, const int N, char *d);
void print_mat(float *a, const int N, const int M, char *d);

int main (void) 
{
    srand( time(NULL) );

    float *a, *b, *c;
    float *dev_a, *dev_b, *dev_c;
    int N=4;
    int M=4;
    int size=N*M;
    int device_size=size*sizeof(float);
    a=(float*)malloc(device_size);
    b=(float*)malloc(device_size);
    c=(float*)malloc(device_size);
    init_mat(a, N, M);
    init_mat(b, N, M);
    init_mat_c(c, N, M);

    printf("<<<<<<<<<< initial data:\n");
    
    print_mat(a, N, M, "matrix A ");
    print_mat(b, N, M, "matrix B ");

    cudaMalloc((void**)&dev_a, device_size);
    cudaMalloc((void**)&dev_b, device_size);
    cudaMalloc((void**)&dev_c, device_size);

    cudaMemcpy(dev_a, a, device_size, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, device_size, cudaMemcpyHostToDevice);

    printf("\n\nRunning Kernel...\n\n");
    //sum_mat<<<N*M,M>>>(dev_a, dev_b, dev_c, N, M);
    sum_mat_c<<<N,N>>>(dev_a, dev_b, dev_c, N, M);
    //sum_mat_r<<<N*M,M>>>(dev_a, dev_b, dev_c, N, M);
    //printf("error code: %s\n",cudaGetErrorString(cudaGetLastError()));

    cudaMemcpy(c, dev_c, device_size, cudaMemcpyDeviceToHost);

    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    printf(">>>>>>>>>> final data:\n");
    print_mat(c,N, M, "Matriz final");
    return 0;
};

void init_mat(float *a, const int N, const int M) 
{
    int i, j;
    for(i=0; i<N; i++)
        for(j=0; j<M; j++)
            a[i*M+j] = rand() % 4 + 1;
}

void init_mat_c(float *a, const int N, const int M) 
{
    int i, j;
    for(i=0; i<N; i++)
        for(j=0; j<M; j++)
            a[i*M+j] = 0;
}
void print_mat(float *a, const int N, const int M, char *d) 
{
    int i, j;
    for(i=0; i<N; i++)
    {
	    printf("\n%s[%d]:", d, i);
    	for (j=0; j<M; j++)
            printf("\t%6.4f", a[i*M+j]);
    }
    printf("\n");
}