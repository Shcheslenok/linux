#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>

volatile sig_atomic_t flag = 1;

void my_handler(int sig){
	printf("MY HANDLER PROC_B\n");
	flag = 0;
}

//PROC B
int main(int argc, char *argv[], char *envp){
	signal(SIGUSR1, my_handler);

	int fd = atoi(argv[1]);
	char pathname[] = "main";
	key_t key;
	int shmid;
	int* result;

	if((key = ftok(pathname, 0)) < 0){
		printf("ERROR FTOK\n");
		exit(1);
	}

	if ((shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL))<0){
		if (errno != EEXIST){
			printf("ERROR SHMGET CREATE\n");
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

	char str[4];
	int num;
	while (flag){
		read(fd, str, 4);
		num = atoi(str);
		result[0] = num * num;
		result[1] = 1;
	}

	kill(atoi(argv[2]), SIGTERM);
	waitpid(atoi(argv[2]), NULL, 0);

	kill(atoi(argv[3]), SIGUSR1);
	waitpid(atoi(argv[3]), NULL, 0);

	if(shmctl(shmid, IPC_RMID, NULL) < 0){
		printf("ERROR SHMDT\n");
		exit(-1);
	}
}
