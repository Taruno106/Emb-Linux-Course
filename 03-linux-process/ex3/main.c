#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

void handle_signal(int signal)
{
    printf("Child received SIGUSR1! Exiting now...\n");
    exit(0);
}

int main()
{
    printf("Process ID: %d\n", getpid());

    int ret = fork();
    if (ret > 0)
    {
        printf("Parent (PID: %d) - i will send SIGUSR1 for my child after 5 seconds\n\n", getpid());
        sleep(5);
        kill(ret, SIGUSR1); // ret == child's pid

        wait(NULL); // wait child exit
        printf("Parent: child process exited\n");
    }
    else if (ret == 0)
    {
        // Register the signal handler
        signal(SIGUSR1, handle_signal);
        printf("Child (PID: %d) - i'm waiting SIGUSR1 from parent\n", getpid());
        while (1)
        {
            pause();
        }
    }
    else
    {
        printf("fork failed!");
        return 1;
    }

    return 0;
}