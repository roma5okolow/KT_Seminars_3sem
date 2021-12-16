#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("3 arguments requiered\n");
		exit(EXIT_FAILURE);
	}

	char* file = argv[1];
	int fd = open(file, O_RDWR);
	if (fd == -1)
	{
		perror("open: ");
		exit(EXIT_FAILURE);
	}

	long start = atol(argv[2]);
	long len = atol(argv[3]);

	struct flock lock = {F_RDLCK, SEEK_SET, start, len, 0};

	if (fcntl(fd, F_SETLK, &lock) == -1)
	{
		perror("fcntl: ");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		sleep(1);
	}
}
