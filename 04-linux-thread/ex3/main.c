#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int data = 0;
int ready = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_data = PTHREAD_COND_INITIALIZER;

int random_in_range(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void *th_producer(void *arg)
{
    printf("\nHi, i'm Producer (TID: %ld)\n", pthread_self());

    pthread_mutex_lock(&lock);
    // sleep(5);
    srand(time(NULL));

    data = random_in_range(1, 10);
    // ready = 1;
    printf("Producer: data is ready!\n");

    pthread_cond_signal(&cond_data);
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

void *th_consumer(void *arg)
{
    printf("\nHi, i'm Consumer (TID: %ld)\n", pthread_self());

    pthread_mutex_lock(&lock);
    while (1)
    {
        printf("Consumer: waiting for data...\n");
        pthread_cond_wait(&cond_data, &lock); // unlock -> wait -> wakeup -> lock
        printf("Consumer: got data\n");
        break;
    }
    printf("\nConsumer: data = %d\n", data);
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    printf("Main thread - PID: %ld\n", pthread_self());

    pthread_t prod_tid, cons_tid;
    pthread_create(&cons_tid, NULL, th_consumer, NULL);
    pthread_create(&prod_tid, NULL, th_producer, NULL);

    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond_data);

    return 0;
}