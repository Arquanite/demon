#ifndef FILELIST_H
#define FILELIST_H

#include <stdlib.h> // NULL
#include "bool.h"

/* Lista, służy do przechowywania listy plików w katalogu */

typedef struct {
    char *name; // nazwa pliku/katalogu
    bool mmap; // czy plik powinien być kopiowany za pomocą mmap
    struct file_list *next; // wskaźnik na następny element
} file_list;

/* Dodaje nowy element do listy, zwraca adres ostatniego elementu */
file_list* add(file_list *list, char *name, bool mmap);

/* Usuwa wszystkie elementy listy */
void remove_all(file_list *first);

#endif
