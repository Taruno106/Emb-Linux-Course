#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080


int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char *msg = "Hello from client";
    char buffer[1024] = { 0 };

    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("create socket failed");
        return -1;
    }

    /* connect */
    // make server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //convert string to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
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