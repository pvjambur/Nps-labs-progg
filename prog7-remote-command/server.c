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
