#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
	DIR* dirp = opendir(argv[1]);
	if (dirp == NULL)
	{
		perror("opendir error");
		exit(0);
	}

	struct dirent *dir;

	while ((dir = readdir(dirp)) != NULL)
	{
		if (dir == NULL)
		{
			perror("read error\n");
			exit(0);
		}
		if (dir->d_type == DT_DIR)
		{
			printf("%s\n", dir->d_name);
		}
	}

	if (closedir(dirp) == -1)
	{
		perror("close error\n");
		exit(0);
	}
		
}