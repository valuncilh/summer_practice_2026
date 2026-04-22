#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char** argv){

    struct stat st;

    if(argc < 2) fprintf(stderr, "usage: %s 'path_to_file\n'", argv[0]);

    if(stat(argv[1], &st)){
        printf("File: %s\n", argv[1]);
        printf("Size: %ld\n", (long)st.st_size);
        printf("Size: %ld   Blocks: %ld\n", (long)st.st_size, (long)st.st_blocks);
    }else{
        perror("Can`t open file: ");
    }

    return 0;
}