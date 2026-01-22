#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *arr;
int M, N;
int chunk;

// thread function
void* work(void* arg) {

    int tid = *(int*)arg;   // thread id

    int start = tid * chunk;
    int end   = (tid == N - 1) ? M : start + chunk;

    for (int i = start; i < end; i++) {
        arr[i] = arr[i] * 2;   // simple operation
        printf("Thread %d processed index %d\n", tid, i);
    }

    return NULL;
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Usage: %s M N\n", argv[0]);
        return 0;
    }

    M = atoi(argv[1]);
    N = atoi(argv[2]);

    arr = malloc(M * sizeof(int));

    for (int i = 0; i < M; i++)
        arr[i] = i + 1;

    pthread_t threads[N];
    int tid[N];

    chunk = M / N;

    for (int i = 0; i < N; i++) {
        tid[i] = i;
        pthread_create(&threads[i], NULL, work, &tid[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    printf("\nFinal Array:\n");
    for (int i = 0; i < M; i++)
        printf("%d ", arr[i]);

    free(arr);
    return 0;
}
