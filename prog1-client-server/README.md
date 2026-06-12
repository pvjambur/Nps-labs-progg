# Program 1 – TCP Client-Server File Transfer

A TCP socket program where the client requests a filename and the server sends the file contents back over the connection.

---

## Files

| File | Role |
|------|------|
| `server.c` | Binds to port 15001, waits for a client, receives a filename, and streams the file |
| `client.c` | Connects to server, sends a filename, receives and prints file contents |

---

## Compile

```bash
gcc server.c -o server
gcc client.c -o client
```

---

## Run

Open **two terminals**:

**Terminal 1 – Server**
```bash
./server
```

**Terminal 2 – Client**
```bash
./client 127.0.0.1
```

> Replace `127.0.0.1` with the server's IP address if running on different machines.

---

## How It Works

1. Server creates a TCP socket, binds to port 15001, and waits with `accept()`.
2. Client connects and is prompted to enter a filename.
3. Client sends the filename; server opens the file and sends its contents in chunks.
4. Client writes received data to stdout.

---

## server.c

```c
/* Program 1: TCP File Transfer Server
   Waits for client, receives filename, sends file contents back */

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

    addrlen  = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
    if (client_fd < 0) { perror("Accept failed"); close(server_fd); exit(1); }
    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    memset(frame, 0, sizeof(frame));
    recv(client_fd, frame, sizeof(frame) - 1, 0);
    printf("Requested file: %s\n", frame);

    file_fd = open(frame, O_RDONLY);
    if (file_fd < 0)
    {
        perror("File open failed");
        close(client_fd);
        close(server_fd);
        exit(1);
    }

    while ((n = read(file_fd, buffer, BUFFER_SIZE)) > 0)
        send(client_fd, buffer, n, 0);

    if (n < 0) perror("File read error");

    close(file_fd);
    close(client_fd);
    close(server_fd);
    return 0;
}
```

---

## client.c

```c
/* Program 1: TCP File Transfer Client
   Connects to server, sends filename, receives and prints file contents */

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

    if (argc != 2)
    { printf("Usage: %s <server IP>\n", argv[0]); exit(1); }

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

    if (n < 0) perror("Receive error");

    close(sockfd);
    return 0;
}
```
