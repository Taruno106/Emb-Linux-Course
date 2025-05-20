

#include "../inc/chat.h"

// #define PORT 8080

connection_t conn_queue[MAX_CONNECTION];
int conn_count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;



void create_client(connection_t *conn)
{
    char server_ip[INET_ADDRSTRLEN];
    int server_port = conn->port;
    int sock = conn->sockfd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    strcpy(server_ip, conn->ip);
    // printf("--> create_client: server_ip: %s - port: %d\n", server_ip, server_port);

    /* connect */
    // make server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    // convert string to binary
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0)
    {
        perror("\nClient: Invalid address/ Address not supported \n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Client: connection failed");
        return;
    }
    
    // add a new connection to conn_queue if connect success
    pthread_mutex_lock(&lock);
    conn_queue[conn_count++] = *conn;
    pthread_mutex_unlock(&lock);

    while (1)
    {
        int n = read(conn->sockfd, buffer, sizeof(buffer));

        if (n > 0)
        {
            buffer[n] = '\0';
            printf("Message from %s at port %d: %s\n", server_ip, server_port, buffer);
        }
    }
}

void create_server(const char *port)
{
    printf("Application is listening on port %s\n", port);

    int server_fd, new_socket;
    int opt = 1;
    struct sockaddr_in address;
    char buffer[1024] = {0};

    // creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("create socket failed");
        exit(EXIT_FAILURE);
    }

    // prevent error
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // inet_pton()
    address.sin_port = htons(atoi(port));

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }

    // listening
    if (listen(server_fd, MAX_CONNECTION) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // accept
    socklen_t addrlen = sizeof(address);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    //
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(address.sin_port);
    printf("Accepted a new connection from address: %s, setup at port: %d\n", client_ip, client_port);

    // conn_queue.append
    pthread_mutex_lock(&lock);
    connection_t *conn = malloc(sizeof(connection_t));
    strcpy(conn->ip, client_ip);
    conn->port = client_port;
    conn->sockfd = new_socket;
    conn->id = conn_count + 1;
    conn_queue[conn_count++] = *conn;
    free(conn);
    pthread_mutex_unlock(&lock);

    while (1)
    {
        int n = read(new_socket, buffer, sizeof(buffer));

        if (n > 0)
        {
            buffer[n] = '\0';
            printf("Message from %s at port %d: %s\n", client_ip, client_port, buffer);
        }
    }
}
