#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define CLIENT_PATH "./dgram_client"
#define SERVER_PATH "./dgram_server"


int main(int argc, char *argv[])
{
    int client_fd;
    struct sockaddr_un cliaddr, servaddr;
    char buffer[1024];
    socklen_t len;

    // create socket
    if ((client_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        perror("create socket fail");
        exit(EXIT_FAILURE);
    }

    unlink(CLIENT_PATH);

    cliaddr.sun_family = AF_UNIX;
    strcpy(cliaddr.sun_path, CLIENT_PATH);
    bind(client_fd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    
    // bind
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, SERVER_PATH);
    
    // send
    len = sizeof(servaddr);
    const char *msg = "Hello from UNIX domain datagram client";
    sendto(client_fd, msg, strlen(msg), 0, (const struct sockaddr*)&servaddr, len);
    
    // receive 
    int n = recvfrom(client_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&servaddr, &len);
    buffer[n] = '\0';
    printf("Receive from server: %s\n", buffer);

    // closing
    close(client_fd);
    unlink(CLIENT_PATH);

    return 0;
}