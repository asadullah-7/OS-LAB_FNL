#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define READERS 3
#define WRITERS 1

int readcount = 0;
pthread_mutex_t mutex;
sem_t rw;

void* reader(void* arg)
{
    int id = *(int*)arg;

    pthread_mutex_lock(&mutex);
    readcount++;
    if(readcount == 1)
        sem_wait(&rw);  // first reader locks writer
    pthread_mutex_unlock(&mutex);

    printf("Reader %d is reading\n", id);
    sleep(1);

    pthread_mutex_lock(&mutex);
    readcount--;
    if(readcount == 0)
        sem_post(&rw); // last reader releases writer
    pthread_mutex_unlock(&mutex);

    printf("Reader %d finished reading\n", id);
    return NULL;
}

void* writer(void* arg)
{
    int id = *(int*)arg;

    sem_wait(&rw);
    printf("Writer %d is writing\n", id);
    sleep(2);
    printf("Writer %d finished writing\n", id);
    sem_post(&rw);

    return NULL;
}

int main()
{
    pthread_t r[READERS], w[WRITERS];
    int idsR[READERS], idsW[WRITERS];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&rw, 0, 1);

    for(int i=0;i<READERS;i++) { idsR[i]=i+1; pthread_create(&r[i],NULL,reader,&idsR[i]); }
    for(int i=0;i<WRITERS;i++) { idsW[i]=i+1; pthread_create(&w[i],NULL,writer,&idsW[i]); }

    for(int i=0;i<READERS;i++) pthread_join(r[i],NULL);
    for(int i=0;i<WRITERS;i++) pthread_join(w[i],NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&rw);

    return 0;
}
