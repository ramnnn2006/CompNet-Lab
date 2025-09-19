#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int voct(const char *s) { // validate octet
    int l = 0;
    while (s[l] != '\0') l++;
    if (l == 0) return 0;
    if (l > 1 && s[0] == '0') return 0;
    for (int i = 0; i < l; i++) {
        if (!isdigit(s[i])) return 0;
    }
    int v = atoi(s);
    if (v < 0 || v > 255) return 0;
    return 1;
}

int main() {
    char ip[40], t[40];
    int oc[4];
    printf("Enter IPv4: ");
    scanf("%39s", ip);

    strcpy(t, ip);
    char *p = strtok(t, ".");
    int c = 0;

    while (p && c < 4) {
        if (!voct(p)) {
            printf("Invalid IP\n");
            return 1;
        }
        oc[c++] = atoi(p);
        p = strtok(NULL, ".");
    }

    if (c != 4) {
        printf("Invalid IP\n");
        return 1;
    }

    char cls;
    unsigned char m[4] = {0};
    if (oc[0] <= 127) { cls = 'A'; m[0]=255; }
    else if (oc[0] <= 191) { cls = 'B'; m[0]=255; m[1]=255; }
    else if (oc[0] <= 223) { cls = 'C'; m[0]=255; m[1]=255; m[2]=255; }
    else if (oc[0] <= 239) { cls = 'D'; }
    else { cls = 'E'; }

    printf("Valid IP. Class: %c\n", cls);


if (cls == 'D' || cls == 'E') {
    // Mask all zero
    for (int i = 0; i < 4; i++) {
        m[i] = 0;
    }
}

    unsigned char nid[4], hid[4], bc[4], fh[4], lh[4];
    for (int i = 0; i < 4; i++) {
        nid[i] = oc[i] & m[i];
        hid[i] = oc[i] & (~m[i]);
        bc[i] = nid[i] | (~m[i]);
        fh[i] = nid[i];
        lh[i] = bc[i];
    }
    fh[3] += 1;
    lh[3] -= 1;

    unsigned int hb = 0, mb = 0;
    for (int i = 0; i < 4; i++) {
        unsigned char x = m[i];
        for (int b = 7; b >= 0; b--) {
            if (x & (1 << b)) mb++;
        }
    }
    hb = 32 - mb;
    unsigned int th = (1U << hb) - 2;

    printf("Default Subnet Mask: %d.%d.%d.%d\n", m[0], m[1], m[2], m[3]);
    printf("Network ID: %d.%d.%d.%d\n", nid[0], nid[1], nid[2], nid[3]);
    printf("Host ID: %d.%d.%d.%d\n", hid[0], hid[1], hid[2], hid[3]);
    printf("Broadcast Address: %d.%d.%d.%d\n", bc[0], bc[1], bc[2], bc[3]);
    printf("First Host: %d.%d.%d.%d\n", fh[0], fh[1], fh[2], fh[3]);
    printf("Last Host: %d.%d.%d.%d\n", lh[0], lh[1], lh[2], lh[3]);
    printf("Total Hosts: %u\n", th);
