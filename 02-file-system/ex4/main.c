#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please run cmd with syntax: \033[1;34m./program filename.\n");
        return -1;
    }

    char *filename = argv[1];
    int fd = open(filename, O_RDWR | O_CREAT, 0644);

    if (fd == -1) {
        perror("cannot open file");
        return -1;
    }

    char *buf = "hi brooooo";
    write(fd, buf, strlen(buf));
    close(fd);

    struct stat fileStat;

    if (stat(filename, &fileStat)) {
        perror("Stat error");
        return -1;
    }

    char *type_file;
    if (S_ISREG(fileStat.st_mode))
        type_file = "Regular File";
    else if (S_ISDIR(fileStat.st_mode))
        type_file = "Directory";
    else
        type_file = "Other";

    char *time_str = ctime(&fileStat.st_mtime);
    time_str[strcspn(time_str, "\n")] = '\0';

    printf("=============================================\n");
    printf("    \033[1;35mField\033[0m            |  \033[1;34mValue\033[0m\n");
    printf("    Type File        |  %s\n", type_file);
    printf("    File Name        |  %s\n", filename);
    printf("    Date Modified    |  %s\n", time_str);
    printf("    File Size        |  %ld bytes\n", fileStat.st_size);
    printf("=============================================\n");

    return 0;
}