#include <stdio.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Error! 1 argument requiered.");
	}
	
	int inotify_fd = inotify_init();
	if (inotify_fd == -1)
	{
		perror("inotify_init error: ");
		exit(0);
	}

	struct inotify_event *event;

	int watch_id = inotify_add_watch(inotify_fd, argv[1], IN_CREATE | IN_DELETE);
	if (watch_id == -1)
	{
		perror("watch_add error: ");
		exit(0);
	}

	size_t max_size = 3 * (sizeof(event) + PATH_MAX);
	char buf[max_size];
	int len = 0;

	while(1)
	{
		len = read(inotify_fd, buf, max_size);
		if (len == -1 && errno != EAGAIN)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}

		for(char* ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len)
		{
			event = (struct inotify_event *) ptr;

			if (event->mask & IN_CREATE)
			{
				printf("%s was created\n", event->name);
			}

			if (event->mask & IN_DELETE)
			{
				printf("%s was deleted\n", event->name);
			}

		}
	}

	close(inotify_fd);

}