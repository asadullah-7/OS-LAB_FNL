// CAR PARKING PROBLEMS
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define CARS 10
#define SLOTS 3

sem_t sema;

void* car(void* arg)
{
    int id = *(int*)arg;

    printf("Car %d wants to park\n",id);
    sem_wait(&sema);

    printf("Car %d has been parked\n",id);
    sleep(1);

    printf("Car %d leaving!\n",id);
    sem_post(&sema);

    return NULL;
}

int main()
{
    pthread_t cars[CARS];
    int id[CARS];

    sem_init(&sema, 0, SLOTS);

    for(int i = 0; i < CARS; i++)
    {
        id[i] = i + 1;
        pthread_create(&cars[i],NULL,car,&id[i]);
    }

    for(int i = 0 ; i < CARS; i++)
    {
        pthread_join(cars[i],NULL);
    }

    sem_destroy(&sema);

    return 0;
}
