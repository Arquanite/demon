#ifndef SYNC_H
#define SYNC_H

#include <stdlib.h>

/* Tworzy pusty plik */
void create_file(char *path);

/* Zapisuje cały bufor do file descriptora */
ssize_t write_all (int fd, const void* buffer, size_t count);

/* Kopiuje zawartość jednego pliku i tworzy identyczną kopię */
void read_file(char* path1, char* path2);

/* Wypisuje zawartość katalogu */
void read_directory(char *path);

/* Wypisuje rekursywnie zawartość katalogu i wszystkich jego podkatalogów */
void read_directory_recursive(char *path);

#endif
