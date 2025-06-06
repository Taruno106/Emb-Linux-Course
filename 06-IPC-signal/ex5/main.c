#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t running = 1;

void sig_handler(int sig) {
    if (sig == SIGINT) {
        printf("\nSIGINT received.\n");
    } else if (sig == SIGTERM) {
        printf("\nSIGTERM received. Exiting...\n");
        running = 0;
    }
}

void *input_thread(void *arg) {
    char buffer[128];
    char *act;
    while (running) {
        printf("Enter your cmd: ");
        fflush(stdout);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            buffer[strcspn(buffer, "\n")] = 0; // remove newline
            printf("You entered: %s\n", buffer);

            act = strtok(buffer, " ");
            if (act && strcmp(act, "kill") == 0) {
                char *sig = strtok(NULL, " ");
                char *pid = strtok(NULL, " ");
                if (strcmp(sig, "-2") == 0 || strcmp(sig, "-SIGINT") == 0) {
                    kill(atoi(pid), SIGINT);
                } else if (strcmp(sig, "-15") == 0 || strcmp(sig, "-SIGTERM") == 0) {
                    kill(atoi(pid), SIGTERM);
                }
            }
        }
    }

    return NULL;
}

int main() {
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    printf("PID: %d\n", getpid());

    pthread_t tid;
    pthread_create(&tid, NULL, input_thread, NULL);

    while (running) {
        pause();
    }

    pthread_cancel(tid); // kill input thread
    pthread_join(tid, NULL);

    return 0;
}
