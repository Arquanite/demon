#include "filelist.h"

file_list *add(file_list *list, char *name, char *path, bool mmap){
    while(list->next != NULL) list = (file_list*)list->next;
    list->next = malloc(sizeof(file_list));
    list = (file_list*)list->next;
    list->name = name;
    list->path = path;
    list->mmap = mmap;
    list->next = NULL;
    return list;
}

void remove_all(file_list *first){
    if(first == NULL) return;
    while(first->next != NULL){
        file_list *prev = first;
        first = (file_list*)first->next;
        free(prev);
    }
    free(first);
}
