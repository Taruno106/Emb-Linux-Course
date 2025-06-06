#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int count = 0;

void handle_child(int num) {
    printf("\n[%d] CHILD: Received signal from parent.\n", count);
    if (count == 5) exit(0);
}

int main(int argc, char *argv[]) {
    int pid = fork();

    if (pid < 0) {
        printf("cannot fork");
        return 1;
    }

    if (pid > 0) {
        while (count < 5)
        {
            sleep(2);
            count++;
            printf("[%d] PARENT: emit signal after 2 seconds\n", count);
            kill(pid, SIGUSR1);
            if (count == 5) exit(0);
        }
    } else {
        if (signal(SIGUSR1, handle_child) == SIG_ERR) {
            printf("cannot handle signal");
            exit(EXIT_FAILURE);
        }
        while (1) {
            pause();
        }
    }

    return 0;
}
