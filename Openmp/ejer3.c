#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>


//Compilar: gcc -fopenmp -o ejer3 ejer3.c
//Ejecular: ./ejer3

int main(int argc, char **argv)
{


}


void Count sort (int a [], int n ) 
{
	int i , j , count;
	int∗ temp = malloc (n∗sizeof(int));
	for ( i = 0; i < n ; i ++) 
	{
		count = 0;
		for ( j = 0; j < n ; j ++)
			if ( a[j] < a[i])
				count++;
			else if (a[j] == a[i] && j< i)
				count++;
		temp[count] = a[i];
	}
	memcpy (a,temp,n∗sizeof(int));
	free (temp);
} /∗ Count sort ∗/