 #include <stdio.h>
#define BITS 8

unsigned char awc(unsigned char a, unsigned char b) {
    unsigned short sum = a + b;
    if (sum > 0xFF)
        sum = (sum & 0xFF) + 1;  
    return (unsigned char)sum;
}
void prbin(unsigned char n) {
    for (int i = BITS - 1; i >= 0; --i)
        printf("%d", (n >> i) & 1);
}
int main() {
    int count;
    int temp;
    unsigned char nums[10];
    printf("number of 8-bit numbers to input :\n");
    if(scanf("%d", &count) != 1 || count <= 0 || count > 10) {
        printf("Invalid input count.\n");
        return 1;
    }
    for (int i = 0; i < count; i++) {
        if(scanf("%d", &temp) != 1 || temp < 0 || temp > 255) {
            printf("Input invalid.\n");
            return 1;
        }
        nums[i] = (unsigned char)temp;
    }
    unsigned char sum = nums[0];
    printf("Sum after adding number 1: ");
    prbin(sum);
    printf(" (%d)\n", sum);

    for (int i = 1; i < count; i++) {
        sum = awc(sum, nums[i]);
        printf("Sum after adding number %d: ", i + 1);
        prbin(sum);
        printf(" (%d)\n", sum);
    }
    unsigned char cs = ~sum;
    printf("\nFinal sum: ");
    prbin(sum);
    printf(" (%d)\n", sum);
    printf("Checksum: ");
    prbin(cs);
    printf(" (%d)\n", cs);
    printf("\nData to be transmitted (binary): ");
    for (int i = 0; i < count; i++) {
        prbin(nums[i]);
        printf(" ");
    }
    prbin(cs);
    printf(" (checksum)\n");

    return 0;
}
