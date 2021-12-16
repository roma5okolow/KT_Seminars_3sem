#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char** argv){

	if (argc < 3){
		printf("not enough arguments");
		exit(0);
	}

	int input_fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC,
						0700);
	if (!input_fd){
		printf("Open error\n");
	}
	dup2(input_fd, STDOUT_FILENO);
	execvp(argv[2], argv + 2); 
}