#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

int main(int argc, char*argv[])
{
    if(argc != 3)
    {
        printf("Usage, 1,2%s\n",argv[0]);
        return 1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    printf("%d x %d = %d\n",a,b,a*b);

    return 0;
}