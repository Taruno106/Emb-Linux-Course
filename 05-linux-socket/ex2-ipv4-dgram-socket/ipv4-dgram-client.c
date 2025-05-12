#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080


int main(int argc, char *argv[])
{
    int client_fd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    socklen_t len;

    // create socket
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("create socket fail");
        exit(EXIT_FAILURE);
    }
    
    // bind
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    
    // send
    len = sizeof(servaddr);
    const char *msg = "Hello from UDP client";
    sendto(client_fd, msg, strlen(msg), 0, (const struct sockaddr*)&servaddr, len);
    
    // receive 
    int n = recvfrom(client_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&servaddr, &len);
    buffer[n] = '\0';
    printf("Receive from server: %s\n", buffer);

    // closing
    close(client_fd);

    return 0;
}