/* Program 4: UDP Multicast – Receiver (Client)
   Joins multicast group 225.0.0.37 and prints every message received */

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
    struct sockaddr_in addr;
    struct sockaddr_in sender_addr;
    socklen_t addrlen;
    struct ip_mreq mreq;
    char msgbuf[MSGBUFSIZE];
    int yes = 1;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    { perror("socket"); exit(1); }

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
