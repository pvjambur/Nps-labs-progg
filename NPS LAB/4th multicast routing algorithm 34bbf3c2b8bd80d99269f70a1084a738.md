# 4th multicast routing algorithm

- sender/server
    
    ```c
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    
    #define HELLO_PORT 12345
    #define HELLO_GROUP "225.0.0.37"
    
    int main() {
        struct sockaddr_in addr;
        int fd;
        char *message = "RVCE-CSE";
    
        // 1. Create UDP socket
        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("socket");
            exit(1);
        }
    
        // 2. Set destination address (multicast group)
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
        addr.sin_port = htons(HELLO_PORT);
    
        printf("Sending multicast messages...\n");
    
        // 3. Send loop
        while (1) {
            if (sendto(fd, message, strlen(message), 0,
                       (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("sendto");
                close(fd);
                exit(1);
            }
    
            printf("Sent: %s\n", message);
            sleep(1);
        }
    
        close(fd);
        return 0;
    }
    ```
    
- client
    
    ```c
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    
    #define HELLO_PORT 12345
    #define HELLO_GROUP "225.0.0.37"
    #define MSGBUFSIZE 1024   // increased buffer size
    
    int main() {
        int fd, nbytes;
        struct sockaddr_in addr;         // for binding
        struct sockaddr_in sender_addr;  // for receiving sender info
        socklen_t addrlen;
        struct ip_mreq mreq;
        char msgbuf[MSGBUFSIZE];
        int yes = 1;
    
        // 1. Create socket
        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("socket");
            exit(1);
        }
    
        // 2. Allow multiple sockets to use same PORT
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
            perror("SO_REUSEADDR");
            close(fd);
            exit(1);
        }
    
        // 3. Bind to the proper port number with the IP address specified as INADDR_ANY
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(HELLO_PORT);
    
        if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("bind");
            close(fd);
            exit(1);
        }
    
        // 4. Join the multicast group
        mreq.imr_multiaddr.s_addr = inet_addr(HELLO_GROUP);
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    
        if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
            perror("IP_ADD_MEMBERSHIP");
            close(fd);
            exit(1);
        }
    
        printf("Listening for multicast messages on %s:%d...\n", HELLO_GROUP, HELLO_PORT);
    
        // 5. Receive loop
        while (1) {
            addrlen = sizeof(sender_addr);
    
            nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE - 1, 0,
                              (struct sockaddr *)&sender_addr, &addrlen);
    
            if (nbytes < 0) {
                perror("recvfrom");
                continue;
            }
    
            // Fix: ensure null-termination
            msgbuf[nbytes] = '\0';
    
            printf("Received from %s: %s\n",
                   inet_ntoa(sender_addr.sin_addr),
                   msgbuf);
        }
    
        // Cleanup (not reached due to infinite loop)
        close(fd);
        return 0;
    }
    ```