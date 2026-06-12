7.  remote command execution using socket system
    
    ```c
    client.c
    
    #include<stdio.h>
    #include<stdlib.h>
    #include<unistd.h>
    #include<arpa/inet.h>
    #include<netinet/in.h>
    #include<sys/socket.h>
    #include<string.h>
    
    #define BUF_SIZE 1024
    #define PORT 15001
    
    void str_cli(FILE *fp, int sockfd)
    {
        char buffer[BUF_SIZE];
        int n;
    
        while (fgets(buffer, BUF_SIZE, fp) != NULL)
        {
            // Send actual data length
            if (send(sockfd, buffer, strlen(buffer), 0) < 0)
            {
                perror("send failed");
                return;
            }
    
            // Receive response
            n = recv(sockfd, buffer, BUF_SIZE - 1, 0);
            if (n < 0)
            {
                perror("recv failed");
                return;
            }
            else if (n == 0)
            {
                printf("Server closed connection\n");
                return;
            }
    
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
        {
            fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    
        // Create socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
    
        printf("Socket created\n");
    
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(PORT);
    
        if (inet_pton(AF_INET, argv[1], &address.sin_addr) <= 0)
        {
            perror("Invalid address");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    
        // Connect
        if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0)
        {
            perror("connect failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    
        printf("Connected to server %s:%d\n", argv[1], PORT);
    
        // Run client
        str_cli(stdin, sockfd);
    
        close(sockfd);
        return 0;
    }
    ```
    
    ```c
    server.c
    
    #include<stdio.h>
    #include<stdlib.h>
    #include<unistd.h>
    #include<fcntl.h>
    #include<arpa/inet.h>
    #include<netinet/in.h>
    #include<sys/types.h>
    #include<sys/stat.h>
    #include<sys/socket.h>
    #include<string.h>
    
    void remote_command(int connfd, int port){
    	int n = 1, buffsize = 1024;
    	char *buffer = malloc(buffsize);
    
    	if (buffer == NULL) {
    		perror("malloc failed");
    		return;
    	}
    
    	do{
    		while((n = recv(connfd, buffer, buffsize - 1, 0)) > 0){
    			buffer[n] = '\0';   // ✅ null terminate
    
    			send(connfd, buffer, n, 0);  // already correct
    
    			printf("Port Number: %d\n", port);
    
    			system(buffer);   // (kept as you requested)
    		}
    	}while(n > 0);
    
    	free(buffer);   // ✅ free memory
    	close(connfd);  // ✅ close connection
    }
    
    int main(int argc, char const *argv[])
    {
    	int create_socket, new_socket, pid;
    	socklen_t addrlen;
    	struct sockaddr_in address, client_addr;
    
    	if((create_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    		perror("socket failed");
    		exit(1);
    	}
    	printf("The socket is created successfully\n");
    
    	memset(&address, 0, sizeof(address));  // ✅ initialize
    
    	address.sin_family = AF_INET;
    	address.sin_port = htons(15001);
    	address.sin_addr.s_addr = INADDR_ANY;
    
    	if(bind(create_socket, (struct sockaddr*)&address, sizeof(address)) < 0){
    		perror("bind failed");
    		exit(1);
    	}
    	printf("Binding Successful\n");
    
    	if(listen(create_socket, 3) < 0){
    		perror("listen failed");
    		exit(1);
    	}
    
    	printf("The server is listening\n");
    
    	while(1){
    		addrlen = sizeof(client_addr);
    
    		if ((new_socket = accept(create_socket, (struct sockaddr*)&client_addr, &addrlen)) < 0){
    			perror("accept failed");
    			continue;
    		}
    
    		printf("The client is connected: %s\n", inet_ntoa(client_addr.sin_addr));
    
    		if((pid = fork()) == 0){
    			printf("Inside Child\n");
    			close(create_socket);
    
    			// ✅ correct port printing
    			remote_command(new_socket, ntohs(client_addr.sin_port));
    
    			exit(0);
    		}
    
    		close(new_socket);  // ✅ parent closes
    	}
    
    	return 0;
    }
    ```
    
    ```latex
    
    ```