#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please run with syntax: ./App content\n");
        return 1;
    }

    int fd;
    char *content = argv[1];
    char *file_name = "/dev/m_character_device";

    if ((fd = open(file_name, O_RDWR)) < 0) {
        printf("Cannot open file %s\n", file_name);
        return 1;
    }

    ssize_t bytes_written = write(fd, content, strlen(content));
    if (bytes_written < 0) {
        perror("Write failed");
        close(fd);
        return 1;
    }

    printf("Wrote to device: %s\n", content);

    close(fd);

    return 0;
}