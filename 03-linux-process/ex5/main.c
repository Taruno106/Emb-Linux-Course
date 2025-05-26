#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int main()
{
    int ret = fork();
    
    if (ret < 0) {
        perror("Fork thất bại");
        return 1;
    }

    if (ret == 0)
    {
        printf("[CHILD] PID: %d, parent: %d\n", getpid(), getppid());

        // Tạo orphan: ngủ lâu hơn tiến trình cha
        sleep(5);
        printf("[CHILD] Sau sleep: PID: %d, PPID: %d\n", getpid(), getppid());
    }
    else
    {
        printf("[PARENT] PID: %d, CHILD PID: %d\n", getpid(), ret);

        // Tạo zombie: không gọi wait(), thoát ngay
        sleep(1);
        printf("[PARENT] Kết thúc\n");
        exit(0);
    }
    
    return 0;
}