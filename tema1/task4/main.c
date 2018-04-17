#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(int argc, char *argv[], char *envp[]){
    int fd[2];
    size_t size;
    if (pipe(fd) < 0){
        printf("ERROR PIPE\n");
        exit(-1);
    }

    /*pid_t fork_b=fork();
    if (fork_b > 0){	//proc A
		char des[3];
		char pid_child[4];

		sprintf(des, "%d", fd[1]);
		sprintf(pid_child, "%d", fork_b);

		if (execle("./proc_A", argv[0], &des, &pid_child, NULL, envp) < 0){
			printf("ERROR EXECLE PROC A\n");
			exit(1);
		} 
    }else if(fork_b == 0){	//proc B
		pid_t fork_c=fork();

    	if (fork_c > 0){	//proc B
			char des[3];
			char pid_child[4];

			sprintf(des, "%d", fd[0]);
			sprintf(pid_child, "%d", fork_c);

			if (execle("./proc_B", argv[0], &des, &pid_child,  NULL, envp) < 0){
				printf("ERROR EXECLE PROC B\n");
				exit(1);
			}
   		}else if (fork_c == 0){	//proc C
       		if (execle("./proc_C", argv[0], NULL, envp) < 0){
				printf("ERROR EXECLE PROC A\n");
				exit(1);
			}
		}
	}*/
	
	pid_t fork_a = fork();
	if(fork_a > 0){	//proc B
		pid_t fork_c = fork();
		if (fork_c > 0){	//proc B
			char des[3];
			char pid_proc_a[4];
			char pid_proc_c[4];
			
			sprintf(des, "%d", fd[0]);
			sprintf(pid_proc_a, "%d", getppid());
			sprintf(pid_proc_c, "%d", getppid());
			
			if (execle("./proc_B", argv[0], &des, &pid_proc_a, &pid_proc_c, NULL, envp) < 0){
				printf("ERROR EXECLE PROC B\n");
				exit(1);
			}
		} else if(fork_c == 0){	//proc C
			if (execle("./proc_C", argv[0], NULL, envp) < 0){
				printf("ERROR EXECLE PROC A\n");
				exit(1);
			}
		} else{
			printf("ERROR FORK_C\n");
			exit(1);
		}
	} else if (fork_a == 0){	//proc A
		char des[3];
		char pid_proc_b[4];

		sprintf(des, "%d", fd[1]);
		sprintf(pid_proc_b, "%d", getppid());

		if (execle("./proc_A", argv[0], &des, &pid_proc_b, NULL, envp) < 0){
			printf("ERROR EXECLE PROC A\n");
			exit(1);
		}
	} else{
		printf("ERROR FORK_A\n");
		exit(1);
	}
}
