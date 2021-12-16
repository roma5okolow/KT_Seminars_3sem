#include <stdio.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define TIMEOUT -1
#define MAXSIZE 1000000

/*struct pollfd
{
	int fd;
	short events;
	short revents;
};
*/
int main(int argc, char** argv)
{
	struct pollfd* fd_arr = (struct pollfd*)calloc(argc - 1, sizeof(struct pollfd));

	for(int i = 0; i < argc - 1; i++)
	{
		(fd_arr + i)->fd = open(argv[i + 1], O_RDONLY | O_NONBLOCK);
		(fd_arr + i)->events = POLLIN;
	}
	char*str = (char*)calloc(MAXSIZE, sizeof(char));


	while(1)
	{
		poll(fd_arr, argc - 1, TIMEOUT);
			for(int i = 0; i < argc - 1; i++)
			{
				if ((fd_arr + i)->revents == POLLIN)
				{
					read((fd_arr + i)->fd, str, MAXSIZE);
					printf("fifo №%d: %s\n", i+1, str);	
				}	
			}	
	}
	 
}