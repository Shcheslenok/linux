#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#define INT 1
#define ARRAY 2
#define STRUCT 3

//client
int main(void){
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
	
	mybuf.mtype = STRUCT;
    mybuf.num = 0;
    strcpy(mybuf.str, "Hello");
    mybuf.in_struct.a = 1;
    mybuf.in_struct.b = 2;
    mybuf.in_struct.c = 3;

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, 8*sizeof(int), 0) < 0){
        printf("ERROR msgsnd\n");
        exit(-1);
    }
}

