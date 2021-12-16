#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const long MAX_FILE_SIZE = 1e11;

void OpenFile(char *path);
void RecieverExit(int signum);
void Empty(int signum);
void TransmitterExit(int signum);

	int fd = -1;
pid_t reciever_pid = 0;

int	main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Error, 2 arguments required\n");
		exit(0);
	}

	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigaddset(&set, SIGALRM);
	sigprocmask(SIG_BLOCK, &set, NULL);

	sigfillset(&set);
	sigdelset(&set, SIGUSR1);
	sigdelset(&set, SIGUSR2);
	sigdelset(&set, SIGALRM);
	sigdelset(&set, SIGINT);

	OpenFile(argv[1]);

	reciever_pid = atoi(argv[2]);
	printf("my_pid = %d\n", getpid());
	printf("reciever_pid = %d\n", reciever_pid);

	// test signal
	struct sigaction test;
	test.sa_handler = &Empty;
	int res = sigaction(SIGQUIT, &test, NULL);
	if (res == -1)
	{
		perror("test: ");
		exit(EXIT_FAILURE);
	}

	sigset_t set_test;
	sigfillset(&set_test);
	sigdelset(&set_test, SIGQUIT);
	sigdelset(&set_test, SIGINT);
	res = kill(reciever_pid, SIGQUIT);
	if (res == -1)
	{
		perror("kill SIGQUIT: ");
		exit(EXIT_FAILURE);
	}
	sigsuspend(&set_test);

	struct sigaction reciever_exit;
	memset(&reciever_exit, 0, sizeof(reciever_exit));
	reciever_exit.sa_handler = &RecieverExit;
	sigfillset(&reciever_exit.sa_mask);
	res = sigaction(SIGALRM, &reciever_exit, NULL);
	if (res == -1)
	{
		perror("reciever_exit: ");
		exit(EXIT_FAILURE);
	}

	struct sigaction reciever_confirm;
	memset(&reciever_confirm, 0, sizeof(reciever_confirm));
	reciever_confirm.sa_handler = &Empty;
	sigfillset(&reciever_confirm.sa_mask);
	res = sigaction(SIGUSR1, &reciever_confirm, NULL);
	if (res == -1)
	{
		perror("reciever_confirm usr1: ");
		exit(EXIT_FAILURE);
	}

	struct sigaction transmitter_exit;
	memset(&transmitter_exit, 0, sizeof(transmitter_exit));
	transmitter_exit.sa_handler = &TransmitterExit;
	sigfillset(&transmitter_exit.sa_mask);
	res = sigaction(SIGINT, &transmitter_exit, NULL);
	if (res == -1)
	{
		perror("transmitter_exit SIGINT");
		exit(EXIT_FAILURE);
	}

	res = sigaction(SIGUSR2, &reciever_confirm, NULL);
	if (res == -1)
	{
		perror("reciever_confirm usr2: ");
		exit(EXIT_FAILURE);
	}

	char buf = 0;
	ssize_t size = 0;

	clock_t begin = clock();

	double capacity = 0;
	while ((size = read(fd, &buf, 1)) > 0)
	{
		alarm(1);
		for (int mask = 128; mask >= 1; mask /= 2)
		{
			if (buf & mask)
			{
				res = kill(reciever_pid, SIGUSR1);
				if (res == -1)
				{
					perror("kill SIGUSR1: ");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				res = kill(reciever_pid, SIGUSR2);
				if (res == -1)
				{
					perror("kill SIGUSR2: ");
					exit(EXIT_FAILURE);
				}
			}
			sigsuspend(&set);
		}
		capacity++;
	}
	if (size == -1)
	{
		perror("read: ");
		exit(EXIT_FAILURE);
	}

	res = kill(reciever_pid, SIGCHLD);
	if (res == -1)
	{
		perror("kill SIGCHLD: ");
		exit(EXIT_FAILURE);
	}

	capacity /= 1000;
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	double speed = capacity / time_spent;
	printf("file size = %g KBytes\n", capacity);
	printf("time spent = %g s\n", time_spent);
	printf("speed = %g Kbytes/s\n", speed);
	return 0;
}

void OpenFile(char *path)
{
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("Open file error:");
		exit(EXIT_SUCCESS);
	}
}

void RecieverExit(int signum)
{
	close(fd);
	exit(EXIT_SUCCESS);
}

void TransmitterExit(int signum)
{
	kill(reciever_pid, SIGCHLD);
	close(fd);
	exit(EXIT_SUCCESS);
}

void Empty(int signum)
{
}
