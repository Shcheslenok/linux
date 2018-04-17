#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

void my_handler(int sig){
	printf("MY HANDLER PROC_A\n");
	exit(0);
}

int main(int argc, char *argv[], char *envp){
	int fd = atoi(argv[1]);
	char str[4];

	while (1){
		printf("Enter number: ");
		scanf("%s", str);
		write(fd, str, strlen(str));

		if (atoi(str) == 10){
			kill(atoi(argv[2]),SIGUSR1);
		}
	}
}
