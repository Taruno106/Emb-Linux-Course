#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_ctrl_z(int num) { 
    printf("\nhandle_ctrl_z: SIGTSTP ignored.\n"); 
}

int main(int argc, char *argv[]) {
    if (signal(SIGTSTP, handle_ctrl_z) == SIG_ERR) {
        printf("cannot handle signal");
        exit(EXIT_FAILURE);
    }
    while(1);

    return 0;
}



// Q: Điều gì xảy ra nếu không xử lý tín hiệu SIGTSTP và người dùng nhấn Ctrl+Z?
/*
A:
nếu k xử lý tín hiệu SIGTSTP thì nó sẽ thực hiện action default (thoát chương trình).
còn nếu nếu handle nó thì nó sẽ rơi vào case catch and handle signal (do dev tự định nghĩa).

*/