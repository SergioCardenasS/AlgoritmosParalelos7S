#include <iostream>
#include <random>
#include <time.h>

using namespace std;

#define MAX 1000
double A[MAX][MAX];
double x[MAX];
double y[MAX];

int main()
{
    int random = 30;
    for (int i=0; i<MAX;i++)
    {
        x[i] = 0;
        y[i] = rand() % random;
        for (int j=0; j< MAX; j++)
            A[i][j] = rand() % random;
    }

    int inicio, fin;
    inicio = clock();

    for (int i=0; i<MAX;i++)
    {
        for (int j=0; j< MAX; j++)
            y[i]+= A[i][j]*x[j];
    }

    fin = clock();
    cout<<(double)(fin - inicio) / CLOCKS_PER_SEC<<endl;

    inicio = clock();
    for (int j = 0; j<MAX; j++)
    {
        for (int i =0; i<MAX; i++)
            y[i]+=A[i][j]*x[j];
    }
    fin = clock();
    cout<<(double)(fin - inicio) / CLOCKS_PER_SEC<<endl;
    return 0;
}