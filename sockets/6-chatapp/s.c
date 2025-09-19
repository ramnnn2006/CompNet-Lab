#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int create_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}


void bind_socket(int sockfd, struct sockaddr_in *addr) {
    if (bind(sockfd, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void start_listening(int sockfd) {
    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

int accept_connection(int sockfd) {
    int comm_fd = accept(sockfd, NULL, NULL);
    if (comm_fd < 0) {
        perror("Accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    return comm_fd;
}

void chat_loop(int comm_fd) {
    char recv_buffer[1024];
    char send_buffer[1024];

    while (1) {
        memset(recv_buffer, 0, sizeof(recv_buffer));
        ssize_t bytes_received = recv(comm_fd, recv_buffer, sizeof(recv_buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("Client disconnected or error occurred\n");
            break;
        }
        printf("Client: %s", recv_buffer);

        printf("Your message: ");
        if (fgets(send_buffer, sizeof(send_buffer), stdin) == NULL) {
            printf("Error reading input\n");
            break;
        }

        if (send(comm_fd, send_buffer, strlen(send_buffer), 0) < 0) {
            perror("Send failed");
            break;
        }

        if (strncmp(send_buffer, "exit", 4) == 0) {
            printf("Exiting chat.\n");
            break;
        }
    }
}

int main() {
    int listen_fd;
    struct sockaddr_in servaddr;

    listen_fd = create_socket();

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(22000);            

    bind_socket(listen_fd, &servaddr);
    start_listening(listen_fd);

    printf("Waiting for a client to connect...\n");
    int comm_fd = accept_connection(listen_fd);
    printf("Client connected!\n");

    chat_loop(comm_fd);

    close(comm_fd);
    close(listen_fd);

    return 0;
}
