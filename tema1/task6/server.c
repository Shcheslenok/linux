#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 51000
int sockfd;//, newsockfd;
/*int 0clilen;
int n;
char line[1000];
struct sockaddr_in servaddr, cliaddr;*/

void *thread_server(void *dummy){
	int newsockfd;
	int clilen;
	int n;
	char line[1000];
	struct sockaddr_in cliaddr;

	if(listen(sockfd, 5) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	
	char buf[1020];
	FILE *fp;
	int i = 0;
	char *res;
	while(1){
		clilen = sizeof(cliaddr);
		if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0){
			perror(NULL);
			close(sockfd);
			exit(1);
		}
		memset(line, '\0', sizeof(line));	
		memset(buf, '\0', sizeof(buf));
		while((n = read(newsockfd, line, 999)) > 0){
			strncpy(buf, line, strlen(line) - 1);
			strcat(buf, " > /tmp/result.txt");
			system(buf);
			printf("buf: %s\n", buf);
		
			memset(buf, '\0', sizeof(buf));	
			strncpy(buf, line, strlen(line) - 1);
			strcat(buf, " 2>> /tmp/result.txt");
			system(buf);
			printf("buf: %s\n", buf);
			
			fp = fopen("/tmp/result.txt", "r");
			
			while(fgets(buf, 999, fp) != NULL)
			//while(fscanf(fp, "%s", buf) > 0)
				i++;

			fseek(fp, 0, SEEK_SET);
				
			printf("num str: %d\n", i);
			sprintf(buf, "%d", i);
			if((n = write(newsockfd, buf, 1000)) < 0){
				perror(NULL);
				close(sockfd);
				close(newsockfd);
				exit(1);
			}

			//printf("dskjfhkdsghkdshkg\n");

			while(fgets(buf, 999, fp) != NULL){
			//while(fscanf(fp, "%s", buf) > 0){
				if((n = write(newsockfd, buf, 1000)) < 0){
					perror(NULL);
					close(sockfd);
					close(newsockfd);
					exit(1);
				}
				//memset(buf, '\0', sizeof(buf));
			}

			if(n < 0){
				perror(NULL);
				close(sockfd);
				close(newsockfd);
				exit(1);
			}
			fclose(fp);
			
			i = 0;
			memset(line, '\0', sizeof(line));
			memset(buf, '\0', sizeof(buf));
		}
		close(newsockfd);
	}
}

//server
void main(int argc, char *argv[]){
	//int sockfd;// newsockfd;
	//int clilen;
	//int n;
	//char line[1000];
	struct sockaddr_in servaddr;//, cliaddr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("ERROR socket\n");
		perror(NULL);
		exit(1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
		printf("ERROR BIND\n");
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	int opt;
	char str_help[] = 
			"server [option] ...\n"
			"-p <number of process> - multiprocess server\n"
			"-t <number of threads> - multithreads server\n"
			"-h - help\n";
	if (argc != 3 && argc != 2){
		printf("ERROR\n%s", str_help);
		exit(1);
	}

	int flag = 0;
	int n = 0;
	while((opt = getopt(argc, argv, "p:t:h")) != -1){
		switch(opt){
			case 'p':
				flag = 0;
				n = atoi(optarg);
				break;

			case 't':
				flag = 1;
				n = atoi(optarg);
				break;

			case 'h':
				printf("%s", str_help);
				exit(0);

			case '?':
				fprintf(stderr, "Unknown error\n");
				exit(1);
			default:
				fprintf(stderr, "Unknown error\n");
				exit(1);
		}
	}
	//fork();
	
	/*if(listen(sockfd, 5) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	
	char buf[1020];
	FILE *fp;
	int i = 0;
	char *res;
	while(1){
		clilen = sizeof(cliaddr);
		if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0){
			perror(NULL);
			close(sockfd);
			exit(1);
		}

		while((n = read(newsockfd, line, 999)) > 0){
			strncpy(buf, line, strlen(line) - 1);
			strcat(buf, " > /tmp/result.txt");
			system(buf);
			printf("buf: %s\n", buf);
		
			memset(buf, '\0', sizeof(buf));	
			strncpy(buf, line, strlen(line) - 1);
			strcat(buf, " 2>> /tmp/result.txt");
			system(buf);
			printf("buf: %s\n", buf);
			
			fp = fopen("/tmp/result.txt", "r");
			
			while(fgets(buf, 999, fp) != NULL)
			//while(fscanf(fp, "%s", buf) > 0)
				i++;

			fseek(fp, 0, SEEK_SET);
				
			printf("num str: %d\n", i);
			sprintf(buf, "%d", i);
			if((n = write(newsockfd, buf, 1000)) < 0){
				perror(NULL);
				close(sockfd);
				close(newsockfd);
				exit(1);
			}

			//printf("dskjfhkdsghkdshkg\n");

			while(fgets(buf, 999, fp) != NULL){
			//while(fscanf(fp, "%s", buf) > 0){
				if((n = write(newsockfd, buf, 1000)) < 0){
					perror(NULL);
					close(sockfd);
					close(newsockfd);
					exit(1);
				}
				//memset(buf, '\0', sizeof(buf));
			}

			if(n < 0){
				perror(NULL);
				close(sockfd);
				close(newsockfd);
				exit(1);
			}
			fclose(fp);
			
			i = 0;
			memset(line, '\0', sizeof(line));
			memset(buf, '\0', sizeof(buf));
		}
		close(newsockfd);
	}*/
	//void *th_ser(void *);
	//th_ser = thread_server;
	pthread_t thid;
	for (int i = 0; i < n; i++){
		if (flag == 0){
			fork();
		} else if (flag == 1){
			if (pthread_create(&thid, (pthread_attr_t *) NULL, thread_server, NULL) != 0){
				printf("ERROR PTHREAD_CREATE\n");
				exit(1);
			}
		}else{
			printf("ERROR\n");
			exit(1);
		}
	}
	thread_server(NULL);
}
