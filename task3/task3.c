/*
        3. Написать программу на С:
    Отсортировать строки текстового файла различными способами. Строки без
    видимых символов удалить из вывода.
        • Параметры командной строки: 3 параметра: входной файл, выходной файл, способ
    сортировки: plain (строки сравниваются по байтам), lex (лексикографический без
    учета регистра), rplain (обратный plain), rlex (обратный lex).
        • Входные данные: ввод в стандартный поток отсутствует.
        • Выходные данные: вывод в стандартный поток отсутствует.
        • Пример запуска:
    # cat in.txt
        >3
        >
        >2
        >a
    # task3 in.txt out.txt plain
    # cat out.txt
        >2
        >3
        >a
*/

#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define count_func_type 4

typedef struct {
    char ** lines;
    size_t count;
    size_t capacity;
} dynamo;

const char* FuncType_names[] = {
    "plain",
    "lex",
    "rplain",
    "rlex"
};

int  plain(const char** l, const char** r);
int lex(const char** l, const char** r);
int rplain(const char** l, const char** r);
int rlex(const char** l, const char** r);

int main(int argc, char** argv)
{
    //var
    static int max_read_size = 256;
    dynamo arr_s = {NULL, 0, 8};
    char buff[max_read_size];
    // var

    if(argc < 4){
        fprintf(stderr, "usage: %s <input.txt> <output.txt> <type_of_sort>\n", argv[0]); return 1;
    }

    int type = {-1};
    for(int i = 0; i < count_func_type; ++i) 
        if(!strcmp(argv[3], FuncType_names[i])) type = i;

    if(type == -1){
        fprintf(stderr, "matching type of func_sort failed\n");
        return 1;
    }

    FILE* flin = fopen(argv[1], "r");
    if(!flin){
        fprintf(stderr, "val of errno: %d\n", errno);
        return 1;
    }
    
    
    while(fgets(buff, max_read_size, flin) != NULL){
        size_t len = strlen(buff);
        if(len > 0 && buff[len - 1] == '\n'){ 
            buff[len - 1] = '\0';
            --len;
        } 

        _Bool isntspace = 0;
        for(int i = 0; i < len; ++i){
            if(!isspace(buff[i])) isntspace = 1;
        }

        if(!isntspace){
            continue;
        }

        if(arr_s.count == arr_s.capacity){
            arr_s.capacity *= 2;
            char ** tmp = (char**)realloc(arr_s.lines, arr_s.capacity);
            if(!tmp){
                fprintf(stderr, "realloc failed to alloc");
                return 1;
            }else{
                arr_s.lines = tmp;
            }
        }

        arr_s.lines[arr_s.count] = strdup(buff);
        ++arr_s.count;
    }


    fclose(flin);


    // FILE* flout = fopen(argv[2], "w");


    // fclose(flout);
    return 0;
}