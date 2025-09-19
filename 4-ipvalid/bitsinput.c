#include <stdio.h>
#include <string.h>
int b2d(const char *s, int len) {
    int x = 0;
    for (int i = 0; i < len; i++)
        x = x*2 + (s[i] - '0');
    return x; }

int v(const char *s) {
    if (strlen(s) != 32) return 0;
    for (int i = 0; i < 32; i++)
        if (s[i] != '0' && s[i] != '1') return 0;
    return 1; }

char cls(int x) {
    if (x <= 127) return 'A';
    if (x <= 191) return 'B';
    if (x <= 223) return 'C';
    if (x <= 239) return 'D';
    return 'E';}

int main() {
    char s[40];
    printf("Enter 32-bit binary IP: ");
    scanf("%39s", s);

    if (!v(s)) {
        printf("Invalid IP.\n");
        return 0;
    }
    int o[4]; 
    for (int i = 0; i < 4; i++)
        o[i] = b2d(s + 8*i, 8);

    printf("Valid IP. Decimal: %d.%d.%d.%d Class: %c\n", o[0], o[1], o[2], o[3], cls(o[0]));
    return 0;
}
