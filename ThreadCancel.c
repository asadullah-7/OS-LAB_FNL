#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* worker(void* arg) {

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    while (1) {
        printf("Thread running...\n");
        sleep(1);

        // cancellation point
        pthread_testcancel();
    }

    return NULL;
}

int main() {

    pthread_t t;

    pthread_create(&t, NULL, worker, NULL);

    sleep(5);  // thread ko chalne do

    printf("Main: cancelling thread\n");
    pthread_cancel(t);

    pthread_join(t, NULL);

    printf("Thread cancelled successfully\n");
    return 0;
}
