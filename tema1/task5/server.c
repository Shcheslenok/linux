#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

#define INT 1
#define ARRAY 2
#define STRUCT 3

//server
int main(int argc, char *argv[]){

	int opt;
	mode_t mode = 0666;
	int fd;
	char * file_int = "int.txt";
	char * file_array = "array.txt";
	char * file_struct = "struct.txt";
	char str_help[] = 
			"server [options] ...\n"
			"-D - Start as demon\n"
			"-i <namefile> - name for file with int\n"
			"-c <namefile> - name for file with char[5]\n"
			"-s <namefile> - name for file with struct\n"
			"-h - help\n";

	while((opt = getopt(argc, argv, "Dhi:c:s:")) != -1){
		switch(opt){
			case 'D':
				printf("Demon\n");
				break;

			case 'i':
				file_int = optarg;
				break;

			case 'c':
				file_array = optarg;
				break;

			case 's':
				file_struct = optarg;
				break;

			case 'h':
				printf("%s", str_help);
				exit(0);

			case '?':
				fprintf(stderr, "%s", str_help);
				exit(1);

			default:
				fprintf(stderr, "Unknown error\n");
				exit(1);
		}
	}

    int msqid;
    char pathname[] = "server";
    key_t key;
    struct msgbuf{
		long mtype;
        int num;
        char str[5];
        struct mystruct{
            int a;
            int b;
            int c;
        }in_struct;
    } mybuf;

	if((key = ftok(pathname, 0)) < 0){
		printf("ERROR FTOK\n");
		exit(-1);
	}

	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
		printf("ERROR MSGGET\n");
		exit(-1);
	}

	char buf[4096];
	char s_time[40];
	struct tm *u;
	time_t timer;
	char elem[10];

    while(1){
    	if (msgrcv(msqid, (struct msgbuf *) &mybuf, 8*sizeof(int), 0, 0) < 0){
        	printf("ERROR msgrcv\n");
        	exit(-1);
    	}
		
		timer = time(NULL);
		u = localtime(&timer);
		strftime(s_time, 40/*strlen(s_time)*/, "%x %X" /*"%d.%m.%Y" %H:%M:%S"*/, u);
		printf("time: %s\n",s_time);

		if (mybuf.mtype == INT){
			if ((fd=open(file_int, O_WRONLY | O_APPEND | O_CREAT, mode)) < 0){
				printf("ERROR OPEN\n");
				exit(-1);
			}

			sprintf(elem, "%d", mybuf.num);
			strcpy(buf, s_time);
			strcat(buf, ":");
			strcat(buf, elem);
			strcat(buf, "\n");

			if(write(fd, buf, strlen(buf)) < 0){
				printf("ERROR WRITE\n");
				exit(-1);
			}

			close(fd);
		}

		if (mybuf.mtype == ARRAY){
			if ((fd=open(file_array, O_WRONLY | O_APPEND | O_CREAT, mode)) < 0){
				printf("ERROR OPEN\n");
				exit(-1);
			}

			strcpy(buf, s_time);
			strcat(buf, ":");
			strcat(buf, mybuf.str);
			strcat(buf, "\n");

			if(write(fd, buf, strlen(buf)) < 0){
				printf("ERROR WRITE\n");
				exit(-1);
			}

			close(fd);
		}

		if (mybuf.mtype == STRUCT){
			if ((fd=open(file_struct, O_WRONLY | O_APPEND | O_CREAT, mode)) < 0){
				printf("ERROR OPEN\n");
				exit(-1);
			}
			
			strcpy(buf, s_time);
			strcat(buf, ":");
			sprintf(elem, "%d", mybuf.in_struct.a);
			strcat(buf, elem);
			strcat(buf, " ");
			sprintf(elem, "%d", mybuf.in_struct.b);
			strcat(buf, elem);
			strcat(buf, " ");
			sprintf(elem, "%d", mybuf.in_struct.c);
			strcat(buf, elem);
			strcat(buf, "\n");

			if(write(fd, buf, strlen(buf)) < 0){
				printf("ERROR WRITE\n");
				exit(-1);
			}

			close(fd);
		}

		printf("num: %d, str: %s\nstruct{a: %d, b: %d, c: %d}\n",mybuf.num, mybuf.str, mybuf.in_struct.a, mybuf.in_struct.b, mybuf.in_struct.c);
	}
}

