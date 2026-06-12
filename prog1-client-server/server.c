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
