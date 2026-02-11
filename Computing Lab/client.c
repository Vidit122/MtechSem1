#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUF_SIZE 1024

int sock = 0;

void *receive_handler(void *arg) {
    char buffer[BUF_SIZE];
    while (1) {
        int bytes = recv(sock, buffer, BUF_SIZE - 1, 0);
        if (bytes <= 0) {
            printf("Disconnected from server.\n");
            close(sock);
            exit(0);
        }
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }
    return NULL;
}

int main() {
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    char username[50];
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    send(sock, username, strlen(username), 0);

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_handler, NULL);
    pthread_detach(recv_thread);

    while (1) {
        fgets(buffer, BUF_SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "/quit\n") == 0) {
            printf("You have left the chat.\n");
            printf("Disconnected from server.\n");
            close(sock);
            exit(0);
        }
    }

    return 0;
}
