#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define PORT 51000

void main(int argc, char **argv){
	int sockfd;
	int n;
	int num_str;
	char sendline[1000], recvline[1000];

	struct sockaddr_in servaddr;
	if (argc != 2){
		printf("Usage: client <IP address>\n");
		exit(1);
	}

	bzero(sendline, 1000);
	bzero(recvline, 1000);

	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror(NULL);
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if(inet_aton(argv[1], &servaddr.sin_addr) == 0){
		printf("Invalid IP address\n");
		close(sockfd);
		exit(1);
	}

	if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	while(1){
		printf("=> ");
		fflush(stdin);
		fgets(sendline, 1000, stdin);
		if((n = write(sockfd, sendline, strlen(sendline) + 1))< 0){
			perror("Can\'t write\n");
			close(sockfd);
			exit(1);
		}
		memset(recvline, '\0', sizeof(recvline));

		if ((n = read(sockfd, recvline, 1000)) < 0){
			perror("Can\'t read\n");
			close(sockfd);
			exit(1);
		}
		
		num_str = atoi(recvline);
		for(int i = 0; i < num_str; i++){
			if ((n = read(sockfd, recvline, 1000)) < 0){
				perror("Can\'t read\n");
				close(sockfd);
				exit(1);
			} 
			printf("%s", recvline);

			memset(recvline, '\0', sizeof(recvline));
		}
	}
	close(sockfd);
}
