#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BACKLOG 100

int main(int argc, char *argv[])
{
    int server_fd, new_socket;
    int opt = 1;
    struct sockaddr_in address;
    char buffer[1024] = { 0 };
    char* msg = "Hello from NETWORK domain stream server";

    // creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create socket failed");
        exit(EXIT_FAILURE);
    }
    
    // prevent error
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    // bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // inet_pton()
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }
    
    // listening
    if (listen(server_fd, BACKLOG) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    
    // accept
    socklen_t addrlen = sizeof(address);
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    
    // write, read
    read(new_socket, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    send(new_socket, msg, strlen(msg), 0);
    printf("sent message to client\n");

    // closing
    close(new_socket); // close the connected socket
    close(server_fd); // close the listening socket


    return 0;
}