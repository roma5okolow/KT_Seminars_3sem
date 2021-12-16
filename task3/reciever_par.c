#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void GetPid(int sig, siginfo_t *info, void *ucontext);
void OpenFile(char *path);
void One(int signum);
void Zero(int signum);
void TransmitterExit(int signum);

unsigned char mask = 128;
unsigned char out_char = 0;
int fd = -1;
pid_t transmitter_pid = 0;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Error, 1 argument required\n");
		exit(EXIT_FAILURE);
	}

	OpenFile(argv[1]);

	printf("From reciever: my pid = %d\n", getpid());

	struct sigaction test;
	test.sa_flags = SA_SIGINFO;
	test.sa_sigaction = &GetPid;
	int res = sigaction(SIGQUIT, &test, NULL);
	if (res == -1)
	{
		perror("sigaction test: ");
		exit(EXIT_FAILURE);
	}

	sigset_t set_test;
	sigsuspend(&set_test);
	sigfillset(&set_test);
	sigdelset(&set_test, SIGQUIT);
	sigprocmask(SIG_BLOCK, &set_test, NULL);
	res = kill(transmitter_pid, SIGQUIT);
	if (res == -1)
	{
		perror("kill SIGQUIT: ");
		exit(EXIT_FAILURE);
	}

	printf("transmitter_pid = %d\n", transmitter_pid);

	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_BLOCK, &set, NULL);
	sigemptyset(&set);

	struct sigaction act_zero;
	memset(&act_zero, 0, sizeof(act_zero));
	act_zero.sa_handler = &Zero;
	sigfillset(&act_zero.sa_mask);
	//	sigdelset(&act_zero.sa_mask, SIGUSR2);
	res = sigaction(SIGUSR2, &act_zero, NULL);
	if (res == -1)
	{
		perror("act_zero");
		exit(EXIT_FAILURE);
	}

	struct sigaction act_one;
	memset(&act_one, 0, sizeof(act_one));
	act_one.sa_handler = &One;
	sigfillset(&act_one.sa_mask);
	//	sigdelset(&act_one.sa_mask, SIGUSR1);
	res = sigaction(SIGUSR1, &act_one, NULL);
	if (res == -1)
	{
		perror("act_one: ");
		exit(EXIT_FAILURE);
	}

	struct sigaction transmitter_exit;
	memset(&transmitter_exit, 0, sizeof(transmitter_exit));
	transmitter_exit.sa_handler = &TransmitterExit;
	//	sigfillset(&transmitter_exit.sa_mask);
	//	sigdelset(&act_one.sa_mask, SIGCHLD);
	res = sigaction(SIGCHLD, &transmitter_exit, NULL);
	if (res == -1)
	{
		perror("transmitter_exit: ");
		exit(EXIT_FAILURE);
	}

	do
	{
		if (mask == 0)
		{
			write(fd, &out_char, 1);
			mask = 128;
			out_char = 0;
		}
		sigsuspend(&set);

	} while (1);

	res = kill(transmitter_pid, SIGALRM);
	if (res == -1)
	{
		perror("kill SIGALRM: ");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

void GetPid(int sig, siginfo_t *info, void *ucontext)
{
	transmitter_pid = info->si_pid;
}

void OpenFile(char *path)
{
	fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		perror("Creating file error:");
		exit(EXIT_FAILURE);
	}
}

void One(int signum)
{
	out_char += mask;
	mask /= 2;
	int res = kill(transmitter_pid, SIGUSR1);
	if (res == -1)
	{
		perror("One: ");
		exit(EXIT_FAILURE);
	}
}

void Zero(int signum)
{
	mask /= 2;
	int res = kill(transmitter_pid, SIGUSR2);
	if (res == -1)
	{
		perror("Zero: ");
		exit(EXIT_FAILURE);
	}
}

void TransmitterExit(int signum)
{
	int res = close(fd);
	if (res == -1)
	{
		perror("TransmitterExit: ");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}