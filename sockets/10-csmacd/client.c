include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024], result[256];

    // 1. Input parameters
    double bandwidth, delay;
    int frame_size;
    printf("Enter Bandwidth (Mbps): ");
    scanf("%lf", &bandwidth);
    printf("Enter Propagation Delay (microseconds): ");
    scanf("%lf", &delay);
    printf("Enter Frame Size (bytes, or 0 to skip): ");
    scanf("%d", &frame_size);

    // 2. Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 3. Define server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 4. Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 5. Send data
    sprintf(buffer, "%lf %lf %d", bandwidth, delay, frame_size);
    send(sockfd, buffer, strlen(buffer)+1, 0);

    // 6. Receive result
    recv(sockfd, result, sizeof(result), 0);
    printf("Minimum frame size required for CSMA/CD: %s bytes\n", result);

    close(sockfd);
    return 0;
}
