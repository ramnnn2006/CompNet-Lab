#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int create_socket() {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        perror("socket");
        exit(1);
    }
    return sfd;
}

void setup_server_addr(struct sockaddr_in *sa, const char *ip, int port) {
    memset(sa, 0, sizeof(*sa));
    sa->sin_family = AF_INET;
    sa->sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &sa->sin_addr) <= 0) {
        perror("inet_pton");
        exit(1);
    }
}

void connect_server(int sfd, struct sockaddr_in *sa) {
    if (connect(sfd, (struct sockaddr *)sa, sizeof(*sa)) < 0) {
        perror("connect");
        close(sfd);
        exit(1);
    }
}

void send_ip(int sfd, const char *ip) {
    size_t len = strlen(ip);
    if (send(sfd, ip, len, 0) != (ssize_t)len) {
        perror("send");
        close(sfd);
        exit(1);
    }
}

void recv_result(int sfd) {
    char buf[128];
    ssize_t r = recv(sfd, buf, sizeof(buf) - 1, 0);
    if (r <= 0) {
        printf("No response or error from server.\n");
    } else {
        buf[r] = 0;
        printf("Server: %s", buf);
    }
}

int main() {
    int sfd = create_socket();

    struct sockaddr_in sa;
    setup_server_addr(&sa, "127.0.0.1", 22000);

    connect_server(sfd, &sa);

    char ip[40];
    printf("Enter IP to validate: ");
    if (fgets(ip, sizeof(ip), stdin) == NULL) {
        printf("Error reading input\n");
        close(sfd);
        return 1;
    }

    // Remove newline if present
    size_t l = strlen(ip);
    if (l > 0 && ip[l-1] == '\n') ip[l-1] = '\0';

    send_ip(sfd, ip);
    recv_result(sfd);

    close(sfd);
    return 0;
}
