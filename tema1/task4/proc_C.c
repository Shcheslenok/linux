#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <errno.h>

void my_handler(int sig){
	printf("MY HANDLER PROC_C\n");
	exit(0);
}

int main(int argc, char *argv[], char *envp){
	signal(SIGUSR1, my_handler);

	int result = 0 ;
	char pathname[] = __FILE__;
	key_t key;
	int shmid;

	if((key = ftok(pathname, 0)) < 0){
		printf("ERROR FTOK\n");
		exit(1);
	}

	if ((shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL))<0){
		if (errno != EEXIST){
			printf("ERROR SHMGET CREAT\n");
			exit(1);
		} else{
			if ((shmid = shmget(key, sizeof(int), 0)) < 0){
				printf("ERROR SHMGET FIND\n");
				exit(1);
			}
		}
	}
	
	if ((result = shmat(shmid, NULL, 0)) == -1){
		printf("ERROR SHMAT\n");
		exit(1);
	}
	while(1){
		printf("Process C: Value = %4d\n", result);
	}
}
