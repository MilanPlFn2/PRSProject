// Server side implementation of UDP client-server model
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

void readFile( const char * filename ) {
    int returnCode;
    int count;

    FILE * stream = fopen( filename, "r" );
    if ( stream == NULL ) {
        fprintf( stderr, "Cannot open file for reading\n" );
        exit( -1 );
    }

    printf( "How many blocks to read: " );
    scanf( "%d", &count );

    {
        char buffer[ MAXLINE * count + 1];
        if ( count != fread( buffer, MAXLINE, count, stream ) ) {
            fprintf( stderr, "Cannot read blocks in file\n" );
        }
        buffer[ MAXLINE * count ] = '\0';
        printf( buffer );
    }

    returnCode = fclose( stream );
    if ( returnCode == EOF ) {
        fprintf( stderr, "Cannot close file\n" );
        exit( -1 );
    }
}

// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];	
	struct sockaddr_in servaddr, cliaddr;
	
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	int sockfd2;
	char buffer2[MAXLINE];	
	struct sockaddr_in servaddr2, cliaddr2;
	
	// Creating socket file descriptor
	if ( (sockfd2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr2, 0, sizeof(servaddr2));
	memset(&cliaddr2, 0, sizeof(cliaddr2));
	
	// Filling server information
	servaddr2.sin_family = AF_INET; // IPv4
	servaddr2.sin_addr.s_addr = INADDR_ANY;
	servaddr2.sin_port = htons(PORT2);
	
	// Bind the socket with the server address
	if ( bind(sockfd2, (const struct sockaddr *)&servaddr2,
			sizeof(servaddr2)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int len, n;
	int len2, n2;
	char *hello = "SYN-ACK";
	char *hello2 = "SYN-ACK";
	int ret;
	char msg[MAXLINE];
	len = sizeof(cliaddr); //len is value/resuslt
	len2 = sizeof(cliaddr2);
	while(1){	
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
					MSG_WAITALL, ( struct sockaddr *) &cliaddr,
					&len);
		buffer[n] = '\0';
		printf("Client : %s\n", buffer);
		ret = strncmp(buffer,"ACK",3);
		if(ret==0){					
			pid_t pid = fork();
			if (pid == -1) {
				// Il y a une erreur
				perror("fork");
				return EXIT_FAILURE;
			} else if (pid == 0){
				while (1)
				{
					n2 = recvfrom(sockfd2, (char *)buffer2, MAXLINE,
					MSG_WAITALL, ( struct sockaddr *) &cliaddr2,
					&len2);
					buffer2[n2] = '\0';
					printf("Client data : %s\n", buffer2);

					int returnCode;
					int count;

					FILE * stream = fopen( buffer2, "r" );
					if ( stream == NULL ) {
						fprintf( stderr, "Cannot open file for reading\n" );
						exit( -1 );
					}
					
					while (! feof( stream )){
						char buffer[ MAXLINE + 1];
						if ( count != fread( buffer, MAXLINE, 1, stream ) ) {
							fprintf( stderr, "Cannot read blocks in file\n" );
						}
						buffer[ MAXLINE * count ] = '\0';
						sendto(sockfd2, buffer, strlen(buffer),
						MSG_CONFIRM, (const struct sockaddr *) &cliaddr2,
							len2);
						printf("%s send\n",hello);						
					}
					returnCode = fclose( stream );
							if ( returnCode == EOF ) {
								fprintf( stderr, "Cannot close file\n" );
								exit( -1 );
							}
			

					
					/* hello="Bonjour";
					sendto(sockfd2, (const char *)hello, strlen(hello),
						MSG_CONFIRM, (const struct sockaddr *) &cliaddr2,
							len2);
					printf("%s send\n",hello); */
				}	
			}
			
		}else if (strncmp(buffer,"SYN",3)==0)
		{
			hello="SYN-ACK8081";
			sendto(sockfd, (const char *)hello, strlen(hello),
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
				len);
			printf("%s send\n",hello);
		}
		
		
		
	}
	return 0;
}