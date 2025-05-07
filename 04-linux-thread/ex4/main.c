#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100

int arr[100];
int num_even = 0;
int num_odd = 0;

int random_in_range(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void create_arr()
{
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
    {
        arr[i] = random_in_range(1, 100);
    }
    printf("Created an array with 100 elements\n");
}

void* count_even(void* arg)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (arr[i] % 2 == 0)
        {
            num_even++;
        }
    }

    pthread_exit(NULL);
}

void* count_odd(void* arg)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (arr[i] % 2 != 0)
        {
            num_odd++;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    printf("Main thread - TID: %ld\n", pthread_self());
    create_arr();

    // printf("Array = ");
    // for (int i = 0; i < SIZE; i++)
    // {
    //     printf("%i\t", arr[i]);
    // }
    pthread_t th_even, th_odd;

    pthread_create(&th_even, NULL, count_even, NULL);
    pthread_create(&th_odd, NULL, count_odd, NULL);

    pthread_join(th_even, NULL);
    pthread_join(th_odd, NULL);
    
    printf("the number of even: %d\n", num_even);
    printf("the number of odd: %d\n", num_odd);

    return 0;
}