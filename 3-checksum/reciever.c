#include <stdio.h>
#include <string.h>

#define B 8
#define M 10

unsigned char awc(unsigned char a, unsigned char b) {
    unsigned short s = a + b;
    if (s > 0xFF)
        s = (s & 0xFF) + 1;
    return (unsigned char)s;
}

void pb(unsigned char n) {
    for (int i = B - 1; i >= 0; i--)
        printf("%d", (n >> i) & 1);
}

int btb(char *s, unsigned char *r) {
    if (strlen(s) != B) return 0;
    unsigned char v = 0;
    for (int i = 0; i < B; i++) {
        if (s[i] != '0' && s[i] != '1') return 0;
        v = (v << 1) | (s[i] - '0');
    }
    *r = v;
    return 1;
}

int main() {
    int c;
    char iStr[20];
    unsigned char n[M];

    printf("Enter number of 8-bit nums (excl checksum, max %d):\n", M);
    if (scanf("%d", &c) != 1 || c <= 0 || c > M) {
        printf("Invalid count.\n");
        return 1;
    }

    printf("Enter %d 8-bit binary nums:\n", c);
    for (int j = 0; j < c; j++) {
        scanf("%s", iStr);
        if (!btb(iStr, &n[j])) {
            printf("Invalid input: %s\n", iStr);
            return 1;
        }
    }

    printf("Enter checksum as 8-bit binary:\n");
    scanf("%s", iStr);
    unsigned char ch;
    if (!btb(iStr, &ch)) {
        printf("Invalid checksum: %s\n", iStr);
        return 1;
    }

    unsigned char s = n[0];
    printf("Intermediate sums:\n");
    printf("Sum after add num 1: ");
    pb(s);
    printf(" (%d)\n", s);

    for (int j = 1; j < c; j++) {
        s = awc(s, n[j]);
        printf("Sum after add num %d: ", j + 1);
        pb(s);
        printf(" (%d)\n", s);
    }

    s = awc(s, ch);
    printf("Sum after add checksum: ");
    pb(s);
    printf(" (%d)\n", s);

    if (s == 0xFF) {
        printf("\nIntegrity Check: passed\n");
    } else {
        printf("\nIntegrity Check: FAILED\n");
    }

    return 0;
}
