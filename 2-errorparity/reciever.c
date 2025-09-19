#include <stdio.h>

int chepar(int data[], int si) { 
    int ct = 0;
    for(int i = 0; i < si; i++) {
        if(data[i] == 1)
            ct++;
    }
    return (ct % 2 == 0) ? 0 : 1;
}

int main() {
    int received_si; 

    printf("Enter the total length of the sequence received: ");
    scanf("%d", &received_si);

    if (received_si <= 0) {
        printf("Invalid number of bits.\n");
        return 1;
    }

    int tr[received_si]; 
    printf("Enter %d received bits (0 or 1): ", received_si);
    for(int i = 0; i < received_si; i++) {
        scanf("%d", &tr[i]);
        if(tr[i] != 0 && tr[i] != 1) {
            printf("Invalid input! Please enter only 0 or 1.\n");
            return 1;
        }
    }

    int error = chepar(tr, received_si); 
    if(error == 0) {
        printf("No single-bit error detected in the received data.\n");
    } else {
        printf("Single-bit error detected in the received data!\n");
    }

    return 0;
}
