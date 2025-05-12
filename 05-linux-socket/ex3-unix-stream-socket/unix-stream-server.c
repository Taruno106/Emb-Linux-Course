#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "./socket_stream"
#define BACKLOG 100

int main(int argc, char *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_un address;
    char buffer[1024] = { 0 };
    char* msg = "Hello from UNIX domain stream server";

    // creating socket
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("create socket failed");
        exit(EXIT_FAILURE);
    }

    unlink(SOCKET_PATH); // delete if file exist
    
    // bind
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

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
    if ((new_socket = accept(server_fd, NULL, NULL)) < 0)
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
    unlink(SOCKET_PATH);
    
    return 0;
}