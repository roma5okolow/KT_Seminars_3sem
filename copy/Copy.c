#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10000

int main(int argc, char** argv)
{
    char* buf = (char*) calloc(MAX_SIZE, sizeof(char));
    int source = open(argv[1], O_RDONLY);
    int dest = open(argv[2], O_RDWR, O_TRUNC); 
    
    if (source == -1 || dest == -1)
    {
        printf("Read error\n");
        exit(0);
    }
    
    int source_size = read(source, buf, MAX_SIZE);

    buf = (char*) realloc(buf, source_size * sizeof(char));

    write(dest, buf, source_size);

    close(source);
    close(dest);
    
    free(buf);

    return 0; 
}
