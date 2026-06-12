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
