#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>   
#include <strings.h>  
#include <stdlib.h>

#define COUNT_FUNC_TYPE 4

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

int cmp_plain(const void *l, const void *r) {
    const char *s1 = *(const char **)l;
    const char *s2 = *(const char **)r;
    return strcmp(s1, s2);
}

int cmp_lex(const void *l, const void *r) {
    const char *s1 = *(const char **)l;
    const char *s2 = *(const char **)r;
    return strcasecmp(s1, s2);
}

int cmp_rplain(const void *l, const void *r) {
    const char *s1 = *(const char **)l;
    const char *s2 = *(const char **)r;
    return strcmp(s2, s1); 
}

int cmp_rlex(const void *l, const void *r) {
    const char *s1 = *(const char **)l;
    const char *s2 = *(const char **)r;
    return strcasecmp(s2, s1); 
}

void cleanup_dynamo(dynamo *d) {
    if (d->lines) {
        for (size_t i = 0; i < d->count; ++i) {
            free(d->lines[i]);
        }
        free(d->lines);
    }
}

int main(int argc, char** argv)
{
    static const int max_read_size = 512;
    dynamo arr_s = {NULL, 0, 0};
    char buff[max_read_size];
    int (*cmp_func)(const void *, const void *) = NULL;


    if (argc < 4) {
        fprintf(stderr, "Usage: %s <input.txt> <output.txt> <type_of_sort>\n", argv[0]);
        return 1;
    }

    int type = -1;
    for (int i = 0; i < COUNT_FUNC_TYPE; ++i) {
        if (strcmp(argv[3], FuncType_names[i]) == 0) {
            type = i;
            break;
        }
    }

    if (type == -1) {
        fprintf(stderr, "Error: Unknown sort type '%s'\n", argv[3]);
        return 1;
    }

    FILE* flin = fopen(argv[1], "r");
    if (!flin) {
        fprintf(stderr, "Error opening input file: %s\n", strerror(errno));
        return 1;
    }

    while (fgets(buff, max_read_size, flin) != NULL) {
        size_t len = strlen(buff);

        if (len > 0 && buff[len - 1] == '\n') {
            buff[len - 1] = '\0';
            --len;
        }

        int has_visible = 0;
        for (size_t i = 0; i < len; ++i) {
            if (!isspace((unsigned char)buff[i])) {
                has_visible = 1;
                break;
            }
        }

        if (!has_visible) {
            continue; 
        }

        if (arr_s.count >= arr_s.capacity) {
            size_t new_capacity = (arr_s.capacity == 0) ? 8 : arr_s.capacity * 2;
            
            char **tmp = realloc(arr_s.lines, new_capacity * sizeof(char *));
            if (!tmp) {
                fprintf(stderr, "Memory allocation failed\n");
                cleanup_dynamo(&arr_s); 
                fclose(flin);
                return 1;
            }
            
            arr_s.lines = tmp;
            arr_s.capacity = new_capacity;
        }

        arr_s.lines[arr_s.count] = strdup(buff);
        if (!arr_s.lines[arr_s.count]) {
            fprintf(stderr, "Memory allocation failed (strdup)\n");
            cleanup_dynamo(&arr_s);
            fclose(flin);
            return 1;
        }
        
        ++arr_s.count;
    }

    fclose(flin);

    switch (type) {
        case 0: cmp_func = cmp_plain; break;
        case 1: cmp_func = cmp_lex; break;
        case 2: cmp_func = cmp_rplain; break;
        case 3: cmp_func = cmp_rlex; break;
    }

    if (arr_s.count > 0) {
        qsort(arr_s.lines, arr_s.count, sizeof(char *), cmp_func);
    }

    
    FILE* flout = fopen(argv[2], "w");
    if (!flout) {
        fprintf(stderr, "Error opening output file: %s\n", strerror(errno));
        cleanup_dynamo(&arr_s);
        return 1;
    }

    for (size_t i = 0; i < arr_s.count; ++i) {
        fprintf(flout, "%s\n", arr_s.lines[i]);
    }

    fclose(flout); 

    cleanup_dynamo(&arr_s);
    return 0;
}