#include "filelist.h"
#include <string.h>

file_list *list_create(){
    file_list *list = malloc(sizeof(file_list));
    list->next = NULL;
    list->name = NULL;
    list->path = NULL;
    return list;
}

file_list *list_add(file_list *list, char *name, char *path, FILE_TYPE type, bool mmap){
    while(list->next != NULL) list = list->next;
    list->next = malloc(sizeof(file_list));
    list = list->next;
    list->name = malloc(strlen(name)+1);
    strcpy(list->name, name);
    list->path = malloc(strlen(path)+1);
    strcpy(list->path, path);
    list->mmap = mmap;
    list->next = NULL;
    list->type = type;
    return list;
}

void list_remove_all(file_list *first){
    if(first == NULL) return;
    while(first->next != NULL){
        file_list *prev = first;
        first = first->next;
        free(prev->name);
        free(prev->path);
        free(prev);
    }
    free(first);
}

void list_append(file_list *first, file_list *next){
    while(first->next != NULL) first = first->next;
    first->next = next->next;
    free(next->path);
    free(next->name);
    free(next);
}
