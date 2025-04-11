#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //./example_program filename num-bytes [r/w] [index/"Hello"]
    if (argc != 5) {
        printf("the number of arguments must equal 5\n");
        return 1;    
    }

    char *file_name = argv[1];
    // char file_name[100];
    // strcpy(file_name, argv[1]);
    int num_bytes = atoi(argv[2]);
    char *mode = argv[3];
    int fd;

    fd = open(file_name, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        printf("cannot open %s file\n", file_name);
        return 1;
    }
    
    if (strcmp(mode, "r") == 0) {
        int index = atoi(argv[4]);
        char buf_read[num_bytes + 1]; // cap phat them 1 byte de them '\0'

        lseek(fd, index, SEEK_SET);
        ssize_t numb_read = read(fd, buf_read, num_bytes);
        buf_read[numb_read] = '\0';
        printf("read %d bytes of file: %s\n", num_bytes, buf_read);
    } else if (strcmp(mode, "w") == 0) {
        char *content = argv[4];
        write(fd, content, num_bytes);
        printf("write <%s> (%d bytes) to %s file\n", content, num_bytes, file_name);
    }
    
    close(fd);

    return 0;
}