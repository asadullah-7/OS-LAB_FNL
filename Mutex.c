#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

int count = 0;
pthread_mutex_t lock;

void* increment(void* arg)
{
    pthread_mutex_lock(&lock);
    ////////////// CRITICAL SECTION START //////////////////////
    count++;
    printf("Count = %d\n",count);
    ////////////// CRITICAL SECTION END //////////////////////

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(int argc, char* argv[])
{

    if(argc !=2 )
    {
        printf("Write the range %s\n",argv[0]);
        return 0;
    }

    int range = atoi(argv[1]);
    pthread_t t[range];
    pthread_mutex_init(&lock, NULL );

    for(int i = 0 ; i < range; i++)
    {
        pthread_create(&t[i], NULL, increment, NULL);
    }

    for(int i = 0 ; i < range; i++)
    {
        pthread_join(t[i],NULL);
    }

    pthread_mutex_destroy(&lock);

    return 0;


}

