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
#define NET_BUF_SIZE 32

struct rcv {
  char *buffer;
  int taille;
};

int rcpor(char *buffer){
	char test2[4];
	test2[0]=buffer[7];
	test2[1]=buffer[8];	
	test2[2]=buffer[9];	
	test2[3]=buffer[10];
	return atoi( test2);
}
void send_data_txt_serv(int sockfd,char *msg,struct sockaddr_in addr){
	sendto(sockfd, (const char *)msg, strlen(msg),
			MSG_CONFIRM, (const struct sockaddr *) &addr,
				sizeof(addr));
	printf("Data : %s message sent.\n",msg);
}
void send_seg_serv(int sockfd,char *msg,struct sockaddr_in addr){
	sendto(sockfd, (const char *)msg, strlen(msg),
			MSG_CONFIRM, (const struct sockaddr *) &addr,
				sizeof(addr));
	printf("%s message sent.\n",msg);
}
struct rcv rcv_ack_serv(char *buffer,int sockfd,struct sockaddr_in addr){
	struct rcv v;
	int n, len;
	len=sizeof(addr);
	n = recvfrom(sockfd, (char *)buffer, MAXLINE,
					MSG_WAITALL, (struct sockaddr *) &addr,
					&len);
	buffer[n] = '\0';		
	printf("Server : %s\n", buffer);
	v.taille=n;
	v.buffer=buffer;
	return v;
}
int socket_create(){
	// Creating socket file descriptor
	int sockfd;
	if ( ( sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	return sockfd;
}
struct sockaddr_in addr(int port){
	struct sockaddr_in	 servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	return servaddr;
}
// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];	
	char *hello = "SYN";
	int ret;
	char msg[MAXLINE];
	struct sockaddr_in	 servaddr;	
	struct rcv v;

	sockfd=socket_create();
	servaddr=addr(PORT);	
	int n, len;
	int n2, len2;
	send_seg_serv(sockfd,hello,servaddr);
	v = rcv_ack_serv(buffer,sockfd,servaddr);
	ret = strncmp(v.buffer,"SYN-ACK",7);
	if(ret==0){
		hello= "ACK";
		send_seg_serv(sockfd,hello,servaddr);
		int sockfd2;
		char buffer2[MAXLINE];	
		char *filename = "SYN";			
		int port2 = rcpor(v.buffer);				
		struct sockaddr_in servaddr2;
		sockfd2=socket_create();
		servaddr2=addr(port2);		
		filename="test.txt";
		send_data_txt_serv(sockfd2,filename,servaddr2);		
		int returnCode;
		int index;
		FILE * stream = fopen( filename, "w" );
		if ( stream == NULL ) {
			fprintf( stderr, "Cannot open file for writing\n" );
			exit( -1 );
		}
		int test=1;
		int i=0;
		struct rcv v2;
		while (test)
		{
			v2=rcv_ack_serv(buffer2,sockfd2,servaddr2);
			send_seg_serv(sockfd2,"ACK",servaddr2);
			sprintf( buffer2, "%s", v2.buffer );
			if ( 1 != fwrite( buffer2, v2.taille, 1, stream ) ) {
            fprintf( stderr, "Cannot write block in file\n" );
        	}			
			if (v2.taille<MAXLINE)
			{
				returnCode = fclose( stream );
				if ( returnCode == EOF ) {
					fprintf( stderr, "Cannot close file\n" );
					exit( -1 );
			}	test=0;
			}
			
			
		}
		
		

	}else{
		printf("%s\n", buffer);
		fgets(msg, MAXLINE, stdin);
   		hello=msg;
	}		
	close(sockfd);
	return 0;
}