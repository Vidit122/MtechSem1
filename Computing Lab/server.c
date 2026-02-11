#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 50
#define BUF_SIZE 1024

int client_sockets[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(const char *message, int exclude_fd) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && client_sockets[i] != exclude_fd) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int fd) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == fd) {
            client_sockets[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    char buffer[BUF_SIZE];
    char username[50];

    int len = recv(client_fd, username, sizeof(username) - 1, 0);
    if (len <= 0) {
        close(client_fd);
        return NULL;
    }
    username[len] = '\0';

    printf("[INFO] User '%s' joined the chat.\n", username);

    char join_msg[100];
    snprintf(join_msg, sizeof(join_msg), "SERVER: %s has joined the chat.\n", username);
    broadcast_message(join_msg, client_fd);

    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int bytes = recv(client_fd, buffer, BUF_SIZE, 0);
        if (bytes <= 0) {
            printf("[INFO] User '%s' disconnected.\n", username);
            char leave_msg[100];
            snprintf(leave_msg, sizeof(leave_msg), "SERVER: %s has left the chat.\n", username);
            broadcast_message(leave_msg, client_fd);
            remove_client(client_fd);
            close(client_fd);
            break;
        }
        buffer[bytes] = '\0';

        if (strcmp(buffer, "/quit\n") == 0) {
            printf("[INFO] User '%s' disconnected.\n", username);
            char leave_msg[100];
            snprintf(leave_msg, sizeof(leave_msg), "SERVER: %s has left the chat.\n", username);
            broadcast_message(leave_msg, client_fd);
            remove_client(client_fd);
            close(client_fd);
            break;
        }

        printf("[MESSAGE] %s: %s", username, buffer);

        char msg[BUF_SIZE + 50];
        snprintf(msg, sizeof(msg), "%s: %s", username, buffer);
        broadcast_message(msg, client_fd);
    }

    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);

    memset(client_sockets, 0, sizeof(client_sockets));

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d. Waiting for clients...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("[INFO] New client connected from %s:%d\n", client_ip, ntohs(address.sin_port));

        pthread_mutex_lock(&clients_mutex);
        int added = 0;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = new_socket;
                added = 1;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        if (!added) {
            char *msg = "Server full!\n";
            send(new_socket, msg, strlen(msg), 0);
            close(new_socket);
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, &new_socket);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
