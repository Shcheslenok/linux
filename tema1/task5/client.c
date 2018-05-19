#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <getopt.h>
#include <errno.h>

#define INT 1
#define STRING 2
#define STRUCT 3
#define REQUEST_SERVER_PID 254
#define SERVER_PID 255

//client
int main(int argc, char *argv[]){
	char str_help[] = 
			"client [option] ...\n"
			"-i - input from keybord\n"
			"-f <filename> - input from file\n"
			"-p - get pid server\n"
			"-h - help\n";
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
        } in_struct;
    } mybuf;
	if((key = ftok(pathname, 0)) < 0){
        printf("ERROR ftok\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666)) < 0){
        printf("ERROR msgget: no connection\n");
        exit(-1);
    }

	printf("Connected\n");
	
	int opt;
	char type_data[10] = "";
	FILE *fp;
	char buf[100];
	char *index;
	char num[10];
	while((opt = getopt(argc, argv, "if:ph")) != -1){
		switch (opt){
			case 'i':
				printf("Type of data:\n1)int\n2)string\n3)struct\nInput type of data: ");
				scanf("%s", type_data);

				if (strcmp(type_data, "int") == 0){
					printf("Input number: ");
					scanf("%d", &mybuf.num);
					mybuf.mtype = INT;
				}else if (strcmp(type_data, "string") == 0){
					printf("Input string: ");
					scanf("%s", mybuf.str);
					mybuf.mtype = STRING;
				}else if (strcmp(type_data, "struct") == 0){
					printf("Input first number: ");
					scanf("%d", &mybuf.in_struct.a);
					printf("Input second number: ");
					scanf("%d", &mybuf.in_struct.b);
					printf("Input third number: ");
					scanf("%d", &mybuf.in_struct.c);
					mybuf.mtype = STRUCT;
				}else{
					printf("Unknown type\n");
					exit(1);
				}
				if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf), 0) < 0){
        			printf("ERROR msgsnd\n");
        			exit(-1);
    			}

				break;
			case 'f':
				if ((fp = fopen(optarg, "r")) != NULL){
					while(fscanf(fp, "%s", buf) > 0){
						strcpy(type_data, "int");
						
						if((index = strchr(buf, ':')) != 0){
							strncpy(type_data, buf, index - buf);
						} else {
							printf("Uncorrect data:\n%s\n", buf);
							break;
						}

						if (strcmp(type_data, "int") == 0){
							mybuf.num = atoi(index + 1);
							mybuf.mtype = INT;
						} else if (strcmp(type_data, "string") == 0){
							strcpy(mybuf.str, index + 1);
							mybuf.mtype = STRING;
						} else if (strcmp(type_data, "struct") == 0){
							mybuf.in_struct.a = atoi(index + 1);
							fscanf(fp,"%s", num);
							mybuf.in_struct.b = atoi(num);
							fscanf(fp,"%s", num);
							mybuf.in_struct.c - atoi(num);
							mybuf.mtype = STRUCT;
						}
						
						if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf), 0) < 0){
        					printf("ERROR msgsnd\n");
        					exit(-1);
    					}
					}
				}else{
					printf("ERROR FOPEN\n");
					exit(1);
				}
				break;
			
			case 'p':
				mybuf.mtype = REQUEST_SERVER_PID;
				if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf), 0) < 0){
        			printf("ERROR msgsnd\n");
        			exit(-1);
    			}

				if (msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf), SERVER_PID, 0) < 0){
					printf("ERROR MSGRCV\n");
					exit(1);
				}

				printf("Server pid: %d\n", mybuf.num);
				break;
			case 'h':
				printf("%s", str_help);
				exit(0);
			case '?':
				fprintf(stderr, "%s", str_help);
				exit(1);
			default:
				printf("Unknown error\n");
				exit(-1);
		}
	}
}

