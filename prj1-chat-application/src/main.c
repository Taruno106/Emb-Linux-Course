

#include "../inc/chat.h"

char *portno;

void *server_thread(void *arg)
{
    create_server((char *)arg);
    return NULL;
}

void *client_thread(void *arg)
{
    connection_t *conn = (connection_t *)arg;
    create_client(conn);
    return NULL;
}

void show_info_help()
{
    printf("================================== Chat Application ==================================\n");
    printf("\n");
    printf("Use the commands below:                                           \n");
    printf("  1. help                               : display user interface options   \n");
    printf("  2. myip                               : display IP address of this app   \n");
    printf("  3. myport                             : display listening port of this app \n");
    printf("  4. connect <destination> <port no>    : connect to the app of another computer \n");
    printf("  5. list                               : list all the connections of this app \n");
    printf("  6. terminate <connection id>          : terminate a connection           \n");
    printf("  7. send <connection id> <message>     : send a message to a connection \n");
    printf("  8. exit                               : close all connections & terminate this app \n");
    printf("\n");
    printf("=====================================================================================\n\n");
}

void list_connection()
{
    printf("=============================================\n");
    printf("ID |       IP Address       |  Port No.  \n");
    pthread_mutex_lock(&lock);
    for (int i = 0; i < conn_count; i++)
    {
        printf("%-2d |    \033[1;34m%-19s\033[0m |  \033[1;35m%-18d\033[0m\n",
               conn_queue[i].id,
               conn_queue[i].ip,
               conn_queue[i].port);
    }
    pthread_mutex_unlock(&lock);
    printf("=============================================\n");
}

void get_my_ip(char *ip_buffer, size_t buf_len)
{
    struct sockaddr_in dummy_addr;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        strncpy(ip_buffer, "unknown", buf_len);
        return;
    }

    dummy_addr.sin_family = AF_INET;
    dummy_addr.sin_port = htons(80);
    inet_pton(AF_INET, "8.8.8.8", &dummy_addr.sin_addr); // Google DNS

    connect(sock, (struct sockaddr *)&dummy_addr, sizeof(dummy_addr));

    struct sockaddr_in local_addr;
    socklen_t addr_len = sizeof(local_addr);
    getsockname(sock, (struct sockaddr *)&local_addr, &addr_len);

    inet_ntop(AF_INET, &local_addr.sin_addr, ip_buffer, buf_len);

    close(sock);
}

void connect_to_host(const char *ip, const char *port)
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Client: create socket failed");
        return;
    }

    // cấp phát vùng nhớ mới
    connection_t *conn = malloc(sizeof(connection_t));
    strcpy(conn->ip, ip);
    conn->port = atoi(port);
    conn->sockfd = sock;
    conn->id = conn_count + 1;

    pthread_t cli_tid;
    pthread_create(&cli_tid, NULL, client_thread, conn);
}

void do_cmd_send(char *cmd, char *action, char *id_str)
{
    int id = atoi(id_str);

    char *msg;
    if (action && id_str)
    {
        msg = strstr(cmd + strlen(action) + strlen(id_str) + 2, "");
    }
    pthread_mutex_lock(&lock);
    for (int i = 0; i < conn_count; i++)
    {
        // printf("conn_queue[%d] - id: %d - sockfd: %d\n", i, conn_queue[i].id, conn_queue[i].sockfd);
        if (id == conn_queue[i].id)
        {
            send(conn_queue[i].sockfd, msg, strlen(msg), 0);
            printf("\nSend message \033[1;32mSuccessfully\033[0m.\n\n");
            break;
        }
    }
    pthread_mutex_unlock(&lock);
}

void do_cmd_terminate(const char *id_str)
{
    if (!id_str)
    {
        printf("Usage: terminate <sockfd>\n");
        return;
    }

    int id = atoi(id_str);
    int found = 0;

    pthread_mutex_lock(&lock);
    for (int i = 0; i < conn_count; i++)
    {
        if (id == conn_queue[i].id)
        {
            shutdown(conn_queue[i].sockfd, SHUT_RDWR);
            close(conn_queue[i].sockfd);
            printf("Closed connection with ID: %d\n", id);

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

void do_cmd_exit()
{
    printf("Closing all connections...\n");
    pthread_mutex_lock(&lock);
    for (int i = 0; i < conn_count; i++)
    {
        shutdown(conn_queue[i].sockfd, SHUT_RDWR);
        close(conn_queue[i].sockfd);
        printf("Closed ID: %d (%s:%d)\n",
               i,
               conn_queue[i].ip,
               conn_queue[i].port);
    }

    conn_count = 0;
    pthread_mutex_unlock(&lock);

    printf("Exiting application.\n");
    exit(0);
}

void handle_cmd()
{
    char cmd[100];
    char *action;
    while (1)
    {
        printf("Enter your command: ");
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = 0;

        // Dùng strtok để tách
        action = strtok(cmd, " ");

        if (action && strcmp(action, "help") == 0)
        {
            // printf("help: %s\n", action);
            show_info_help();
        }
        else if (action && strcmp(action, "myip") == 0)
        {
            char my_ip[INET_ADDRSTRLEN];
            get_my_ip(my_ip, sizeof(my_ip));
            printf("Local IP address: \033[1;34m%s\033[0m\n", my_ip);
        }
        else if (action && strcmp(action, "myport") == 0)
        {
            printf("Listening port of this app: \033[1;35m%s\033[0m\n", portno);
        }
        else if (action && strcmp(action, "connect") == 0)
        {
            char *ip = strtok(NULL, " ");
            char *port = strtok(NULL, " ");
            connect_to_host(ip, port);
        }
        else if (action && strcmp(action, "list") == 0)
        {
            list_connection();
        }
        else if (action && strcmp(action, "terminate") == 0)
        {
            char *id_str = strtok(NULL, " ");
            do_cmd_terminate(id_str);
        }
        else if (action && strcmp(action, "send") == 0)
        {
            char *id_str = strtok(NULL, " ");
            do_cmd_send(cmd, action, id_str);
        }
        else if (action && strcmp(action, "exit") == 0)
        {
            do_cmd_exit();
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("The number of parameters should equal 2! Please check port\n");
        return -1;
    }

    portno = argv[1];
    pthread_t serv_tid;

    show_info_help();

    pthread_create(&serv_tid, NULL, server_thread, (void *)portno);
    pthread_detach(serv_tid);

    sleep(0.15); // thay bang signal
    handle_cmd();

    return 0;
}