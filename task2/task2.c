#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <errno.h>

int main(int argc, char** argv)
{
    if(argc < 2){
        fprintf(stderr, "usage: %s fifo_path", argv[0]); return 1;
    }

    const char* fifo_path = argv[1];
    if(!fifo_path){ fprintf(stderr, "path is NULL"); return 1; }

    if(mkfifo(fifo_path, 0666) == -1 && errno != EEXIST){
        perror("mkfifo failed");
        return 1;
    }

    printf("FIFO '%s' is ready. Chating...\n", fifo_path);

    return 0;
}