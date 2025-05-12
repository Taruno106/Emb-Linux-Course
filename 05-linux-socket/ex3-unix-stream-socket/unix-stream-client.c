#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "./socket_stream"


int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_un serv_addr;
    char *msg = "Hello from UNIX domain stream client";
    char buffer[1024] = { 0 };

    // create socket
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("create socket failed");
        return -1;
    }

    /* connect */
    // make server address
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCKET_PATH);
    
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connection failed");
        return -1;
    }
    
    // write, read
    send(sock, msg, strlen(msg), 0);
    printf("sent message to server\n");
    read(sock, buffer, sizeof(buffer));
    printf("%s\n", buffer);

    // closing
    close(sock);

    return 0;
}