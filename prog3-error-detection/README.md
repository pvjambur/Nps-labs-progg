# Program 3 – Error Detection and Correction

Two sub-programs covering IP checksum (error detection) and Hamming code (error correction).

---

## Files

| File | Role |
|------|------|
| `checksum.c` | IP header checksum – sender computes it, receiver verifies it |
| `hamming.c` | Hamming (7,4) code – encodes 4 bits, detects and corrects single-bit errors |

---

## 3A – IP Checksum

### Compile & Run

```bash
gcc checksum.c -o checksum
./checksum
```

### How It Works

1. Sender enters 10 IP header fields in 16-bit hex; program computes 1's-complement checksum.
2. Receiver enters the same 10 fields plus the checksum as the 11th value.
3. If receiver result is `0x0000` → no error; otherwise → error detected.

### checksum.c

```c
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
```

---

## 3B – Hamming Code (7,4)

### Compile & Run

```bash
gcc hamming.c -o hamming
./hamming
```

### How It Works

1. Enter 4 data bits; program calculates 3 parity bits and outputs the 7-bit codeword.
2. Enter the 7 received bits (simulate an error by flipping one bit).
3. Syndrome bits pinpoint the error position; the bit is corrected automatically.

### hamming.c

```c
/* Program 3B: Hamming Code (7,4) – Error Detection and Correction
   Encodes 4 data bits into 7-bit Hamming code; detects and corrects single-bit errors */

#include <stdio.h>

int main()
{
    int data[8] = {0};
    int recv[8] = {0};
    int c, c0, c1, c2, i;

    printf("Enter four data bits (space-separated, e.g. 1 0 1 1): ");
    scanf("%d %d %d %d", &data[3], &data[5], &data[6], &data[7]);

    /* Calculate parity bits */
    data[1] = (data[3] + data[5] + data[7]) % 2;
    data[2] = (data[3] + data[6] + data[7]) % 2;
    data[4] = (data[5] + data[6] + data[7]) % 2;

    printf("The Data encoded (7 bits) is: ");
    for (i = 1; i < 8; i++) printf("%d", data[i]);
    printf("\n");

    printf("Enter received 7 bits (space-separated): ");
    for (i = 1; i < 8; i++) scanf("%d", &recv[i]);

    /* Syndrome calculation */
    c0 = (recv[1] + recv[3] + recv[5] + recv[7]) % 2;
    c1 = (recv[2] + recv[3] + recv[6] + recv[7]) % 2;
    c2 = (recv[4] + recv[5] + recv[6] + recv[7]) % 2;
    c  = c2 * 4 + c1 * 2 + c0;

    if (c == 0)
        printf("No error detected in received data.\n");
    else if (c > 0 && c < 8) {
        printf("Error detected at position %d. Correcting...\n", c);
        recv[c] ^= 1;
    } else
        printf("Invalid syndrome.\n");

    printf("The Data after correction is: ");
    for (i = 1; i < 8; i++) printf("%d", recv[i]);
    printf("\n");

    printf("Extracted original data bits: %d %d %d %d\n",
           recv[3], recv[5], recv[6], recv[7]);
    return 0;
}
```
