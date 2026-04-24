#define _GNU_SOURCE

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAX_ERRNO 133 
char *my_sys_errlist[MAX_ERRNO];

void init_sys_errlist() {
    for (int i = 0; i < MAX_ERRNO; i++) {
        my_sys_errlist[i] = strerror(i);
    }
}

int main(void) {
    init_sys_errlist();

    int fd = open("file_does_not_exist_2005.txt", O_RDONLY);

    if (fd == -1) {
        int er = errno;

        fprintf(stderr, "Can't open file: ");


        if (er >= 0 && er < MAX_ERRNO) {
            fprintf(stderr, "%s.\n", my_sys_errlist[er]);
        } else {
            fprintf(stderr, "Unknown error\n");
        }

        fprintf(stderr, "Errno = ");
        switch (er) {
            case ENOENT:
                fprintf(stderr, "ENOENT\n");
                break;
            case EACCES:
                fprintf(stderr, "EACCES\n");
                break;
            case EISDIR:
                fprintf(stderr, "EISDIR\n");
                break;
            case EMFILE:
                fprintf(stderr, "EMFILE\n");
                break;
            default:
                fprintf(stderr, "%d\n", er);
                break;
        }

        return 1;
    }

    close(fd);
    return 0;
}