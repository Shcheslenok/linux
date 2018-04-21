#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

volatile sig_atomic_t flag = 1;
int* result;

void *C2(void *dummy){
	while(flag){
		if (result[1]){
			printf("Process C: Value = %4d\n", result[0]);
			result[1] = 0;
		} else {
			printf("I am alive\n");
		}
		sleep(1);
	}
	printf("exit\n");
	pthread_exit(NULL);
}

void my_handler(int sig){
	printf("MY HANDLER PROC_C\n");
	flag = 0;
	exit(0);
}
//PROC C
int main(int argc, char *argv[], char *envp){
	signal(SIGUSR1, my_handler);

	char pathname[] = "main";
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
	
	if ((result = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)){
		printf("ERROR SHMAT\n");
		exit(1);
	}

	pthread_t thid;
	if(pthread_create(&thid, (pthread_attr_t *)NULL, C2, NULL) != 0){
		printf("ERROR PTREAD_CREATE\n");
		exit(1);
	}
	
	while(1);
}
