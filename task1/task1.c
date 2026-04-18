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
    if(num_of_thds <= 0){
        fprintf(stderr, "Number of threads should be >0\n");
        return 1;
    }

    array = parse_input(&array_size);
    if(array == NULL || array_size == 0){
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    //if(array){
    //    parallel_merge_sortt(array, array_size, num_of_thds);
    //    free(array);
    //}

    // checking res pars
    for(int i = 0; i < array_size; ++i){
        printf("%d ", array[i]);
    }printf("\n");


    free(array);
    return 0;
}

int* parse_input(int* size_ptr)
{
    char* buff = NULL;
    size_t buff_size = 0;

    ssize_t chars_read = getline(&buff, &buff_size, stdin);
    if(chars_read == -1){
        fprintf(stderr, "Error reading input\n");
        exit(1);
    }

    int count = 0;
    char* copy_buf = strdup(buff);
    if(!copy_buf){ fprintf(stderr, "strdup"); exit(1); }

    char* token = strtok(copy_buf, " \t\n");
    while(token){
        ++count;
        token = strtok(NULL, " \t\n");
    }
    free(copy_buf);

    if(count == 0){
        free(buff);
        *size_ptr = 0;
        return NULL;
    }

    int* result = (int*)malloc(count * sizeof(int));
    if(!result){ fprintf(stderr, "result:=malloc"); exit(1); }

    int idx = 0;
    token = strtok(buff, " \t\n");
    while(token){
        result[idx++] = atoi(token);
        token = strtok(NULL, " \t\n");
    }

    free(buff);
    *size_ptr = count;
    return result;
}