#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int count = 0;


void handle_alarm(int num) {
    printf("\nTimer: %d seconds\n", count);
}

int main(int argc, char* argv[])
{
    if (signal(SIGALRM, handle_alarm) == SIG_ERR)
    {
        printf("cannot handle signal");
        exit(EXIT_FAILURE);
    }
    while (1) {
        count++;
        alarm(1);
        pause();
        if (count == 10) break;
    };

    return 0;
}