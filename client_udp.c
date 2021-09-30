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
#define cipherKey 'S'
#define sendrecvflag 0
#define nofile "File Not Found!"
// function to clear buffer
void clearBuf(char* b)
{
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}
  
// function to encrypt
char Cipher(char ch)
{
    return ch ^ cipherKey;
}
  
// function sending file
int sendFile(FILE* fp, char* buf, int s)
{
    int i, len;
    if (fp == NULL) {
        strcpy(buf, nofile);
        len = strlen(nofile);
        buf[len] = EOF;
        for (i = 0; i <= len; i++)
            buf[i] = Cipher(buf[i]);
        return 1;
    }
  
    char ch, ch2;
    for (i = 0; i < s; i++) {
        ch = fgetc(fp);
        ch2 = Cipher(ch);
        buf[i] = ch2;
        if (ch == EOF)
            return 1;
    }
    return 0;
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
char* rcv_ack_serv(char *buffer,int sockfd,struct sockaddr_in addr){
	int n, len;
	len=sizeof(addr);
	n = recvfrom(sockfd, (char *)buffer, MAXLINE,
					MSG_WAITALL, (struct sockaddr *) &addr,
					&len);
	buffer[n] = '\0';		
	printf("Server : %s\n", buffer);
	return buffer;
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
	char net_buf[NET_BUF_SIZE];
	FILE* fp;

	sockfd=socket_create();
	servaddr=addr(PORT);	
	int n, len;
	int n2, len2;
	send_seg_serv(sockfd,hello,servaddr);
	*buffer = *rcv_ack_serv(buffer,sockfd,servaddr);
	ret = strncmp(buffer,"SYN-ACK",7);
	if(ret==0){
		hello= "ACK";
		send_seg_serv(sockfd,hello,servaddr);		
		*buffer = *rcv_ack_serv(buffer,sockfd,servaddr);
		int sockfd2;
		char buffer2[MAXLINE];	
		char *hello2 = "SYN";		
		int port2 =atoi(buffer);		
		struct sockaddr_in servaddr2;
		sockfd2=socket_create();
		servaddr2=addr(port2);		
		hello2="OUI";
		send_data_txt_serv(sockfd2,hello2,servaddr2);
		fp = fopen(net_buf, "r");
        printf("\nFile Name Received: %s\n", net_buf);
        if (fp == NULL)
            printf("\nFile open failed!\n");
        else
            printf("\nFile Successfully opened!\n");
	}else{
		printf("%s\n", buffer);
		fgets(msg, MAXLINE, stdin);
   		hello=msg;
	}		
	close(sockfd);
	return 0;
}