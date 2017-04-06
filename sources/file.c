#include "file.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

FILE_TYPE get_file_type(const char* path){
    struct stat st;
    lstat(path, &st);
    if(S_ISLNK(st.st_mode)) return SYMBOLIC_LINK;
    else if(S_ISDIR(st.st_mode)) return DIRECTORY;
    else if(S_ISCHR(st.st_mode)) return CHARACTER_DEVICE;
    else if(S_ISBLK(st.st_mode)) return BLOCK_DEVICE;
    else if(S_ISFIFO(st.st_mode))return FIFO;
    else if(S_ISSOCK(st.st_mode))return SOCKET;
    else if(S_ISREG(st.st_mode)) return REGULAR_FILE;
    else return UNKNOWN;
}

bool check_directory(const char *name){
    DIR* dir = opendir(name);
    if(dir){
        printf("Katalog %s istnieje i ma się dobrze\n", name);
        closedir(dir);
        return true;
    }
    else {
        perror(name);
        return false;
    }
}

bool is_directory(const char *name){
    DIR* dir = opendir(name);
    if(dir){
        closedir(dir);
        return true;
    }
    return false;
}

file_list* read_directory(char *path, bool recursive){
    file_list *list = list_create();
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        int len = strlen(path);
        len += strlen(entry->d_name) + 2;
        char full_path[len];
        snprintf(full_path, len, "%s/%s", path, entry->d_name);
        FILE_TYPE type;
        type = get_file_type(full_path);
        if(type == REGULAR_FILE) list_add(list, entry->d_name, path, type, true);
        if(type == DIRECTORY && recursive){
            list_add(list, entry->d_name, path, type, true);
            list_append(list, read_directory(full_path, true));
        }
    }
    closedir(dir);
    return list;
}
