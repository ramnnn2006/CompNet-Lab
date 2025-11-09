#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[1024];
    
    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // 2. Bind socket
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 3. Listen
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);
    
    // 4. Accept connection
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 5. Receive data
    recv(client_fd, buffer, sizeof(buffer), 0);
    double bandwidth, delay;
    int frame_size = 0;
    sscanf(buffer, "%lf %lf %d", &bandwidth, &delay, &frame_size);

    // 6. Calculate minimum frame size
    // Bandwidth: Mbps to bits/sec, Delay: us to sec
    double minimum_bits = 2 * (bandwidth * 1e6) * (delay * 1e-6); 
    int minimum_bytes = (int)((minimum_bits + 7) / 8); // round up to nearest byte

    char result[256];
    sprintf(result, "%d", minimum_bytes);

    // 7. Send result back to client
    send(client_fd, result, strlen(result)+1, 0);

    // 8. Compare with provided frame size (optional)
    if (frame_size > 0) {
        printf("User frame size: %d bytes\n", frame_size);
        printf("Minimum frame size: %d bytes\n", minimum_bytes);
        if (frame_size >= minimum_bytes) {
            printf("Frame size is efficient for CSMA/CD.\n");
        } else {
            printf("Frame size is too small for efficient CSMA/CD.\n");
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
