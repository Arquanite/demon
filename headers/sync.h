#ifndef SYNC_H
#define SYNC_H

#include <stdlib.h>
#include "filelist.h"

/* Tworzy pusty plik */
void create_file(char *path);

/* Zapisuje cały bufor do file descriptora */
ssize_t write_all (int fd, const void* buffer, size_t count);

/* Kopiuje zawartość jednego pliku i tworzy identyczną kopię */
void read_file(char* path1, char* path2);


void sync_all(char *source_path, char *dest_path);

#endif
