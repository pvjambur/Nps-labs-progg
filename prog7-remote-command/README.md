# Program 7 – Remote Command Execution via Socket

A TCP server that receives command strings from the client, echoes them back, and executes them using `system()`. Demonstrates how remote shell execution works at the socket level.

---

## Files

| File | Role |
|------|------|
| `server.c` | Receives a command, echoes it to client, executes it on the server |
| `client.c` | Sends typed commands and prints the server's echo response |

---

## Compile

```bash
gcc server.c -o server
gcc client.c -o client
```

---

## Run

**Terminal 1 – Server**
```bash
./server
```

**Terminal 2 – Client**
```bash
./client 127.0.0.1
```

---

## server.c

```c
/* Program 7: Remote Command Execution Server
   Receives command strings from client, echoes them back, and executes them */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define PORT     15001
#define BUFSIZE  1024

void remote_command(int connfd, int port)
{
    int n;
    char *buffer = malloc(BUFSIZE);

    if (buffer == NULL) { perror("malloc failed"); return; }

    while ((n = recv(connfd, buffer, BUFSIZE - 1, 0)) > 0) {
        buffer[n] = '\0';
        send(connfd, buffer, n, 0);          /* echo command back to client */
        printf("Port: %d | Command: %s", port, buffer);
        system(buffer);                       /* execute on server */
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

---

## client.c

```c
/* Program 7: Remote Command Execution Client
   Sends typed commands and prints the server's echo response */

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
        if (send(sockfd, buffer, strlen(buffer), 0) < 0)
        { perror("send failed"); return; }

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

    if (argc < 2)
    { fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]); exit(EXIT_FAILURE); }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket failed"); exit(EXIT_FAILURE); }
    printf("Socket created\n");

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port   = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &address.sin_addr) <= 0)
    { perror("Invalid address"); close(sockfd); exit(EXIT_FAILURE); }

    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    { perror("connect failed"); close(sockfd); exit(EXIT_FAILURE); }
    printf("Connected to server %s:%d\n", argv[1], PORT);

    str_cli(stdin, sockfd);
    close(sockfd);
    return 0;
}
```
