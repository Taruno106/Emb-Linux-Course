#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080


int main(int argc, char *argv[])
{
    int server_fd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    socklen_t len;

    // create socket
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("create socket fail");
        exit(EXIT_FAILURE);
    }
    
    // bind
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

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
    const char *msg = "Hello from UDP server";
    sendto(server_fd, msg, strlen(msg), 0, (const struct sockaddr*)&cliaddr, len);

    // closing
    close(server_fd);

    return 0;
}