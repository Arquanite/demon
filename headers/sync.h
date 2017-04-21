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

/*Usuwa pliki i foldery na podstawie ich obecności w katalogu źródłowym */
void remove_files(char *source_path, char *dest_path);

/*Kopiuje plik za pomocą mmap */
void copy_map(char *source_path, char *dest_path);

/*Zbiera wszystkie funkcje do kupy*/
void sync_all(char *source_path, char *dest_path, bool recursive);

#endif
