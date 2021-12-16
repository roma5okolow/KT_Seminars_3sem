#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 1000

struct msgbuf
{
	long mtype;
	char mtext[MAX_SIZE];
};

int main(int argc, char **argv)
{
	key_t key = 1234;
	int msgid = msgget(key, IPC_CREAT | 0666);
	if (msgid == -1){
		perror(("msgget error\n"));
		exit(0);
	}

	struct msgbuf msg;
	long mtype = 1;

	msgrcv(msgid, &msg, sizeof(msg.mtext), mtype, 0);

	puts(msg.mtext);
}