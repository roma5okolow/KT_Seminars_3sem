#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define NUM_PROC 3

int main(int argc, char** argv){

	int pid_file = open(argv[1], O_RDWR | O_CREAT | O_APPEND);

	if (!pid_file){
		printf("Open error\n");
		exit(0);
	}
	
	for (int i = 0; i < NUM_PROC; i++){
		pid_t pid = fork();
		
		if (pid < 0){
			perror("Fork failed");
		}

		if(pid == 0){
			int *child_pids = (int *)calloc(NUM_PROC - i - 1, sizeof(int));
		}

		if (pid > 0){
			childs_pids[i] = pid;
		}
	}
	printf("My pid: %d, Parents pid: %d\n");
	for(int i = 0; i < sizeof(child_pids); i++){
		printf("Child %d : %d\n",i, child_pids[i]) 
	}
	

	return 0;
}