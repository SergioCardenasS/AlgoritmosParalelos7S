#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_cond_t wait  = PTHREAD_COND_INITIALIZER;
pthread_cond_t start  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int tasks;

void *task()
{
    pthread_mutex_unlock(&mutex);
    for(int i = 1; i <= 4; i++)
    {
        printf("Trabajando....%d\n", i);
        pthread_cond_signal(&start);
    }
    pthread_mutex_lock(&mutex);
}

void *trabajo()
{
    pthread_mutex_lock(&mutex);
    while( pthread_cond_wait(&start, &mutex) != 0 );
    pthread_mutex_unlock(&mutex);
    printf("Fin del trabajo....\n");
    pthread_cond_signal(&wait);
}

int main(int argc, char *argv[])
{
    int nthreads=atof(argv[1]);
    pthread_t threads[nthreads];
    pthread_create(&threads[0], NULL, task, (void *)0);
    for (long i = 1; i < nthreads; ++i)
        pthread_create(&threads[i], NULL, trabajo, (void *)i);
    for (long i = 1; i < nthreads; ++i)
        pthread_join(threads[i],NULL);
    pthread_exit(NULL);
}