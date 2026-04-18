#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int* parse_input(int*);
void parallel_merge_sortt(int*, int, int);

int main(int argc, char **argv)
{
    int array_size = 0;
    int* array = NULL;

    if(argc != 2){
        fprintf(stderr, "Should usage: %s > number_of_threads \n", argv[0]);
        return 1;
    }

    int num_of_thds = atoi(argv[1]);
    if(num_threads <= 0){
        fprintf(stderr, "Number of threads should be >0\n");
        return 1;
    }

    // array = parse_input(&array_size);

    if(array){
        parallel_merge_sortt(array, array_size, num_of_thds);
        free(array);
    }

    return 0;
}

int* parse_input(int* size_ptr)
{

}