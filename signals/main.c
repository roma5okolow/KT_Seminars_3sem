#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
/*
typedef void (*my_handler)(int);
typedef void (*my_sigaction)(int, siginfo_t *, void *);
*/

void my_handler1(int signum);
void my_handler2(int signum);

int main(int argc, char **argv)
{
	printf("My pid: %d\n", getpid());

	struct sigaction act1, act2;
	act1.sa_handler = &my_handler1;
	act2.sa_handler = &my_handler2;

	int status1 = sigaction(SIGUSR1, &act1, NULL);
	int status2 = sigaction(SIGUSR2, &act2, NULL);

	if (status1 == -1 || status2 == -1)
	{
		perror("Sigaction error");
		exit(-1);
	}
	
	while(1){

	}

	return 0;
}

void my_handler1(int signum)
{
	printf("london\n");
	
}

void my_handler2(int signum)
{
	printf("is the capital of great Britain\n");
}
