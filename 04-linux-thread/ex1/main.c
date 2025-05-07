#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t tid1, tid2;

void* thread_handler1(void* arg)
{
    printf("Hi, i'm thread 1 (TID: %ld)\n", pthread_self());
    sleep(1);
    pthread_exit(NULL);
}

void* thread_handler2(void* arg)
{
    printf("Hi, i'm thread 2 (TID: %ld)\n", pthread_self());
    sleep(1);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    printf("Main thread - PID: %ld\n\n", pthread_self());
    
    pthread_create(&tid1, NULL, &thread_handler1, NULL);
    pthread_create(&tid2, NULL, &thread_handler2, NULL);

    printf("aaaaaa\n");

    pthread_join(tid1, NULL); // wait tid1 exit, then run pthread_join(tid2, NULL)
    pthread_join(tid2, NULL); // wait tid2 exit, then run printf()

    printf("Main thread finish after tid1, tid2 exited!\n");

    return 0;
}