

#include "../inc/chat.h"

connection_t conn_queue[MAX_CONNECTION];
int conn_count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int next_id = 1;

void create_client(connection_t *conn)
{
    char server_ip[INET_ADDRSTRLEN];
    int server_port = conn->port;
    int sock = conn->sockfd;
    int id = conn->id;
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
            printf("\n\nMessage from \033[1;34m%s\033[0m at port \033[1;35m%d\033[0m (id: %d): %s\n\n", server_ip, server_port, id, buffer);
        }
        else if (n == 0)
        {
            printf("\nClient: The peer at port \033[1;35m%d\033[0m (id: %d) has \033[1;31mdisconnected\033[0m.\n",
                   server_port, id);

            int found = 0;
            pthread_mutex_lock(&lock);
            for (int i = 0; i < conn_count; i++)
            {
                if (id == conn_queue[i].id)
                {
                    close(conn_queue[i].sockfd);

                    // delete the found element of conn_queue and move these rest elements to left
                    for (int j = i; j < conn_count - 1; j++)
                    {
                        conn_queue[j] = conn_queue[j + 1];
                    }
                    conn_count--;
                    found = 1;
                    break;
                }
            }
            pthread_mutex_unlock(&lock);

            if (!found)
            {
                printf("Connection with sockfd %d not found.\n", id);
            }
        }
        else
        {
            perror("\nClient: read error\n");
            break;
        }
    }

    free(conn);
}

void create_server(const char *port)
{
    printf("Application is listening on port %s\n", port);

    int server_fd, new_socket, max_fd;
    int opt = 1;
    struct sockaddr_in address;
    fd_set master_set, read_fds;
    char buffer[1024];

    // creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("create socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(port));

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CONNECTION) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&master_set);
    FD_SET(server_fd, &master_set);
    max_fd = server_fd;

    while (1)
    {
        read_fds = master_set;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
        {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i <= max_fd; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == server_fd)
                {
                    // New connection
                    struct sockaddr_in client_addr;
                    socklen_t addrlen = sizeof(client_addr);
                    new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
                    if (new_socket < 0)
                    {
                        perror("accept failed");
                        continue;
                    }

                    FD_SET(new_socket, &master_set);
                    if (new_socket > max_fd)
                        max_fd = new_socket;

                    // Add to conn_queue
                    char client_ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
                    int client_port = ntohs(client_addr.sin_port);

                    pthread_mutex_lock(&lock);
                    if (conn_count < MAX_CONNECTION)
                    {
                        connection_t conn;
                        strcpy(conn.ip, client_ip);
                        conn.port = client_port;
                        conn.sockfd = new_socket;
                        conn.id = conn_count + 1;

                        conn_queue[conn_count++] = conn;

                        printf("\nAccepted connection from \033[1;34m%s\033[0m:\033[1;35m%d\033[0m\n",
                               conn.ip, conn.port, conn.sockfd, conn.id);
                    }
                    else
                    {
                        printf("Max connections reached. Rejecting %s:%d\n", client_ip, client_port);
                        close(new_socket);
                    }
                    pthread_mutex_unlock(&lock);
                }
                else
                {
                    // Existing client sent data
                    int n = read(i, buffer, sizeof(buffer));
                    if (n > 0)
                    {
                        buffer[n] = '\0';

                        pthread_mutex_lock(&lock);
                        for (int j = 0; j < conn_count; j++)
                        {
                            if (conn_queue[j].sockfd == i)
                            {
                                printf("\n\nMessage from \033[1;34m%s\033[0m at port \033[1;35m%d\033[0m (id: %d): %s\n\n",
                                       conn_queue[j].ip, conn_queue[j].port, conn_queue[j].id, buffer);
                                break;
                            }
                        }
                        pthread_mutex_unlock(&lock);
                    }
                    else if (n == 0)
                    {
                        // Client disconnected
                        close(i);
                        FD_CLR(i, &master_set);

                        pthread_mutex_lock(&lock);
                        for (int j = 0; j < conn_count; j++)
                        {
                            if (conn_queue[j].sockfd == i)
                            {
                                printf("\n\nThe peer at port \033[1;35m%d\033[0m (id: %d) has \033[1;31mdisconnected\033[0m.\n\n",
                                       conn_queue[j].port, conn_queue[j].id);
                                for (int k = j; k < conn_count - 1; k++)
                                    conn_queue[k] = conn_queue[k + 1];
                                conn_count--;
                                break;
                            }
                        }
                        pthread_mutex_unlock(&lock);
                    }
                    else
                    {
                        perror("read error");
                    }
                }
            }
        }
    }
}
