#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char ** argv){

    int fd_src = -1;
    pid_t pid;
    char buff[4096];

    if(argc < 2){ printf("usage: %s path_to_file\n", argv[0]); return 1; }

    if((fd_src = open(argv[1], O_RDONLY)) == -1){ printf("Can`t open file\n"); return 1; }

    if((pid = fork()) == -1){ printf("Failed fork\n"); return 1; }
    else if(!pid){
        lseek(fd_src, 0, SEEK_SET);
        
        int fd_out = open("child_copy", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        ssize_t bytes;

        while((bytes = read(fd_src, buff, sizeof(buff))) > 0){
            write(fd_out, buff, bytes);
        }

        close(fd_src);
        close(fd_out);

        printf("Child:\n");
        int fd_print = open("child_copy", O_RDONLY);
        while((bytes = read(fd_print, buff, sizeof(buff))) > 0){
            write(STDOUT_FILENO, buff, bytes);
        }printf("\n\n");

        close(fd_print);

        exit(0);

    }else{

        wait(NULL);

        lseek(fd_src, 0, SEEK_SET);

        int fd_out = open("parent_copy", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        ssize_t bytes;

        while((bytes = read(fd_src, buff, sizeof(buff))) > 0){
            write(fd_out, buff, bytes);
        }

        close(fd_src);
        close(fd_out);


        printf("Parent:\n");
        int fd_print = open("parent_copy", O_RDONLY);
        while((bytes = read(fd_print, buff, sizeof(buff))) > 0){
            write(STDOUT_FILENO, buff, bytes);
        }printf("\n\n");

        close(fd_print);

        exit(0);
    }

    return 0;
};