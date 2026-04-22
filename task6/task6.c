#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h> 
#include <grp.h>

void print_timestamp(const char* , struct timespec);
void convert_mode_to_str(char * , mode_t *);
const char* set_type_file(struct stat * );

int main(int argc, char** argv){

    struct stat st;

    if(argc < 2){ fprintf(stderr, "usage: %s path_to_file\n", argv[0]); return 1; }

    if(!stat(argv[1], &st)){

        char perms[11] = "----------";
        mode_t m = st.st_mode;

        convert_mode_to_str(perms, &m);

        const char* file_type = set_type_file(&st);

        struct passwd * pw = getpwuid(st.st_uid);
        struct group * gr = getgrgid(st.st_gid);

        const char *user_name = pw ? pw->pw_name : "unknown";
        const char *group_name = gr ? gr->gr_name : "unknown";

        printf("  File: %s\n", argv[1]);

        printf("  Size: %ld   Blocks: %ld    IO Block: %ld    %s\n",
            (long)st.st_size, (long)st.st_blocks,
            (long)st.st_blksize,
            file_type);

        printf("Device: %lu    Inode: %lu   Links: %ld\n",
            (unsigned long)st.st_dev, 
            (unsigned long)st.st_ino, 
            (long)st.st_nlink);

        printf("Access: (%04o/%s) Uid: (%5ld/   %s)   Gid: (%5ld/   %s)\n", 
           (unsigned int)(st.st_mode & 0777), 
            perms, 
           (long)st.st_uid,
            user_name,
           (long)st.st_gid,
           group_name);

        print_timestamp("Access", st.st_atim);
        print_timestamp("Modify", st.st_mtim);
        print_timestamp("Change", st.st_ctim);
    }else{
        perror("Stat error");
        return 1;
    }

    return 0;
}


const char* set_type_file(struct stat * st)
{
    const char* file_type = "unknown";

    if (S_ISREG((*st).st_mode)) file_type = "regular file";
    else if (S_ISDIR((*st).st_mode)) file_type = "directory";
    else if (S_ISLNK((*st).st_mode)) file_type = "symbolic link";
    else if (S_ISCHR((*st).st_mode)) file_type = "character device";
    else if (S_ISBLK((*st).st_mode)) file_type = "block device";
    return file_type;
}

void convert_mode_to_str(char * perms, mode_t * m)
{
    if (S_ISDIR(*m)) perms[0] = 'd';
    if (*m & S_IRUSR) perms[1] = 'r';
    if (*m & S_IWUSR) perms[2] = 'w';
    if (*m & S_IXUSR) perms[3] = 'x';
    if (*m & S_IRGRP) perms[4] = 'r';
    if (*m & S_IWGRP) perms[5] = 'w';
    if (*m & S_IXGRP) perms[6] = 'x';
    if (*m & S_IROTH) perms[7] = 'r';
    if (*m & S_IWOTH) perms[8] = 'w';
    if (*m & S_IXOTH) perms[9] = 'x';
}

void print_timestamp(const char* label, struct timespec ts) 
{
    struct tm *tm_info;
    char date_part[64];
    
    tm_info = localtime(&ts.tv_sec);
    strftime(date_part, sizeof(date_part), "%Y-%m-%d %H:%M:%S %z", tm_info);
    
    printf("  %s: %.19s.%09ld %s\n", label, date_part, ts.tv_nsec, date_part + 20);
}