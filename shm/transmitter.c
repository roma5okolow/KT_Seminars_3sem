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

int main(int argc, char **argv)
{
	int source_fd = open(argv[1], O_RDWR);
	if (!source_fd)
	{
		perror("Open error\n");
		exit(0);
	}

/*	key_t transmitter_key = ftok(argv[0], getpid());
	if (transmitter_key == -1)
	{
		perror("ftok error\n");
		exit(0);
	}
*/


	char buf[MAX_FILE_SIZE];
	ssize_t size = read(source_fd, buf, MAX_FILE_SIZE);
	if (size == -1){
		perror("read error\n");
		exit(0);
	}
	
	key_t key1 = 2002;
	int shm_id1 = shmget(key1, 2 * sizeof(int), IPC_CREAT | 0666);
	if (shm_id1 == -1);
	{
		perror("shmget1 error\n");
		exit(0);
	}

	int *shm_ptr1 = (int *)shmat(shm_id1, NULL, 0);
	if (shm_ptr1 == (void *)-1)
	{
		perror("shmat error\n");
		exit(0);
	}
	shm_ptr1[0] = size;

	key_t key2 = 2003;
	int shm_id2 = shmget(key2, size, IPC_CREAT | 0666);
	if (shm_id2 == -1)
	{
		perror("shmget2 error\n");
		exit(0);
	}
	
	char *shm_ptr2 = (char *)shmat(shm_id2, NULL, 0);
	if (shm_ptr2 == (void *)-1)
	{
		perror("shmat error\n");
		exit(0);
	}
	strcpy(shm_ptr2, buf);
}