#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int is_valid_octet(const char *s) {
    int len = 0;
    while (s[len] != '\0' && s[len] != '.') len++;
    if (len == 0) {
        printf("Invalid IP: Empty octet\n");
        return 0;
    }
    if (len > 1 && s[0] == '0') {
        printf("Invalid IP: Leading zero in octet\n");
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (!isdigit(s[i])) {
            printf("Invalid IP: Non-digit char in octet\n");
            return 0;
        }
    }

    int val = 0;
    for (int i = 0; i < len; i++) {
        val = val * 10 + (s[i] - '0');
    }
    if (val < 0 || val > 255) {
        printf("Invalid IP: Octet out of range (0-255)\n");
        return 0;
    }
    return 1;
}

int main() {
    char ip[40];
    printf("Enter IPv4: ");
    scanf("%39s", ip);

    int dot_count = 0;
    int start = 0;
    int len = strlen(ip);
    int f_oct = -1;
    for (int i = 0; i <= len; i++) {
        if (ip[i] == '.' || ip[i] == '\0') {
            int part_len = i - start;
            if (part_len <= 0) {
                printf("Invalid IP: Empty octet\n");
                return 1;
            }
            char part[4];
            if (part_len > 3) {
                printf("Invalid IP: Octet too long\n");
                return 1;
            }
            for (int j = 0; j < part_len; j++) {
                part[j] = ip[start + j];
            }
            part[part_len] = '\0';

            if (!is_valid_octet(part)) {
                return 1;
            }
            int val = 0;
            for (int j = 0; j < part_len; j++) {
                val = val * 10 + (part[j] - '0');
            }
            if (dot_count == 0) f_oct = val;

            dot_count++;
            start = i + 1;
        }
    }

    if (dot_count != 4) {
        printf("Invalid IP: Incorrect number of octets\n");
        return 1;
    }

    char c;
    if (f_oct <= 127) c = 'A';
    else if (f_oct <= 191) c = 'B';
    else if (f_oct <= 223) c = 'C';
    else if (f_oct <= 239) c = 'D';
    else c = 'E';

    printf("Valid IP. Class: %c\n", c);
    return 0;
}
