#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int main()
{
    int status, child_pid;
    int ret = fork();
    
    
    if (ret > 0)
    {
        printf("Parent (PID: %d)\n", getpid());
        child_pid = wait(&status);

        if (WIFEXITED(status)) {
            printf("Parent: child exited normally with status = %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Parent: child terminated by signal %d\n", WTERMSIG(status));
        }
    }
    else if (ret == 0)
    {
        printf("Child (PID: %d) - i will exit after 2 seconds\n", getpid());
        sleep(2);
        exit(0);
    }
    else
    {
        printf("fork failed!");
    }
    

    return 0;
}