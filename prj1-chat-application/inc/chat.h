#ifndef CHAT_H
#define CHAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CONNECTION 100

// properties
typedef struct {
    int id;
    char ip[INET_ADDRSTRLEN];
    int port;
    int sockfd;
} connection_t;

extern connection_t conn_queue[MAX_CONNECTION];
extern int conn_count;
extern pthread_mutex_t lock;

// methods
void create_client(connection_t* conn);
void create_server(const char* port);

#endif