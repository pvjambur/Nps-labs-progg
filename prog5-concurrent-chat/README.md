# Program 5 – Concurrent TCP Chat Server (Fork-based)

A TCP chat server that handles multiple clients concurrently. Each accepted connection spawns a child process (`fork`) to serve that client independently. The server operator types replies manually.

---

## Files

| File | Role |
|------|------|
| `server.c` | Forks a child per client; reads client message, server types a reply |
| `client.c` | Connects to server; sends typed lines and prints server replies |

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

> Open additional client terminals to test concurrent connections.

---

## server.c

```c
/* Program 5: Concurrent TCP Chat Server (fork-based)
   Each client gets its own child process; server types replies manually */

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

    if (n == 0)
        printf("Client disconnected (port %d)\n", port);
    else
        perror("recv error");

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

---

## client.c

```c
/* Program 5: Concurrent TCP Chat Client
   Sends typed lines to server and prints the server's reply */

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
        if (n > 0) {
            buff[n] = '\0';
            printf("Server replied: ");
            fputs(buff, stdout);
        } else if (n == 0) {
            printf("Server closed connection\n");
            break;
        } else {
            perror("recv error");
            break;
        }
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
