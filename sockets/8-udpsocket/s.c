#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

// Function to check even parity of data bytes
int check_even_parity(const char *data, int len) {
    int count = 0;
    for (int i = 0; i < len; i++) {
        unsigned char ch = (unsigned char)data[i];
        for (int bit = 0; bit < 8; bit++) {
            if (ch & (1 << bit)) {
                count++;
            }
        }
    }
    return (count % 2 == 0);
}

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server details
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server listening on port %d...\n", PORT);

    while (1) {
        int n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom error");
            continue;
        }
        buffer[n] = '\0';

        printf("Received data: %s\n", buffer);

        // Parity check
        if (check_even_parity(buffer, n)) {
            char *msg = "Data received correctly (parity valid).";
            sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&cliaddr, len);
            printf("Parity check passed.\n");
        } else {
            char *msg = "Error detected in data (parity invalid).";
            sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&cliaddr, len);
            printf("Parity check failed.\n");
        }
    }
    close(sockfd);
    return 0;
}
