// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
#define MAXLINE 1024
#define PORT2	 8081

// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];	
	char *hello = "SYN";
	int ret;
	char msg[MAXLINE];
	struct sockaddr_in	 servaddr;

	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	
	
	int n, len;
	int n2, len2;
	
	sendto(sockfd, (const char *)hello, strlen(hello),
			MSG_CONFIRM, (const struct sockaddr *) &servaddr,
				sizeof(servaddr));
	printf("%s message sent.\n",hello);
			
	n = recvfrom(sockfd, (char *)buffer, MAXLINE,
					MSG_WAITALL, (struct sockaddr *) &servaddr,
					&len);
	buffer[n] = '\0';		
	printf("Server : %s\n", buffer);
	ret = strncmp(buffer,"SYN-ACK",7);
	if(ret==0){
		hello= "ACK";
		sendto(sockfd, (const char *)hello, strlen(hello),
		MSG_CONFIRM, (const struct sockaddr *) &servaddr,
			sizeof(servaddr));
		printf("%s message sent.\n",hello);
			
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
					MSG_WAITALL, (struct sockaddr *) &servaddr,
					&len);
		buffer[n] = '\0';		
		printf("Server : %s\n", buffer);
		int sockfd2;
		char buffer2[MAXLINE];	
		char *hello2 = "SYN";
	
		struct sockaddr_in	 servaddr2;

		// Creating socket file descriptor
		if ( (sockfd2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
			perror("socket creation failed");
			exit(EXIT_FAILURE);
		}

		memset(&servaddr2, 0, sizeof(servaddr2));
	
		// Filling server information
		servaddr2.sin_family = AF_INET;
		servaddr2.sin_port = htons(n);
		servaddr2.sin_addr.s_addr = INADDR_ANY;
		fgets(msg, MAXLINE, stdin);
		hello2=msg;
		sendto(sockfd2, (const char *)hello2, strlen(hello2),
		MSG_CONFIRM, (const struct sockaddr *) &servaddr2,
			sizeof(servaddr2));
		printf("%s message sent data.\n",hello2);
			
		n2 = recvfrom(sockfd, (char *)buffer2, MAXLINE,
					MSG_WAITALL, (struct sockaddr *) &servaddr2,
					&len2);
		buffer2[n2] = '\0';		
		printf("Server : %s\n", buffer2);
	}else{
		printf("%s\n", buffer);
		fgets(msg, MAXLINE, stdin);
   		hello=msg;
	}		
	close(sockfd);
	return 0;
}

