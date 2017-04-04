#include "dir.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

const char* get_file_type (const char* path)
{
  struct stat st;
  lstat (path, &st);
  if (S_ISLNK (st.st_mode))
    return "symbolic link";
  else if (S_ISDIR (st.st_mode))
    return "directory";
  else if (S_ISCHR (st.st_mode))
    return "character device";
  else if (S_ISBLK (st.st_mode))
    return "block device";
    else if (S_ISFIFO (st.st_mode))
      return "fifo";
    else if (S_ISSOCK (st.st_mode))
      return "socket";
    else if (S_ISREG (st.st_mode))
      return "regular file";
    else
      /* Unexpected. Each entry should be one of the types above.  */
      assert (0);
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

file_list list_directory(const char *name){
    DIR *dir;
    struct dirent *entry;
    char entry_path[PATH_MAX + 1];
    size_t path_len;
    strncpy(entry_path, name, sizeof(entry_path));
    path_len = strlen(name);
    if (entry_path[path_len - 1] != '/'){
        entry_path[path_len] = '/';
        entry_path[path_len + 1] = '\0';
        ++path_len;
    }
    dir = opendir(name);
    while ((entry = readdir(dir)) != NULL){
        const char *type;
        strncpy(entry_path + path_len, entry->d_name, sizeof(entry_path) - path_len);
        type = get_file_type(entry_path);
        printf("%-18s: %s\n", type, entry_path);
    }
    closedir(dir);
}

file_list list_directory_recursive(const char *name){
    // do something :u
}
