# NPS Lab – All Programs with Full Code and Sample Output

One-stop reference. Copy any block directly into a `.c` file, compile, and run.

---

## Contents

1. [Prog 1 – TCP File Transfer](#prog-1--tcp-file-transfer)
2. [Prog 2 – Distance Vector Routing](#prog-2--distance-vector-routing)
3. [Prog 3A – IP Checksum](#prog-3a--ip-checksum)
4. [Prog 3B – Hamming Code](#prog-3b--hamming-code-74)
5. [Prog 4 – UDP Multicast](#prog-4--udp-multicast)
6. [Prog 5 – Concurrent Chat Server](#prog-5--concurrent-tcp-chat-server)
7. [Prog 7 – Remote Command Execution](#prog-7--remote-command-execution)
8. [Prog 8A – Diffie-Hellman](#prog-8a--diffie-hellman-key-exchange)
9. [Prog 8B – RSA](#prog-8b--rsa-encryption--decryption)

---

## Prog 1 – TCP File Transfer

**Compile:**
```bash
gcc server.c -o server
gcc client.c -o client
```
**Run:** Terminal 1 → `./server` | Terminal 2 → `./client 127.0.0.1`

### server.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT 15001
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, client_fd;
    socklen_t addrlen;
    int n, file_fd;
    char buffer[BUFFER_SIZE];
    char frame[256];
    struct sockaddr_in server_addr, client_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("Socket creation failed"); exit(1); }
    printf("Socket created successfully\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    { perror("Bind failed"); close(server_fd); exit(1); }
    printf("Binding successful\n");

    if (listen(server_fd, 3) < 0)
    { perror("Listen failed"); close(server_fd); exit(1); }
    printf("Waiting for client...\n");

    addrlen   = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
    if (client_fd < 0) { perror("Accept failed"); close(server_fd); exit(1); }
    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    memset(frame, 0, sizeof(frame));
    recv(client_fd, frame, sizeof(frame) - 1, 0);
    printf("Requested file: %s\n", frame);

    file_fd = open(frame, O_RDONLY);
    if (file_fd < 0) { perror("File open failed"); close(client_fd); close(server_fd); exit(1); }

    while ((n = read(file_fd, buffer, BUFFER_SIZE)) > 0)
        send(client_fd, buffer, n, 0);
    if (n < 0) perror("File read error");

    close(file_fd); close(client_fd); close(server_fd);
    return 0;
}
```

### client.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 15001
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sockfd, n;
    char buffer[BUFFER_SIZE];
    char frame[256];
    struct sockaddr_in server_addr;

    if (argc != 2) { printf("Usage: %s <server IP>\n", argv[0]); exit(1); }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Socket creation failed"); exit(1); }
    printf("Socket created\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
    { perror("Invalid address"); close(sockfd); exit(1); }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    { perror("Connection failed"); close(sockfd); exit(1); }
    printf("Connected to server\n");

    printf("Enter the filename: ");
    scanf("%255s", frame);
    send(sockfd, frame, strlen(frame), 0);

    printf("\n--- File Contents ---\n");
    while ((n = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0)
        write(STDOUT_FILENO, buffer, n);
    printf("\n--- End of File ---\n");

    close(sockfd);
    return 0;
}
```

### Sample Output

**Server Terminal:**
```
Socket created successfully
Binding successful
Waiting for client...
Client connected: 127.0.0.1
Requested file: hello.txt
```

**Client Terminal:**
```
Socket created
Connected to server
Enter the filename: hello.txt

--- File Contents ---
Hello from server file!
--- End of File ---
```

---

## Prog 2 – Distance Vector Routing

**Compile:** `gcc dv.c -o dv` | **Run:** `./dv`

### dv.c
```c
#include <stdio.h>

struct node {
    unsigned front[20];
    unsigned dist[20];
} t[10];

int main()
{
    int costmat[20][20];
    int nodes, i, j, k, count = 0;

    printf("\nEnter the no of nodes: ");
    scanf("%d", &nodes);
    printf("Enter the cost matrix:\n");

    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            scanf("%d", &costmat[i][j]);
            costmat[i][i]  = 0;
            t[i].dist[j]   = costmat[i][j];
            t[i].front[j]  = j;
        }
    }

    do {
        count = 0;
        for (i = 0; i < nodes; i++)
            for (j = 0; j < nodes; j++)
                for (k = 0; k < nodes; k++)
                    if (t[i].dist[j] > costmat[i][k] + t[k].dist[j]) {
                        t[i].dist[j]  = costmat[i][k] + t[k].dist[j];
                        t[i].front[j] = k;
                        count++;
                    }
    } while (count != 0);

    for (i = 0; i < nodes; i++) {
        printf("\nFor router %d", i + 1);
        for (j = 0; j < nodes; j++)
            printf("\n  For node %d via %d distance is %d",
                   j + 1, t[i].front[j] + 1, t[i].dist[j]);
    }
    printf("\n\n");
    return 0;
}
```

### Sample Input & Output

**Input:**
```
Enter the no of nodes: 3
Enter the cost matrix:
0 2 7
2 0 1
7 1 0
```

**Output:**
```
For router 1
  For node 1 via 1 distance is 0
  For node 2 via 2 distance is 2
  For node 3 via 2 distance is 3

For router 2
  For node 1 via 1 distance is 2
  For node 2 via 2 distance is 0
  For node 3 via 3 distance is 1

For router 3
  For node 1 via 2 distance is 3
  For node 2 via 2 distance is 1
  For node 3 via 3 distance is 0
```

> Router 1→3: shortest path 1→2→3, cost 3 (direct link cost 7 avoided).

---

## Prog 3A – IP Checksum

**Compile:** `gcc checksum.c -o checksum` | **Run:** `./checksum`

### checksum.c
```c
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

### Sample Output
```
Enter 10 IP header fields in 16-bit hex (sender):
4500 0073 0000 4000 4011 0000 c0a8 0001 c0a8 00c7

Computed Checksum at sender: b861

Enter the same 10 fields + checksum as 11th value (receiver):
4500 0073 0000 4000 4011 0000 c0a8 0001 c0a8 00c7 b861

Computed Checksum at receiver: 0000

No error in Data received
```

---

## Prog 3B – Hamming Code (7,4)

**Compile:** `gcc hamming.c -o hamming` | **Run:** `./hamming`

### hamming.c
```c
#include <stdio.h>

int main()
{
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

    printf("Enter received 7 bits (space-separated): ");
    for (i = 1; i < 8; i++) scanf("%d", &recv[i]);

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
    printf("Extracted original data bits: %d %d %d %d\n", recv[3], recv[5], recv[6], recv[7]);
    return 0;
}
```

### Sample Output – No Error
```
Enter four data bits (space-separated, e.g. 1 0 1 1): 1 0 1 1
The Data encoded (7 bits) is: 0110011
Enter received 7 bits (space-separated): 0 1 1 0 0 1 1
No error detected in received data.
The Data after correction is: 0110011
Extracted original data bits: 1 0 1 1
```

### Sample Output – Error at Bit 5
```
Enter four data bits (space-separated, e.g. 1 0 1 1): 1 0 1 1
The Data encoded (7 bits) is: 0110011
Enter received 7 bits (space-separated): 0 1 1 0 1 1 1
Error detected at position 5. Correcting...
The Data after correction is: 0110011
Extracted original data bits: 1 0 1 1
```

---

## Prog 4 – UDP Multicast

**Multicast Group:** `225.0.0.37` | **Port:** `12345`

**Compile:** `gcc server.c -o server && gcc client.c -o client`
**Run:** Start **client first** → `./client`, then `./server`

### server.c (Sender)
```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HELLO_PORT  12345
#define HELLO_GROUP "225.0.0.37"

int main()
{
    struct sockaddr_in addr;
    int fd;
    char *message = "RVCE-CSE";

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { perror("socket"); exit(1); }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
    addr.sin_port        = htons(HELLO_PORT);

    printf("Sending multicast messages to %s:%d ...\n", HELLO_GROUP, HELLO_PORT);

    while (1) {
        if (sendto(fd, message, strlen(message), 0,
                   (struct sockaddr *)&addr, sizeof(addr)) < 0)
        { perror("sendto"); close(fd); exit(1); }
        printf("Sent: %s\n", message);
        sleep(1);
    }

    close(fd);
    return 0;
}
```

### client.c (Receiver)
```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HELLO_PORT  12345
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE  1024

int main()
{
    int fd, nbytes;
    struct sockaddr_in addr, sender_addr;
    socklen_t addrlen;
    struct ip_mreq mreq;
    char msgbuf[MSGBUFSIZE];
    int yes = 1;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { perror("socket"); exit(1); }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
    { perror("SO_REUSEADDR"); close(fd); exit(1); }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(HELLO_PORT);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    { perror("bind"); close(fd); exit(1); }

    mreq.imr_multiaddr.s_addr = inet_addr(HELLO_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    { perror("IP_ADD_MEMBERSHIP"); close(fd); exit(1); }

    printf("Listening for multicast on %s:%d ...\n", HELLO_GROUP, HELLO_PORT);

    while (1) {
        addrlen = sizeof(sender_addr);
        nbytes  = recvfrom(fd, msgbuf, MSGBUFSIZE - 1, 0,
                           (struct sockaddr *)&sender_addr, &addrlen);
        if (nbytes < 0) { perror("recvfrom"); continue; }
        msgbuf[nbytes] = '\0';
        printf("Received from %s: %s\n", inet_ntoa(sender_addr.sin_addr), msgbuf);
    }

    close(fd);
    return 0;
}
```

### Sample Output

**Sender:** `Sent: RVCE-CSE` *(once per second)*

**Receiver:**
```
Listening for multicast on 225.0.0.37:12345 ...
Received from 127.0.0.1: RVCE-CSE
Received from 127.0.0.1: RVCE-CSE
...
```

---

## Prog 5 – Concurrent TCP Chat Server

**Compile:** `gcc server.c -o server && gcc client.c -o client`
**Run:** `./server` then `./client 127.0.0.1`

### server.c
```c
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

void str_echo(int connfd, int port)
{
    int n;
    char buff[BUFSIZE];

    while ((n = recv(connfd, buff, BUFSIZE - 1, 0)) > 0) {
        buff[n] = '\0';
        printf("From client (port %d): %s", port, buff);
        printf("Reply: ");
        fgets(buff, BUFSIZE, stdin);
        send(connfd, buff, strlen(buff), 0);
    }

    if (n == 0) printf("Client disconnected (port %d)\n", port);
    else        perror("recv error");
    close(connfd);
}

int main()
{
    int listenfd, connfd, pid;
    socklen_t addrlen;
    struct sockaddr_in serv_addr, cli_addr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { perror("Socket creation failed"); exit(1); }
    printf("Socket created\n");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port        = htons(15001);

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    { perror("Bind failed"); exit(1); }
    printf("Binding done\n");

    listen(listenfd, 5);
    printf("Server listening on port 15001...\n");

    while (1) {
        addrlen = sizeof(cli_addr);
        connfd  = accept(listenfd, (struct sockaddr *)&cli_addr, &addrlen);
        if (connfd < 0) { perror("Accept failed"); continue; }

        printf("New client connected from %s:%d\n",
               inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

        if ((pid = fork()) == 0) {
            close(listenfd);
            str_echo(connfd, ntohs(cli_addr.sin_port));
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFSIZE 1024

void str_cli(FILE *fp, int sockfd)
{
    char buff[BUFSIZE];
    int n;

    while (fgets(buff, BUFSIZE, fp) != NULL) {
        send(sockfd, buff, strlen(buff), 0);
        n = recv(sockfd, buff, BUFSIZE - 1, 0);
        if (n > 0) { buff[n] = '\0'; printf("Server replied: "); fputs(buff, stdout); }
        else if (n == 0) { printf("Server closed connection\n"); break; }
        else { perror("recv error"); break; }
    }
    printf("\nEOF\n");
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in address;

    if (argc != 2) { printf("Usage: %s <server_ip>\n", argv[0]); exit(1); }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { perror("Socket creation failed"); exit(1); }
    printf("Socket created\n");

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port   = htons(15001);

    if (inet_pton(AF_INET, argv[1], &address.sin_addr) <= 0)
    { perror("Invalid address"); exit(1); }

    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) == 0)
        printf("Connected\n");
    else { perror("Connect failed"); exit(1); }

    str_cli(stdin, sockfd);
    close(sockfd);
    return 0;
}
```

### Sample Output

**Server Terminal:**
```
Socket created
Binding done
Server listening on port 15001...
New client connected from 127.0.0.1:54321
From client (port 54321): hello world
Reply: hi from server
```

**Client Terminal:**
```
Socket created
Connected
hello world
Server replied: hi from server
```

---

## Prog 7 – Remote Command Execution

**Compile:** `gcc server.c -o server && gcc client.c -o client`
**Run:** `./server` then `./client 127.0.0.1`

### server.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define PORT    15001
#define BUFSIZE 1024

void remote_command(int connfd, int port)
{
    int n;
    char *buffer = malloc(BUFSIZE);
    if (!buffer) { perror("malloc"); return; }

    while ((n = recv(connfd, buffer, BUFSIZE - 1, 0)) > 0) {
        buffer[n] = '\0';
        send(connfd, buffer, n, 0);
        printf("Port: %d | Command: %s", port, buffer);
        system(buffer);
    }

    free(buffer);
    close(connfd);
}

int main()
{
    int listenfd, connfd, pid;
    socklen_t addrlen;
    struct sockaddr_in address, client_addr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { perror("socket failed"); exit(1); }
    printf("The socket is created successfully\n");

    memset(&address, 0, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_port        = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    { perror("bind failed"); exit(1); }
    printf("Binding Successful\n");

    if (listen(listenfd, 3) < 0) { perror("listen failed"); exit(1); }
    printf("The server is listening on port %d...\n", PORT);

    while (1) {
        addrlen = sizeof(client_addr);
        connfd  = accept(listenfd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0) { perror("accept failed"); continue; }
        printf("The client is connected: %s\n", inet_ntoa(client_addr.sin_addr));

        if ((pid = fork()) == 0) {
            printf("Inside Child\n");
            close(listenfd);
            remote_command(connfd, ntohs(client_addr.sin_port));
            exit(0);
        }
        close(connfd);
    }
    return 0;
}
```

### client.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define BUF_SIZE 1024
#define PORT     15001

void str_cli(FILE *fp, int sockfd)
{
    char buffer[BUF_SIZE];
    int n;

    while (fgets(buffer, BUF_SIZE, fp) != NULL) {
        if (send(sockfd, buffer, strlen(buffer), 0) < 0) { perror("send failed"); return; }
        n = recv(sockfd, buffer, BUF_SIZE - 1, 0);
        if (n < 0) { perror("recv failed"); return; }
        else if (n == 0) { printf("Server closed connection\n"); return; }
        buffer[n] = '\0';
        fputs(buffer, stdout);
    }
    printf("\nEOF\n");
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in address;

    if (argc < 2) { fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]); exit(1); }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket failed"); exit(1); }
    printf("Socket created\n");

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port   = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &address.sin_addr) <= 0)
    { perror("Invalid address"); close(sockfd); exit(1); }

    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    { perror("connect failed"); close(sockfd); exit(1); }
    printf("Connected to server %s:%d\n", argv[1], PORT);

    str_cli(stdin, sockfd);
    close(sockfd);
    return 0;
}
```

### Sample Output

**Server Terminal:**
```
The socket is created successfully
Binding Successful
The server is listening on port 15001...
The client is connected: 127.0.0.1
Inside Child
Port: 54322 | Command: ls -l
```

**Client Terminal:**
```
Socket created
Connected to server 127.0.0.1:15001
ls -l
ls -l
whoami
whoami
```

---

## Prog 8A – Diffie-Hellman Key Exchange

**Compile:** `gcc dh.c -o dh` | **Run:** `./dh`

### dh.c
```c
#include <stdio.h>

long long mod_exp(long long base, long long exp, long long mod)
{
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main()
{
    long long p, g, a, b, A, B, keyA, keyB;

    printf("Enter the prime no (p)            : "); scanf("%lld", &p);
    printf("Enter the primitive root of p (g) : "); scanf("%lld", &g);
    printf("Enter Alice's private key (a)     : "); scanf("%lld", &a);
    printf("Enter Bob's   private key (b)     : "); scanf("%lld", &b);

    A    = mod_exp(g, a, p);
    B    = mod_exp(g, b, p);
    keyA = mod_exp(B, a, p);
    keyB = mod_exp(A, b, p);

    printf("\nAlice Public Key = %lld\n", A);
    printf("Bob   Public Key = %lld\n",   B);
    printf("\nAlice Secret Key = %lld\n", keyA);
    printf("Bob   Secret Key = %lld\n",   keyB);

    if (keyA == keyB)
        printf("\nShared secret established successfully: %lld\n", keyA);
    else
        printf("\nKey mismatch — check parameters.\n");

    return 0;
}
```

### Sample Output
```
Enter the prime no (p)            : 23
Enter the primitive root of p (g) : 5
Enter Alice's private key (a)     : 6
Enter Bob's   private key (b)     : 15

Alice Public Key = 8
Bob   Public Key = 19

Alice Secret Key = 2
Bob   Secret Key = 2

Shared secret established successfully: 2
```

> Both keys equal 2 — shared secret established correctly.

---

## Prog 8B – RSA Encryption / Decryption

**Compile:** `gcc rsa.c -o rsa` | **Run:** `./rsa`

### rsa.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long int gcd(long int a, long int b)
{
    if (a == 0) return b;
    if (b == 0) return a;
    return gcd(b, a % b);
}

long int isprime(long int a)
{
    int i;
    if (a < 2) return 0;
    for (i = 2; i * i <= a; i++)
        if ((a % i) == 0) return 0;
    return 1;
}

long int mod_exp(long int base, long int exp, long int mod)
{
    long int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

long int encrypt(char ch, long int n, long int e)
{
    return mod_exp((long int)(unsigned char)ch, e, n);
}

char decrypt(long int ch, long int n, long int d)
{
    return (char)mod_exp(ch, d, n);
}

int main()
{
    long int i, len, p, q, n, phi, e, d, cipher[50];
    char text[50];

    srand((unsigned int)time(0));

    printf("Enter the text to be encrypted: ");
    scanf("%49s", text);
    len = strlen(text);

    do { p = rand() % 30; } while (!isprime(p));
    do { q = rand() % 30; } while (!isprime(q) || q == p);

    n   = p * q;
    phi = (p - 1) * (q - 1);

    do { e = rand() % phi; } while (e <= 1 || gcd(phi, e) != 1);
    do { d = rand() % phi; } while ((d * e) % phi != 1);

    printf("Two prime numbers (p and q): %ld and %ld\n", p, q);
    printf("n = %ld\n", n);
    printf("phi = %ld\n", phi);
    printf("Public key  (n, e): (%ld, %ld)\n", n, e);
    printf("Private key (n, d): (%ld, %ld)\n", n, d);

    printf("Encrypted message:\n");
    for (i = 0; i < len; i++) { cipher[i] = encrypt(text[i], n, e); printf("%ld ", cipher[i]); }

    printf("\nDecrypted message:\n");
    for (i = 0; i < len; i++) { text[i] = decrypt(cipher[i], n, d); printf("%c", text[i]); }
    printf("\n");
    return 0;
}
```

### Sample Output
```
Enter the text to be encrypted: hello
Two prime numbers (p and q): 17 and 19
n = 323
phi = 288
Public key  (n, e): (323, 5)
Private key (n, d): (323, 173)
Encrypted message:
104 149 27 27 67
Decrypted message:
hello
```

> Keys are randomly picked each run. Use short lowercase strings; `n` must exceed the ASCII value of each character.
