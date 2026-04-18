#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int *array;
    int left;
    int right;
} thread_data;

int* parse_input(int*);
void merge(int*, int, int, int);
void* sequential_merge_sort(void*);
void parallel_merge_sort(int*, int, int);

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
        fprintf(stderr, "Number of threads should be > 0\n");
        return 1;
    }

    array = parse_input(&array_size);
    if(array == NULL || array_size == 0){
        fprintf(stderr, "Empty input\n");
        return 1;
    }


    parallel_merge_sort(array, array_size, num_of_thds);

    for(int i = 0; i < array_size; ++i){
        printf("%d", *(array + i));
        if(i < array_size - 1) printf(" ");
    } printf("\n");


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
    if(!copy_buf){ fprintf(stderr, "strdup failed"); exit(1); }

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
    if(!result){ fprintf(stderr, "malloc failed for result array"); exit(1); }

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

void merge(int* arr, int left, int mid, int right)
{
    int p1 = mid - left + 1;      
    int p2 = right - mid;         

    int *L = malloc(p1 * sizeof(int));
    int *R = malloc(p2 * sizeof(int));

    if(!L || !R){
        perror("malloc faiiled in merge");
        free(L);
        free(R);
        exit(1);
    }

    for (int i = 0; i < p1;  ++i) L[i] = arr[left + i];   
    for (int j = 0; j < p2 ; ++j) R[j] = arr[mid + 1 + j]; 

    int i = 0, j = 0, k = left;
    while (i < p1 && j < p2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }
    
    while (i < p1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < p2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }


    free(L);
    free(R);
}

void* sequential_merge_sort(void* arg){
    thread_data* data = (thread_data *)arg;
    int left = data->left;
    int right = data->right;
    
    if (left >= right) return NULL;

    if (right - left <= 1000) {
        for (int i = left + 1; i <= right; ++i) {
            int key = data->array[i];
            int j = i - 1;
            while (j >= left && data->array[j] > key) {
                data->array[j + 1] = data->array[j];
                --j;
            }
            data->array[j + 1] = key;
        }
        return NULL;
    }

    int mid = left + (right - left) / 2;
    
    thread_data left_data = {data->array, left, mid};
    thread_data right_data = {data->array, mid + 1, right};

    sequential_merge_sort(&left_data);
    sequential_merge_sort(&right_data);

    merge(data->array, left, mid, right);

    return NULL;
}

void parallel_merge_sort(int *arr, int size, int num_threads) {
    if (size <= 1) return;

    if(num_threads > size) num_threads = size;
    if(num_threads < 1) num_threads = 1;

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    thread_data *data = malloc(num_threads * sizeof(thread_data));
    
    if (!threads || !data) {
        perror("malloc failed"); free(threads); free(data); exit(1);
    }

    int chunk_size = size / num_threads;
    if(chunk_size < 1) chunk_size = 1;

    for (int i = 0; i < num_threads; i++) {
        data[i].array = arr;
        data[i].left = i * chunk_size;
        // Последний поток заберёт хвост, если размер не поделится нацело
        data[i].right = (i == num_threads - 1) ? size - 1 : (i + 1) * chunk_size - 1;

        // Защита от некорректных диапазонов, если left > right
        if (data[i].left <= data[i].right) {
            pthread_create(&threads[i], NULL, sequential_merge_sort, &data[i]);
        } else {
            // Если диапазон пустой, помечаем поток как "выполненный" (NULL)
            threads[i] = 0; 
        }
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int width = chunk_size; width < size; width *= 2) {
        for (int start = 0; start < size; start += 2 * width) {
            int mid = start + width - 1;
            int end = (start + 2 * width - 1 < size - 1) ? start + 2 * width - 1 : size - 1;
            
            if (mid < size - 1) {
                merge(arr, start, mid, end);
            }
        }
    }

    free(threads);
    free(data);
}

