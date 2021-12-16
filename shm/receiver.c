#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

const size_t MAX_FILE_SIZE = 1000000;

int main(int argc, char** argv)
{
	int dest_fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0700);
	if (!dest_fd)
	{
		perror("Open error\n");
		exit(0);
	}

	key_t key1 = 2002;
	int shm_id = shmget(key1, 500, IPC_CREAT | 0666);
	if (shm_id == -1)
	{
		perror("shmget error\n");
		exit(0);
	}

	char *shm_ptr = (char *)shmat(shm_id, NULL, SHM_EXEC | SHM_RDONLY);
	strcpy (buf, shm_ptr);
	write(dest_fd, buf, MAX_FILE_SIZE);
	shmdt(shm_ptr);
}