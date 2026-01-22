#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define WRITE  1
#define READ  0

void bubbleSort(int array[], int size)
{
    for(int i = 0 ; i < size; i++)
    {
        for(int j = 0 ; j < size - i - 1; j++)
        {
            if(array[j] > array[j + 1])
            {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j+1] = temp;
            }
        }
    }
}

int main()
{
    int pipe1[2];
    int pipe2[2];
    int arr[] = {2,3,5,1,4};
    int n = sizeof(arr)/sizeof(arr[0]);

    if(pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        printf("Error pipe failed");
        exit(1);
    }

    pid_t id = fork();
    if(id < 0)
    {
        printf("Fork failed");
        exit(1);
    }
    if (id > 0)
    {
        close(pipe1[READ]);
        close(pipe2[WRITE]);

        write(pipe1[WRITE], arr, sizeof(arr));
        close(pipe1[WRITE]);

        int sortArr[5];
        read(pipe2[READ], sortArr, sizeof(sortArr));
        close(pipe2[READ]);

        printf("Sorted Array: ");
        for(int i = 0 ; i < n; i++)
        {
            printf("%d, ",sortArr[i]);

        }
        printf("\n");

    }

    else if (id == 0)
    {
        close(pipe1[WRITE]);
        close(pipe2[READ]);

        int recivedArr[5];
        read(pipe1[READ], recivedArr, sizeof(recivedArr));
        close(pipe1[READ]);

        bubbleSort(recivedArr, n);

        write(pipe2[WRITE], recivedArr, sizeof(recivedArr));
        close(pipe2[WRITE]);

        exit(0);
    }

    return 0;

}
