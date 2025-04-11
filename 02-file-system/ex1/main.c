#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd;
    int numb_write;
    char buf1[10] = "hello ";
    char buf2[10] = "linux1234\n";
    
    fd = open("content.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        printf("open() content.txt file failed\n");
        return 1;
    }

    numb_write = write(fd, buf1, strlen(buf1));
    printf("Write %d bytes to content.txt\n", numb_write);

    lseek(fd, 0, SEEK_SET);
    numb_write = write(fd, buf2, strlen(buf2));
    printf("Write additionally %d bytes to content.txt\n", numb_write);
    close(fd);

    return 0;
    /* dữ liệu sẽ xuất hiện ở cuối file vì:
        + Cờ O_APPEND khiến mọi thao tác ghi luôn ghi vào cuối file.
        + lseek() không có tác dụng trong việc thay đổi vị trí ghi 
        khi O_APPEND đang bật.
    */
}