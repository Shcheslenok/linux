#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>

//client
int main(void){
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
        } in_struct;
    } mybuf;
    
    if((key = ftok(pathname, 1)) < 0){
        printf("ERROR ftok\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("ERROR msgget\n");
        exit(-1);
    }

    mybuf.num = 5;
    strcpy(mybuf.str, "Hello");
    mybuf.in_struct.a = 1;
    mybuf.in_struct.b = 2;
    mybuf.in_struct.c = 3;

    int len = 100;
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, 6*sizeof(int), 0) < 0){
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        printf("ERROR msgsnd\n");
        exit(-1);
    }

	if(msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf), 0, IPC_NOWAIT) < 0){
		printf("ERROR MSGRCV\n");
		exit(-1);
	}

	if(errno == EAGAIN){
		printf("No connection to the server\n");
	}
}

