#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

void* handle_client(void* arg) {
    int client_sock = *(int*)arg;
    free(arg);

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    char buffer[1024];
    ssize_t n;

    if (getpeername(client_sock, (struct sockaddr*)&addr, &addr_len) == -1) {
        perror("getpeername failed");
        close(client_sock);
        pthread_exit(NULL);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, client_ip, sizeof(client_ip));
    int client_port = ntohs(addr.sin_port);
    printf("Client connected from %s:%d\n", client_ip, client_port);

    while ((n = recv(client_sock, buffer, sizeof(buffer)-1, 0)) > 0) {
        buffer[n] = '\0';
        printf("Message from %s:%d - %s\n", client_ip, client_port, buffer);
        send(client_sock, buffer, n, 0); // echo back
    }

    close(client_sock);
    printf("Client %s:%d disconnected\n", client_ip, client_port);
    pthread_exit(NULL);
}

int main() {
    int server_sock, *client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t tid;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);

    printf("Server listening on port 8989\n");

    while (1) {
        client_sock = malloc(sizeof(int));
        *client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);

        pthread_create(&tid, NULL, handle_client, client_sock);
        pthread_detach(tid);
    }

    return 0;
}
