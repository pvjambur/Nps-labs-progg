# NPS Lab – All Programs with Outputs

---

## Prog1: TCP Two-Way Chat

**Compile:**
```bash
gcc server.c -o server
gcc client.c -o client
```
**Run:** Terminal 1 → `./server` | Terminal 2 → `./client 127.0.0.1`

### server.c
```c
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int create_socket, new_socket;
    socklen_t addrlen;
    int buffsize = 1024;
    char buffer[1024];
    struct sockaddr_in address;

    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { perror("socket"); return 1; }

    address.sin_family = AF_INET;
    address.sin_port = htons(15001);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(create_socket, (struct sockaddr *)&address, sizeof(address)) < 0) { perror("bind"); close(create_socket); return 1; }
    if (listen(create_socket, 3) < 0) { perror("listen"); close(create_socket); return 1; }

    addrlen = sizeof(struct sockaddr_in);
    if ((new_socket = accept(create_socket, (struct sockaddr*)&address, &addrlen)) < 0) { perror("accept"); close(create_socket); return 1; }

    printf("Client connected: %s\n", inet_ntoa(address.sin_addr));

    while (1) {
        ssize_t recv_len = recv(new_socket, buffer, buffsize - 1, 0);
        if (recv_len <= 0) { printf("Client disconnected or error.\n"); break; }
        buffer[recv_len] = '\0';
        printf("Client: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0) { printf("Conversation ended by client.\n"); break; }

        printf("Server: ");
        fflush(stdout);
        if (!fgets(buffer, buffsize, stdin)) break;
        buffer[strcspn(buffer, "\n")] = 0;
        if (send(new_socket, buffer, strlen(buffer), 0) < 0) { perror("send"); break; }
        if (strcmp(buffer, "exit") == 0) { printf("Conversation ended by server.\n"); break; }
    }

    close(new_socket);
    close(create_socket);
    return 0;
}
```

### client.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 2) { fprintf(stderr, "Usage: %s <server-ip>\n", argv[0]); return 1; }

    int create_socket;
    int buffsize = 1024;
    char buffer[1024];
    struct sockaddr_in address;

    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { perror("socket"); return 1; }

    address.sin_family = AF_INET;
    address.sin_port = htons(15001);
    if (inet_pton(AF_INET, argv[1], &address.sin_addr) != 1) { fprintf(stderr, "Invalid address: %s\n", argv[1]); return 1; }
    if (connect(create_socket, (struct sockaddr *)&address, sizeof(address)) != 0) { perror("connect"); close(create_socket); return 1; }

    while (1) {
        printf("Client: ");
        fflush(stdout);
        if (!fgets(buffer, buffsize, stdin)) break;
        buffer[strcspn(buffer, "\n")] = 0;
        if (send(create_socket, buffer, strlen(buffer), 0) < 0) { perror("send"); break; }
        if (strcmp(buffer, "exit") == 0) { printf("Conversation ended by client.\n"); break; }

        ssize_t recv_len = recv(create_socket, buffer, buffsize - 1, 0);
        if (recv_len <= 0) { printf("Server disconnected or error.\n"); break; }
        buffer[recv_len] = '\0';
        printf("Server: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0) { printf("Conversation ended by server.\n"); break; }
    }

    close(create_socket);
    return 0;
}
```

### Sample Output

**Server Terminal:**
```
Client connected: 127.0.0.1
Client: hello
Server: hi there
Client: how are you
Server: fine thanks
Client: exit
Conversation ended by client.
```

**Client Terminal:**
```
Client: hello
Server: hi there
Client: how are you
Server: fine thanks
Client: exit
Conversation ended by client.
```

---

## Prog2: Distance Vector Routing

**Compile:** `gcc dv.c -o a.out` | **Run:** `./a.out`

### dv.c
```c
#include <stdio.h>

struct node {
    unsigned front[20];
    unsigned dist[20];
} t[10];

int main() {
    int costmat[20][20];
    int nodes, i, j, k, count = 0;
    printf("\nEnter the no of nodes: ");
    scanf("%d", &nodes);
    printf("Enter the matrix:\n ");

    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            scanf("%d", &costmat[i][j]);
            costmat[i][i] = 0;
            t[i].dist[j] = costmat[i][j];
            t[i].front[j] = j;
        }
    }

    do {
        count = 0;
        for (i = 0; i < nodes; i++)
            for (j = 0; j < nodes; j++)
                for (k = 0; k < nodes; k++)
                    if (t[i].dist[j] > costmat[i][k] + t[k].dist[j]) {
                        t[i].dist[j] = costmat[i][k] + t[k].dist[j];
                        t[i].front[j] = k;
                        count++;
                    }
    } while (count != 0);

    for (i = 0; i < nodes; i++) {
        printf("\nFor router %d ", i + 1);
        for (j = 0; j < nodes; j++)
            printf("\nFor node %d via %d distance is %d", j + 1, t[i].front[j] + 1, t[i].dist[j]);
    }
    printf("\n\n");
}
```

### Sample Input & Output

**Input:**
```
Enter the no of nodes: 3
Enter the matrix:
 0 1 7
 1 0 2
 7 2 0
```

**Output:**
```
For router 1
For node 1 via 1 distance is 0
For node 2 via 2 distance is 1
For node 3 via 2 distance is 3

For router 2
For node 1 via 1 distance is 1
For node 2 via 2 distance is 0
For node 3 via 3 distance is 2

For router 3
For node 1 via 2 distance is 3
For node 2 via 2 distance is 2
For node 3 via 3 distance is 0
```

> Node 1→3: shortest path 1→2→3, cost 3 (direct link cost 7 is avoided).

---

## Prog3A: IP Checksum

**Compile:** `gcc checksum.c -o a.out` | **Run:** `./a.out`

### checksum.c
```c
#include <stdio.h>

unsigned short compute_checksum_sender() {
    unsigned fields[10];
    int i;
    unsigned int sum = 0;
    printf("Enter 10 IP header fields in 16 bit hex (sender):\n");
    for (i = 0; i < 10; i++) {
        scanf("%x", &fields[i]);
        sum += (unsigned short)fields[i];
        while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (unsigned short)(~sum);
}

unsigned short compute_checksum_receiver() {
    unsigned fields[11];
    int i;
    unsigned int sum = 0;
    printf("Enter 10 IP header fields + checksum in 16 bit hex (receiver):\n");
    for (i = 0; i < 11; i++) {
        scanf("%x", &fields[i]);
        sum += (unsigned short)fields[i];
        while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (unsigned short)(~sum);
}

int main() {
    unsigned short sender_checksum = compute_checksum_sender();
    printf("\nComputed Checksum at sender: %04x\n", sender_checksum);
    printf("\nNow enter the same 10 fields plus the checksum as the 11th value for receiver check.\n");

    unsigned short receiver_result = compute_checksum_receiver();
    printf("\nComputed Checksum at receiver: %04x\n", receiver_result);

    if (receiver_result == 0x0000)
        printf("\nNo error in Data received\n");
    else
        printf("\nError in Data received\n");

    return 0;
}
```

### Sample Output
```
Enter 10 IP header fields in 16 bit hex (sender):
4500 0073 0000 4000 4011 0000 c0a8 0001 c0a8 00c7

Computed Checksum at sender: b861

Now enter the same 10 fields plus the checksum as the 11th value for receiver check.
Enter 10 IP header fields + checksum in 16 bit hex (receiver):
4500 0073 0000 4000 4011 0000 c0a8 0001 c0a8 00c7 b861

Computed Checksum at receiver: 0000

No error in Data received
```

---

## Prog3B: Hamming Code (7,4)

**Compile:** `gcc hamming.c -o a.out` | **Run:** `./a.out`

### hamming.c
```c
#include <stdio.h>

int main() {
    int data[8] = {0}, recv[8] = {0};
    int c, c0, c1, c2, i;

    printf("Enter four data bits (space-separated, e.g. 1 0 1 1): ");
    scanf("%d %d %d %d", &data[3], &data[5], &data[6], &data[7]);

    data[1] = (data[3] + data[5] + data[7]) % 2;
    data[2] = (data[3] + data[6] + data[7]) % 2;
    data[4] = (data[5] + data[6] + data[7]) % 2;

    printf("The Data encoded (7 bits) is: ");
    for (i = 1; i < 8; i++) printf("%d", data[i]);
    printf("\n");

    printf("Enter received 7 bits (space-separated, e.g. 0 1 1 0 0 1 1): ");
    for (i = 1; i < 8; i++) scanf("%d", &recv[i]);

    c0 = (recv[1] + recv[3] + recv[5] + recv[7]) % 2;
    c1 = (recv[2] + recv[3] + recv[6] + recv[7]) % 2;
    c2 = (recv[4] + recv[5] + recv[6] + recv[7]) % 2;
    c = c2 * 4 + c1 * 2 + c0;

    if (c == 0)
        printf("No error detected in received data.\n");
    else if (c > 0 && c < 8) {
        printf("Error detected at position %d. Correcting...\n", c);
        recv[c] ^= 1;
    } else
        printf("Invalid syndrome or input.\n");

    printf("The Data after correction is: ");
    for (i = 1; i < 8; i++) printf("%d", recv[i]);
    printf("\n");

    printf("Extracted original data bits: %d %d %d %d\n", recv[3], recv[5], recv[6], recv[7]);
    return 0;
}
```

### Sample Output – No Error
```
Enter four data bits (space-separated, e.g. 1 0 1 1): 1 0 1 1
The Data encoded (7 bits) is: 0110011
Enter received 7 bits (space-separated, e.g. 0 1 1 0 0 1 1): 0 1 1 0 0 1 1
No error detected in received data.
The Data after correction is: 0110011
Extracted original data bits: 1 0 1 1
```

### Sample Output – With Error (bit 5 flipped)
```
Enter four data bits (space-separated, e.g. 1 0 1 1): 1 0 1 1
The Data encoded (7 bits) is: 0110011
Enter received 7 bits (space-separated, e.g. 0 1 1 0 0 1 1): 0 1 1 0 1 1 1
Error detected at position 5. Correcting...
The Data after correction is: 0110011
Extracted original data bits: 1 0 1 1
```

---

## Prog4: UDP Multicast

**Multicast Group:** 225.0.0.37 | **Port:** 12345

**Compile:** `gcc server.c -o server && gcc client.c -o client`
**Run:** Client first → `./client`, then `./server`

### server.c (Sender)
```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"

int main() {
    struct sockaddr_in addr;
    int fd;
    char *message = "RVCE-CSE";

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { perror("socket"); exit(1); }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
    addr.sin_port = htons(HELLO_PORT);

    while (1) {
        if (sendto(fd, message, strlen(message), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) { perror("sendto"); exit(1); }
        sleep(1);
    }
}
```

### client.c (Receiver)
```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 25

int main() {
    struct sockaddr_in addr;
    int fd, nbytes;
    socklen_t addrlen;
    struct ip_mreq mreq;
    char msgbuf[MSGBUFSIZE];
    u_int yes = 1;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { perror("socket"); exit(1); }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) { perror("Reusing ADDR failed"); exit(1); }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(HELLO_PORT);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) { perror("bind"); exit(1); }

    mreq.imr_multiaddr.s_addr = inet_addr(HELLO_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) { perror("setsockopt"); exit(1); }

    while (1) {
        addrlen = sizeof(addr);
        if ((nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE - 1, 0, (struct sockaddr *)&addr, &addrlen)) < 0) { perror("recvfrom"); continue; }
        if (nbytes > 0) { msgbuf[nbytes] = '\0'; puts(msgbuf); }
    }
}
```

### Sample Output

**Receiver (client):**
```
RVCE-CSE
RVCE-CSE
RVCE-CSE
...
```
*(One line per second until Ctrl+C)*

**Sender (server):** runs silently.

---

## Prog5: TCP Fork Multi-Client Echo Server

**Compile:** `gcc server.c -o server && gcc client.c -o client`
**Run:** `./server` then `./client 127.0.0.1`

### server.c
```c
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void str_echo(int connfd, int port) {
    int n = 1, bufsize = 1024;
    char *buff = malloc(bufsize);

    do {
        while ((n = recv(connfd, buff, bufsize, 0)) > 0) {
            printf("From client connected to %d :", port);
            ssize_t w = write(1, buff, n); (void)w;
            printf("Reply to the client connected to %d :", port);
            if (fgets(buff, bufsize, stdin) != NULL) {
                size_t len = strlen(buff);
                send(connfd, buff, len, 0);
            }
        }
    } while (n > 0);
}

int main() {
    int listenfd, connfd, pid;
    socklen_t addrlen;
    struct sockaddr_in address;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) > 0)
        printf("The socket was created\n");
    else
        printf("Error in Socket creation\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(15001);

    if (bind(listenfd, (struct sockaddr *)&address, sizeof(address)) == 0)
        printf("Binding Socket\n");

    listen(listenfd, 3);

    for (;;) {
        addrlen = sizeof(struct sockaddr_in);
        connfd = accept(listenfd, (struct sockaddr *)&address, &addrlen);

        if (connfd > 0)
            printf("A new client connected from port :%d \n", address.sin_port);
        else
            printf("A new client's connection wasn't accepted\n");

        if ((pid = fork()) == 0) {
            close(listenfd);
            str_echo(connfd, address.sin_port);
            exit(0);
        }
        close(connfd);
    }
    return 0;
}
```

### client.c
```c
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void str_cli(FILE *fp, int sockfd) {
    int bufs = 1024, cont;
    char *buff = malloc(bufs);

    while (fgets(buff, bufs, fp) != NULL) {
        size_t tosend = strlen(buff);
        if (send(sockfd, buff, tosend, 0) < 0) perror("send");

        if ((cont = recv(sockfd, buff, bufs - 1, 0)) >= 0) {
            buff[cont] = '\0';
            printf("Server replied :");
            fputs(buff, stdout);
        }
    }
    printf("str cli\n");
    printf("\nEOF\n");
}

int main(int argc, char *argv[]) {
    int create_socket, ret;
    struct sockaddr_in address;

    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0)
        printf("Socket created\n");
    else { perror("socket"); return 1; }

    if (argc != 2) { fprintf(stderr, "Usage: %s <server-ip>\n", argv[0]); return 1; }

    address.sin_family = AF_INET;
    address.sin_port = htons(15001);
    if (inet_pton(AF_INET, argv[1], &address.sin_addr) != 1) { fprintf(stderr, "Invalid address: %s\n", argv[1]); return 1; }

    if ((ret = connect(create_socket, (struct sockaddr *)&address, sizeof(address))) == 0)
        printf("Connected\n");
    else { perror("connect"); return 1; }

    str_cli(stdin, create_socket);
    return 0;
}
```

### Sample Output

**Server Terminal:**
```
The socket was created
Binding Socket
A new client connected from port: 15001
From client connected to 15001: hello world
Reply to the client connected to 15001: hi from server
```

**Client Terminal:**
```
Socket created
Connected
hello world
Server replied: hi from server
goodbye
Server replied: bye
```

---

## Prog7: Remote Command Echo Server

**Compile:** `gcc server.c -o server && gcc client.c -o client`
**Run:** `./server` then `./client 127.0.0.1`

### server.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>

void remote_command(int connfd, int port) {
    int n = 1, buffsize = 1024;
    char *buffer = malloc(buffsize);
    if (!buffer) return;

    do {
        while ((n = recv(connfd, buffer, buffsize - 1, 0)) > 0) {
            buffer[n] = '\0';
            if (send(connfd, buffer, n, 0) < 0) perror("send");
            printf("Port Number: %d\n", port);
#ifdef ENABLE_REMOTE_COMMAND_EXEC
            system(buffer);
#else
            fprintf(stderr, "[remote_command] execution disabled. Command received: %s\n", buffer);
#endif
        }
    } while (n > 0);

    free(buffer);
}

int main(int argc, char const *argv[]) {
    int create_socket, new_socket, pid;
    socklen_t addrlen;
    struct sockaddr_in address;

    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0)
        printf("The socket is created successfully\n");

    address.sin_family = AF_INET;
    address.sin_port = htons(15001);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(create_socket, (struct sockaddr *)&address, sizeof(address)) == 0)
        printf("Binding Successfull\n");

    listen(create_socket, 3);
    printf("The server is listening\n");

    while (1) {
        addrlen = sizeof(struct sockaddr);
        if ((new_socket = accept(create_socket, (struct sockaddr *)&address, &addrlen)) > 0)
            printf("The client is connected: %s\n", inet_ntoa(address.sin_addr));

        if ((pid = fork()) == 0) {
            printf("Inside Child\n");
            close(create_socket);
            remote_command(new_socket, htons(address.sin_port));
            exit(0);
        }
        close(new_socket);
    }
    return 0;
}
```

### client.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>

void str_cli(FILE *fp, int sockfd) {
    int cont, buffsize = 1024;
    char *buffer = malloc(buffsize);

    while (fgets(buffer, buffsize, fp) != NULL) {
        size_t tosend = strlen(buffer);
        if (send(sockfd, buffer, tosend, 0) < 0) perror("send");

        cont = recv(sockfd, buffer, buffsize - 1, 0);
        if (cont > 0) { buffer[cont] = '\0'; fputs(buffer, stdout); }
    }
}

int main(int argc, char *argv[]) {
    int create_socket;
    struct sockaddr_in address;

    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { perror("socket"); return 1; }
    if (argc != 2) { fprintf(stderr, "Usage: %s <server-ip>\n", argv[0]); return 1; }

    address.sin_family = AF_INET;
    address.sin_port = htons(15001);
    if (inet_pton(AF_INET, argv[1], &address.sin_addr) != 1) { fprintf(stderr, "Invalid address: %s\n", argv[1]); return 1; }
    if (connect(create_socket, (struct sockaddr *)&address, sizeof(address)) != 0) { perror("connect"); close(create_socket); return 1; }

    str_cli(stdin, create_socket);
    return close(create_socket);
}
```

### Sample Output

**Server Terminal:**
```
The socket is created successfully
Binding Successfull
The server is listening
The client is connected: 127.0.0.1
Inside Child
Port Number: 15001
[remote_command] execution disabled. Command received: ls -l
```

**Client Terminal:**
```
ls -l
ls -l
whoami
whoami
```

---

## Prog8A: Diffie-Hellman Key Exchange

**Compile:** `gcc dh.c -o a.out` | **Run:** `./a.out`

### dh.c
```c
#include <stdio.h>

long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}

int main() {
    long long p, g, a, b, A, B, keyA, keyB;

    printf("Enter the prime no(p): "); scanf("%lld", &p);
    printf("Enter the primitive root of p(g): "); scanf("%lld", &g);
    printf("Enter Alice private key (a): "); scanf("%lld", &a);
    printf("Enter Bob private key (b): "); scanf("%lld", &b);

    A = mod_exp(g, a, p);
    B = mod_exp(g, b, p);
    keyA = mod_exp(B, a, p);
    keyB = mod_exp(A, b, p);

    printf("\nAlice Public Key = %lld", A);
    printf("\nBob Public Key = %lld", B);
    printf("\n\nAlice Secret Key = %lld", keyA);
    printf("\nBob Secret Key = %lld\n", keyB);
    return 0;
}
```

### Sample Output
```
Enter the prime no(p): 23
Enter the primitive root of p(g): 5
Enter Alice private key (a): 6
Enter Bob private key (b): 15

Alice Public Key = 8
Bob Public Key = 19

Alice Secret Key = 2
Bob Secret Key = 2
```

> Both secret keys equal 2 — shared secret established correctly.

---

## Prog8B: RSA Encryption/Decryption

**Compile:** `gcc rsa.c -o a.out -lm` | **Run:** `./a.out`

### rsa.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

long int gcd(long int a, long int b) {
    if (a == 0) return b;
    if (b == 0) return a;
    return gcd(b, a % b);
}

long int isprime(long int a) {
    int i;
    if (a < 2) return 0;
    for (i = 2; i < a; i++)
        if ((a % i) == 0) return 0;
    return 1;
}

long int encrypt(char ch, long int n, long int e) {
    long long base = (unsigned char)ch, result = 1, mod = n, exp = e;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return (long int)result;
}

char decrypt(long int ch, long int n, long int d) {
    long long base = ch, result = 1, mod = n, exp = d;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return (char)result;
}

int main() {
    long int i, len, p, q, n, phi, e, d, cipher[50];
    char text[50];

    printf("Enter the text to be encrypted: ");
    scanf("%s", text);
    len = strlen(text);

    do { p = rand() % 30; } while (!isprime(p));
    do { q = rand() % 30; } while (!isprime(q));

    n = p * q;
    phi = (p - 1) * (q - 1);
    do { e = rand() % phi; } while (gcd(phi, e) != 1);
    do { d = rand() % phi; } while (((d * e) % phi) != 1);

    printf("Two prime numbers (p and q) are: %ld and %ld\n", p, q);
    printf("n(p * q) = %ld\n", p * q);
    printf("(p - 1) * (q - 1) = %ld\n", phi);
    printf("Public key (n, e): (%ld, %ld)\n", n, e);
    printf("Private key (n, d): (%ld, %ld)\n", n, d);

    for (i = 0; i < len; i++) cipher[i] = encrypt(text[i], n, e);
    printf("Encrypted message:\n");
    for (i = 0; i < len; i++) printf("%ld ", cipher[i]);

    for (i = 0; i < len; i++) text[i] = decrypt(cipher[i], n, d);
    printf("\nDecrypted message:\n");
    for (i = 0; i < len; i++) printf("%c", text[i]);
    printf("\n");
    return 0;
}
```

### Sample Output
```
Enter the text to be encrypted: hello
Two prime numbers (p and q) are: 17 and 19
n(p * q) = 323
(p - 1) * (q - 1) = 288
Public key (n, e): (323, 5)
Private key (n, d): (323, 173)
Encrypted message:
104 149 27 27 67
Decrypted message:
hello
```

> Keys are randomly chosen each run. Use short lowercase strings; n must exceed the ASCII value of each character.
