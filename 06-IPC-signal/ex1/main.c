#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int count = 0;


void handle_ctrl_c(int num) {
    printf("\nSIGINT received\n");
    count++;
    if (count == 3) exit(0);
}

int main(int argc, char* argv[])
{
    if (signal(SIGINT, handle_ctrl_c) == SIG_ERR)
    {
        printf("cannot handle signal");
        exit(EXIT_FAILURE);
    }

    while (1);

    return 0;
}

// answer
/*
    nếu bỏ qua tín hiệu SIGINT thì khi user nhấn ctrl + C thì nó sẽ 
    rơi vào trường hợp "the default action", tức là nó sẽ thực hiện tác vụ mặc định
    được định nghĩa cho signal SIGINT là TERMINATE chương trình đang chạy.
*/
