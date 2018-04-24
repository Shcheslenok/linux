#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <getopt.h>

//server
int main(int argc, char *argv[]){

	int opt;
	char * file_int = "int.txt";
	char * file_char = "array.txt";
	char * file_struct = "struct.txt";
	char str_help[] = 
			"server [options] ...\n"
			"-D - Start as demon"
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
				file_char = optarg;
				break;

			case 's':
				file_struct = optarg;
				break;

			case 'h':
				printf("%s", str_help);
				break;

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

    while(1){
    	if (msgrcv(msqid, (struct msgbuf *) &mybuf, 6*sizeof(int), 0, 0) < 0){
        	printf("ERROR msgrcv\n");
        	exit(-1);
    	}

		if(msgsnd(msqid,(struct msgbuf *) &mybuf, sizeof(mybuf), 0) < 0){
			printf("ERROR MSGSND\n");
			msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
			exit(-1);
		}

    	printf("num: %d, str: %s\nstruct{a: %d, b: %d, c: %d}\n",mybuf.num, mybuf.str, mybuf.in_struct.a, mybuf.in_struct.b, mybuf.in_struct.c);
	}
}

