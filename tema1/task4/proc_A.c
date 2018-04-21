#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

//PROC A
int main(int argc, char *argv[], char *envp){
	int fd = atoi(argv[1]);
	char str[4];

	while (1){
		printf("Enter number: ");
		scanf("%s", str);
		write(fd, str, 4);

		if (atoi(str) == 10){
			kill(getppid(),SIGUSR1);
		}
	}
}
