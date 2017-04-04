#include "filelist.h"

file_list *list_create(){
    file_list *list = malloc(sizeof(file_list));
    list->next = NULL;
    return list;
}

file_list *list_add(file_list *list, char *name, char *path, bool mmap){
    while(list->next != NULL) list = list->next;
    list->next = malloc(sizeof(file_list));
    list = list->next;
    list->name = name;
    list->path = path;
    list->mmap = mmap;
    list->next = NULL;
    return list;
}

void list_remove_all(file_list *first){
    if(first == NULL) return;
    while(first->next != NULL){
        file_list *prev = first;
        first = first->next;
        free(prev);
    }
    free(first);
}
