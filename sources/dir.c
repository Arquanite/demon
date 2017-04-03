#include "dir.h"
#include <dirent.h>
#include <stdio.h>

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

file_list list_directory(const char *name){
    // do something :u
}

file_list list_directory_recursive(const char *name){
    // do something :u
}
