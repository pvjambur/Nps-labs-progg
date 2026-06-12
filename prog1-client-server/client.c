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
