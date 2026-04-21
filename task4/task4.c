#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>


const char* ALOWED_LONG_OPTION[] = {
    "1c+\0",
    "2c+\0",
    "2c3\0",
    "4c\0",
    "8c\0",
    "16c\0"
};

_Bool admit_long_option(char * opt_arg)
{
    int size = sizeof(ALOWED_LONG_OPTION) / sizeof(char *);
    for(int i = 0; i < size; ++i){
        if(!strcmp(opt_arg, ALOWED_LONG_OPTION[i])) return 1;
    }
    return 0;
}

int main(int argc, char** argv){

    opterr = 0;

    int opt;
    static struct option long_opt[]= {
        {"elbrus", required_argument, NULL, 'E'},
        {0, 0, 0,0}
    };

    char short_opts[100];
    int short_cnt = 0;

    char* long_opts[100];
    int long_cnt = 0;

    char *non_opts[100];
    int non_cnt = 0;

    for(int i = optind; i < argc; ++i){
        non_opts[non_cnt++] = argv[i];
    }

    while((opt = getopt_long(argc, argv, "mcst", long_opt, NULL)) != -1)
    {
        switch (opt) {
            case 'm': case 'c': case 's': case 't':
                short_opts[short_cnt++] = (char)opt;
                break;
            case 'E':
                if(admit_long_option(optarg)) long_opts[long_cnt++] = optarg;
                else {
                    printf("Incorrect option: 'elbrus=%s'\n", optarg);
                    return 1;
                }
                break;
            case '?':
                if(optopt != 0) printf("Incorrect option: '%c'\n", optopt);
                else printf("Incorrect option: 'elbrus=%s'\n", argv[optind - 1]);
                return 1;
        }
    }

    

    return 0;
}