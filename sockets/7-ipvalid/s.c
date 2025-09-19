#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int is_valid_octet(const char *s) {
    int len = 0;
    while (s[len] != '\0' && s[len] != '.') len++;
    if (len == 0) return -1;
    if (len > 1 && s[0] == '0') return -2;
    for (int i = 0; i < len; i++) {
        if (!isdigit(s[i])) return -3;
    }
    int val = 0;
    for (int i = 0; i < len; i++) {
        val = val * 10 + (s[i] - '0');
    }
    if (val < 0 || val > 255) return -4;
    return val;
}

char get_class(int x) {
    if (x <= 127) return 'A';
    if (x <= 191) return 'B';
    if (x <= 223) return 'C';
    if (x <= 239) return 'D';
    return 'E';
}

int validate_ip(const char *ip, char *msg, int msg_len) {
    int dot_count = 0;
    int start = 0;
    int len = strlen(ip);
    int f_oct = -1;

    for (int i = 0; i <= len; i++) {
        if (ip[i] == '.' || ip[i] == '\0') {
            int part_len = i - start;
            if (part_len <= 0) {
                snprintf(msg, msg_len, "Invalid IP: Empty octet\n");
                return 0;
            }
            if (part_len > 3) {
                snprintf(msg, msg_len, "Invalid IP: Octet too long\n");
                return 0;
            }
            char part[4];
            for (int j = 0; j < part_len; j++) {
                part[j] = ip[start + j];
            }
            part[part_len] = '\0';

            int val = is_valid_octet(part);
            if (val == -1) { snprintf(msg, msg_len, "Invalid IP: Empty octet\n"); return 0; }
            else if (val == -2) { snprintf(msg, msg_len, "Invalid IP: Leading zero in octet\n"); return 0; }
            else if (val == -3) { snprintf(msg, msg_len, "Invalid IP: Non-digit char in octet\n"); return 0; }
            else if (val == -4) { snprintf(msg, msg_len, "Invalid IP: Octet out of range (0-255)\n"); return 0; }

            if (dot_count == 0) f_oct = val;
            dot_count++;
            start = i + 1;
        }
    }
    if (dot_count != 4) {
        snprintf(msg, msg_len, "Invalid IP: Incorrect number of octets\n");
        return 0;
    }
    char cl = get_class(f_oct);
    snprintf(msg, msg_len, "Valid IP. Class: %c\n", cl);
    return 1;
}

int create_socket() {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        perror("socket");
        exit(1);
    }
    return sfd;
}

void bind_socket(int sfd, int port) {
    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(port);

    if (bind(sfd, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
        perror("bind");
        close(sfd);
        exit(1);
    }
}

void listen_socket(int sfd) {
    if (listen(sfd, 5) < 0) {
        perror("listen");
        close(sfd);
        exit(1);
    }
}

int accept_connection(int sfd) {
    int cfd = accept(sfd, NULL, NULL);
    if (cfd < 0) {
        perror("accept");
        close(sfd);
        exit(1);
    }
    return cfd;
}

void handle_client(int cfd) {
    char buf[64] = {0};
    int r = recv(cfd, buf, sizeof(buf) - 1, 0);
    if (r <= 0) {
        close(cfd);
        return;
    }
    buf[r] = 0;

    char res[128];
    validate_ip(buf, res, sizeof(res));
    send(cfd, res, strlen(res), 0);

    close(cfd);
}

int main() {
    int sfd = create_socket();
    bind_socket(sfd, 22000);
    listen_socket(sfd);

    printf("Server ready, waiting for client...\n");

    while (1) {
        int cfd = accept_connection(sfd);
        handle_client(cfd);
    }

    close(sfd);
    return 0;
}
