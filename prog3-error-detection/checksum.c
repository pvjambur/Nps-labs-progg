/* Program 3A: IP Header Checksum (Error Detection)
   Sender computes checksum; receiver verifies it equals 0x0000 */

#include <stdio.h>

unsigned short compute_checksum(int count)
{
    unsigned int sum = 0;
    unsigned field;
    int i;

    for (i = 0; i < count; i++) {
        scanf("%x", &field);
        sum += (unsigned short)field;
        while (sum >> 16)
            sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (unsigned short)(~sum);
}

int main()
{
    unsigned short sender_cs, receiver_cs;

    printf("Enter 10 IP header fields in 16-bit hex (sender):\n");
    sender_cs = compute_checksum(10);
    printf("\nComputed Checksum at sender: %04x\n", sender_cs);

    printf("\nEnter the same 10 fields + checksum as 11th value (receiver):\n");
    receiver_cs = compute_checksum(11);
    printf("\nComputed Checksum at receiver: %04x\n", receiver_cs);

    if (receiver_cs == 0x0000)
        printf("\nNo error in Data received\n");
    else
        printf("\nError in Data received\n");

    return 0;
}
