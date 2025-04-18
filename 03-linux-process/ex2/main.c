#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // export MODE=1 && ./main
    char *env_mode = getenv("MODE");
    if (env_mode == NULL) {
        printf("Environment variable MODE not found!\n");
        return 1;
    }

    int mode = atoi(env_mode);
    if (mode != 1 && mode != 2) {
        printf("MODE must be 1 or 2!\n");
        return 1;
    }

    int pid = fork();

    if (pid < 0)
    {
        printf("fork failed\n");
        return 1;
    }

    if (pid == 0)
    {
        // children
        if (mode == 1)
        {
            printf("children: PID = %d, replaced by 'ls -l'\n", getpid());
            execlp("ls", "ls", "-l", NULL);
        }
        else if (mode == 2)
        {
            printf("children: PID = %d, replaced by 'date'\n", getpid());
            execlp("date", "date", NULL);
        }

        // if exec failed
        perror("exec failed");
        return 1;
    }
    else
    {
        // parent
        printf("parent: PID = %d, wait children process (PID = %d)...\n\n", getpid(), pid);
        wait(NULL);
        printf("\nparent: children process finished\n");
    }

    /* explain sentence: "exec() will replace current process by new process"
    that mean: if exec() success, "ls -l" will run and line "printf("After exec...)"
    will not run */
    // printf("Before exec\n");
    // execlp("ls", "ls", "-l", NULL);
    // printf("After exec (this line will NOT run if exec succeeds)\n");

    return 0;
}