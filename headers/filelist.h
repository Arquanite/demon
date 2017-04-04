#ifndef FILELIST_H
#define FILELIST_H

#include <stdlib.h> // NULL
#include "bool.h"

/* Lista, służy do przechowywania listy plików w katalogu */

typedef struct file_list file_list;

struct file_list {
    char *name; // nazwa pliku/katalogu
    char *path; // ścieżka do katalogu nadrzędnego
    bool mmap; // czy plik powinien być kopiowany za pomocą mmap
    struct file_list *next; // wskaźnik na następny element
};

file_list *list_create();

/* Dodaje nowy element do listy, zwraca adres ostatniego elementu */
file_list *list_add(file_list *list, char *name, char *path, bool mmap);

/* Usuwa wszystkie elementy listy */
void list_remove_all(file_list *first);

#endif
