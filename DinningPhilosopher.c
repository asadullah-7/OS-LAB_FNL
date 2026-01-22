#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define N 5

sem_t forks[N];

void* philosopher(void* arg)
{
    int id = *(int*)arg;
    int left = id;
    int right = (id+1)%N;

    printf("Philosopher %d is thinking\n",id);

    sem_wait(&forks[left]);
    sem_wait(&forks[right]);

    printf("Philosopher %d is eating\n", id);
    sleep(1);

    sem_post(&forks[right]);
    sem_post(&forks[left]);

    printf("Philosopher %d finished eating\n",id);
    return NULL;

}

int main()
{
    pthread_t ph[N];
    int ids[N];

    for(int i = 0 ;  i < N; i++)
    {
        sem_init(&forks[i],0,1);
    }
    for(int i = 0 ;  i < N; i++)
    {
        ids[i] = i;
        pthread_create(&ph[i],NULL,philosopher,&ids[i]);
    }

    for(int i = 0 ;  i < N; i++)
    {
        pthread_join(ph[i],NULL);
    }

    for(int i = 0 ;  i < N; i++)
    {
        sem_destroy(&forks[i]);
    }

    return 0;
}