#ifndef DIR_H
#define DIR_H

#include "bool.h"
#include "filelist.h"
#include "filetype.h"
#include "config.h"

/* Sprawdza jakiego typu jest plik podany w parametrze:
 * symlink, katalog, zwykły plik itp. */
FILE_TYPE get_file_type(const char* path);

/* Sprawdza czy podany katalog rzeczywiście jest katalogiem,
 * czy istnieje oraz czy użytkownik ma prawo otworzyć ten katalog.
 * w razie błędu wyświetla komunikat na stdout. */
bool check_directory(const char *name);

/* Sprawdza czy jeden z katalogów zawiera drugi */
bool contains(const char *dir1, const char *dir2);

/* Sprawdza czy podany katalog rzeczywiście jest katalogiem,
 * czy istnieje oraz czy użytkownik ma prawo otworzyć ten katalog.
 * Nie wyświetla żadnych komunikatów. */
bool is_directory(const char *name);

/* Sprawdza czy plik istnieje */
bool exists(const char *name);

/* Zwraca zawartość katalogu podanego jako parametr
 * oraz jego wszystkich podkatalogów. */
file_list* read_directory(char *path, bool recursive);

/* Tworzy pusty plik */
void create_file(char *path);

/* Kopiuje zawartość jednego pliku i tworzy identyczną kopię */
void copy_file_rw(char* source_path, char* dest_path);

/*Kopiuje plik za pomocą mmap */
void copy_file_mmap(char *source_path, char *dest_path);

/* Wybór odpowiedniego kopiowania */
void copy_file(char *source_path, char *dest_path, bool mmap_on);

/* Zapisuje cały bufor do file descriptora */
ssize_t write_all (int fd, const void* buffer, size_t count);

/*Usuwa pliki i foldery na podstawie ich obecności w katalogu źródłowym */
void remove_files(config c);


#endif
