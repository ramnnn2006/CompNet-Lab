#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

// Function to add even parity bit by toggling last bit if needed
void add_even_parity(char *data, int len) {
    int count = 0;
    for (int i = 0; i < len; i++) {
        unsigned char ch = (unsigned char)data[i];
        for (int bit = 0; bit < 8; bit++) {
            if (ch & (1 << bit)) {
                count++;
            }
        }
    }
    if (count % 2 != 0) {
        data[len - 1] ^= 1; // toggle least significant bit for parity
    }
}

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char message[MAXLINE];
    printf("Enter data to send: ");
    fgets(message, MAXLINE, stdin);
    int msg_len = strlen(message);

    // Remove newline if present
    if (message[msg_len - 1] == '\n') {
        message[msg_len -1] = '\0';
        msg_len--;
    }

    add_even_parity(message, msg_len);

    sendto(sockfd, (const char *)message, msg_len, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Data sent with parity bit.\n");

    int n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, NULL, NULL);
    if (n < 0) {
        perror("recvfrom error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("Server response: %s\n", buffer);

    close(sockfd);
    return 0;
}
