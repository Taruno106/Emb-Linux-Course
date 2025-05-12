#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "./dgram_server"


int main(int argc, char *argv[])
{
    int server_fd;
    struct sockaddr_un servaddr, cliaddr;
    char buffer[1024];
    socklen_t len;

    // create socket
    if ((server_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        perror("create socket fail");
        exit(EXIT_FAILURE);
    }
    
    unlink(SERVER_PATH);

    // bind
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, SERVER_PATH);

    if (bind(server_fd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // receive
    len = sizeof(cliaddr);
    int n = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len);
    buffer[n] = '\0';
    printf("Receive from client: %s\n", buffer);

    // send
    const char *msg = "Hello from UNIX domain datagram server";
    sendto(server_fd, msg, strlen(msg), 0, (const struct sockaddr*)&cliaddr, len);

    // closing
    close(server_fd);
    unlink(SERVER_PATH);

    return 0;
}