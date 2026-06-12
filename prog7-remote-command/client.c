/* Program 7: Remote Command Execution Client
   Sends typed commands to server and prints the echoed response */

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
