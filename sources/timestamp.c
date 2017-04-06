#include "timestamp.h"
#include <sys/stat.h>
#include <utime.h>

void clone_timestamp(char *source_file, char *dest_file){
    struct stat st;
    struct utimbuf new_times;
    stat(source_file, &st);
    new_times.actime = st.st_atim.tv_sec;
    new_times.modtime = st.st_mtim.tv_sec;
    utime(dest_file, &new_times);
    chmod(dest_file, st.st_mode);
}

bool compare_timestamp(char *source_file, char *dest_file){
    struct stat st1;
    struct stat st2;
    stat(source_file, &st1);
    stat(dest_file, &st2);
    return (st1.st_mtim.tv_sec == st2.st_mtim.tv_sec);
}
