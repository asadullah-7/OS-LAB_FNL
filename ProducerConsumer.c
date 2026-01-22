#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define SIZE 1
int buffer;


sem_t empty, full;
pthread_mutex_t lock;

void* producer(void* arg)
{
    int item = 1;

    sem_wait(&empty);
    pthread_mutex_lock(&lock);

    buffer = item;
    printf("Producer Produced %d\n",buffer);

    pthread_mutex_unlock(&lock);
    sem_post(&full);

    return NULL;
}

void* consumer(void* arg)
{
    sem_wait(&full);
    pthread_mutex_lock(&lock);

    printf("Consumer consumed %d\n",buffer);

    pthread_mutex_unlock(&lock);
    sem_post(&empty);

    return NULL;

}

int main()
{
    pthread_t p, c;

    sem_init(&empty,0,SIZE);
    sem_init(&full,0,0);
    pthread_mutex_init(&lock,NULL);

    pthread_create(&p,NULL,producer,NULL);
    pthread_create(&c,NULL,consumer,NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}