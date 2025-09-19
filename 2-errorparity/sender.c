#include <stdio.h>

int calpar(int data[], int si) {
    int ct = 0;
    for(int i = 0; i < si; i++) {
        if(data[i] == 1)
            ct++;
    }
    return (ct % 2 == 0) ? 0 : 1;
}

int main() {
    int si;

    printf("Enter the no.of data bits: ");
    scanf("%d", &si);

    int data[si]; 
    printf("Enter %d data bits (0 or 1): ", si);
    for(int i = 0; i < si; i++) {
        scanf("%d", &data[i]);
        if(data[i] != 0 && data[i] != 1) {
            printf("Invalid...only 0 / 1 \n");
            return 1;
        }
    }
    int parbit = calpar(data, si);
    printf("Calculated parity bit (even parity): %d\n", parbit);

    int tr[si + 1]; 
    for(int i = 0; i < si; i++) {
        tr[i] = data[i];
    }
    tr[si] = parbit; 

    printf("The data to be transmitted is: ");
    for(int i = 0; i < si + 1; i++) {
printf("%d", tr[i]);
    }
    printf("\n");

    return 0;
}
