#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int create_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}


void connect_to_server(int sockfd, struct sockaddr_in *servaddr) {
    if (connect(sockfd, (struct sockaddr *)servaddr, sizeof(*servaddr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void chat_loop(int sockfd) {
    char send_buffer[1024];
    char recv_buffer[1024];

    while (1) {
        printf("Your message: ");
        if (fgets(send_buffer, sizeof(send_buffer), stdin) == NULL) {
            printf("Error reading input\n");
            break;
        }

        if (send(sockfd, send_buffer, strlen(send_buffer), 0) < 0) {
            perror("Send failed");
            break;
        }

        if (strncmp(send_buffer, "exit", 4) == 0) {
            printf("Exiting chat.\n");
            break;
        }

        memset(recv_buffer, 0, sizeof(recv_buffer));
        ssize_t bytes_received = recv(sockfd, recv_buffer, sizeof(recv_buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("Server disconnected or error occurred\n");
            break;
        }

        printf("Server: %s", recv_buffer);
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = create_socket();

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(22000);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect_to_server(sockfd, &servaddr);

    printf("Connected to server!\n");

    chat_loop(sockfd);

    close(sockfd);

    return 0;
}
