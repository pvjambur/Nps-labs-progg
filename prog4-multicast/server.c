/* Program 4: UDP Multicast – Sender (Server)
   Broadcasts "RVCE-CSE" to multicast group 225.0.0.37 every second */

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

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    { perror("socket"); exit(1); }

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
