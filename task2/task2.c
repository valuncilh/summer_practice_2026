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
        fprintf(stderr, "usage: %s fifo_path\n", argv[0]); return 1;
    }

    const char* fifo_path = argv[1];
    if(!fifo_path){ fprintf(stderr, "path is NULL\n"); return 1; }

    if(mkfifo(fifo_path, 0666) == -1 && errno != EEXIST){
        perror("mkfifo failed\n");
        return 1;
    }

    printf("FIFO '%s' is ready. Chating...\n", fifo_path);

    int fifo_fd = open(fifo_path, O_RDWR);
    if(fifo_fd == -1){
        perror("failed open fifo\n");
        return 1;
    }

    char buff[1024];
    fd_set readfds;
    int max_fd = (STDIN_FILENO > fifo_fd) ? STDIN_FILENO : fifo_fd;

    printf("Type a msg and press ENTER. Press Ctrl+D to quit.\n");

    for(;;){
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(fifo_fd, &readfds);

        int action = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if(action == -1){
            if(errno == EINTR) continue;
            perror("select failed\n");
            break;
        }

        if(FD_ISSET(STDIN_FILENO, &readfds)){
            if(fgets(buff, sizeof(buff), stdin) == NULL){
                // Ctrl+D (EOF на stdin)
                printf("\nExiting...\n");
                break;
            }

            if(write(fifo_fd, buff, strlen(buff)) == - 1){
                perror("write to fifo failed");
                break;
            }
        }

        if (FD_ISSET(fifo_fd, &readfds)) {
            ssize_t n = read(fifo_fd, buff, sizeof(buff) - 1);
            
            if (n > 0) {
                buff[n] = '\0';
                printf("> %s", buff);
                fflush(stdout); 
            } else if (n == 0) {
                fprintf(stderr, "\nListener disconnected.\n");
                break;
            } else {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("read from fifo failed");
                    break;
                }
            }
        }
    }

    close(fifo_fd);
    return 0;
}