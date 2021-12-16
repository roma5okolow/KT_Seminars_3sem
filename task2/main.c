#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_FILE_SIZE 10000
#define MAX_OPERANDS_NUM 100
#define MAX_ARGS_NUM 100

void ReadFile(char *dest, char *source);
size_t GetOperands(char **operands, char *buf);
void GetFunctions(char ***progs, char **operands, size_t size);
int MakeOperator(char ***progs, size_t size);

int main(int argc, char **argv)
{
	char buf[MAX_FILE_SIZE];
	ReadFile(buf, argv[1]);
	char *operands[MAX_OPERANDS_NUM];
	size_t size = GetOperands(operands, buf);
	char ***progs = calloc(size, sizeof(char **));
	for (int i = 0; i < size; i++)
	{
		progs[i] = calloc(MAX_ARGS_NUM, sizeof(char *));
	}
	GetFunctions(progs, operands, size);
	MakeOperator(progs, size);
	for (int i = 0; i < size; i++)
	{
		free(progs[i]);
	}
	free (progs);
}

void ReadFile(char *dest, char *source)
{
	int source_fd = open(source, O_RDONLY);
	if (source_fd == -1)
	{
		perror("open error\n");
		exit(0);
	}

	int source_size = read(source_fd, dest, MAX_FILE_SIZE);
	if (source_size == -1)
	{
		perror("read error\n");
		exit(0);
	}

	close(source_fd);
}

size_t GetOperands(char **operands, char *buf)
{
	size_t i = 0;
	char *operand = strtok(buf, "|\n");
	operands[i++] = operand;
	while (operand != NULL)
	{
		operand = strtok(NULL, "|\n");
		if (operand != NULL)
		{
			if (*operand == ' ')
				operands[i++] = (operand + 1);
			else
				operands[i++] = operand;
		}
	}
	return i;
}

void GetFunctions(char ***progs, char **operands, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; i++)
	{
		size_t j = 0;
		char *prog = strtok(operands[i], " ");
		progs[i][j] = prog;
		while (prog != NULL)
		{
			prog = strtok(NULL, " ");
			if (prog != NULL)
			{
				progs[i][++j] = prog;
			}
		}
		progs[i][++j] = NULL;
	}
	return;
}

int MakeOperator(char ***progs, size_t size)
{
	pid_t fdin = 0;
	for (size_t i = 0; i < size; i++)
	{
		int wstatus;
		int pipefd[2];
		int pipe_res = pipe(pipefd);
		if (pipe_res == -1)
		{
			perror("pipe fail");
			exit(0);
		}

		pid_t pid = fork();

		if (pid < 0)
		{
			perror("fork fail");
			exit(0);
		}
		else if (pid > 0)
		{
			if (waitpid(pid, &wstatus, 0) < 0)
			{
				perror("wait error");
				exit(0);
			}
			fdin = pipefd[0];
			close(pipefd[1]);
		}
		else
		{
			dup2(fdin, STDIN_FILENO);
			if (i != size - 1)
			{
				dup2(pipefd[1], STDOUT_FILENO);
			}
			close(pipefd[0]);
			execvp(progs[i][0], progs[i]);
			perror("execvp");
			return -1;
		}
	}
	return 0;
}