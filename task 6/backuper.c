#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("2 argyments requiered!\n");
		exit(EXIT_FAILURE);
	}
	char* source = argv[1];
	char* dest = argv[2];

}