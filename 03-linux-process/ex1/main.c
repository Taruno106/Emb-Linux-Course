#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("I'm main program, shell's pid: %d\n", getppid());
    int pid = fork();

    if (pid > 0) {
        // parent
        printf("\n");
        printf("I'm the parent process, my PID: %d - my children's PID: %d\n", getpid(), pid);
        printf("\n");
    } else if (pid == 0) {
        // children
        printf("I'm the children process, my PID: %d - my parent's PID: %d\n", getpid(), getppid());
    } else {
        // fail to create children process
        printf("fail to create children process\n");
    }

    return 0;
}