#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define ITER_NUM 10

int main(int argc, char *argv[])
{
	int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (fd < 0)
	{
		printf("Open file error\n");
		exit(-1);
	}

	pid_t last_pid;
	int wstatus;
	int child_pids[ITER_NUM] = {0};

	for (int i = 0; i < ITER_NUM; i++)
	{
		last_pid = fork();
		if (last_pid > 0)
		{
			child_pids[i] = last_pid;
		}
	}

	dprintf(fd, "My pid:   %d\tMy parent: %5d\t\n", getpid(), getppid());


	for (int i = 9; i >= 0; i--)
	{
		if (child_pids[i] == 0)
			break;

		waitpid(child_pids[i], &wstatus, 0);
		dprintf(fd, "My child: %d\n", child_pids[i]);
	}

	switch (last_pid)
	{
	case -1:
		perror("fork");
		exit(0);
		break;

	case 0:
		exit(0);
		break;
	}


	close(fd);
	return 0;
}
