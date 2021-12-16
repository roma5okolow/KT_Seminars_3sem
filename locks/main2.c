#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("1 argument requiered\n");
		exit(EXIT_FAILURE);
	}

	int fd = open(argv[1], O_WRONLY);
	if (fd == -1)
	{
		perror("open: ");
		exit(EXIT_FAILURE);
	}

	struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, 0};
	
	if (fcntl(fd, F_GETLK, &lock) == -1)
	{
		perror("fcntl: ");
		exit(EXIT_FAILURE);
	}

	switch(lock.l_type)
	{
		case 0:
			printf("type = F_RDLCK\n");
			break;
		case 1:
			printf("type = F_WRLCK\n");
			break;
		case 2:
			printf("type = F_UNLCK\n");
			break;
	}
	
	printf("start = %ld\n", lock.l_start);
	printf("len = %ld\n", lock.l_len);
	printf("pid = %d\n", lock.l_pid);
	if (close(fd) == -1)
	{
		perror("close");
		exit(EXIT_FAILURE);
	}
}