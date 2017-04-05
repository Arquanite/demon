#ifndef DIR_H
#define DIR_H

#include "bool.h"
#include "filelist.h"

typedef enum {
    SYMBOLIC_LINK,
    DIRECTORY,
    CHARACTER_DEVICE,
    BLOCK_DEVICE,
    FIFO,
    SOCKET,
    REGULAR_FILE,
    UNKNOWN
} FILE_TYPE;

/* Sprawdza jakiego typu jest plik podany w parametrze:
 * symlink, katalog, zwykły plik itp. */
FILE_TYPE get_file_type(const char* path);

/* Sprawdza czy podany katalog rzeczywiście jest katalogiem,
 * czy istnieje oraz czy użytkownik ma prawo otworzyć ten katalog.
 * w razie błędu wyświetla komunikat na stdout. */
bool check_directory(const char *name);

/* Sprawdza czy podany katalog rzeczywiście jest katalogiem,
 * czy istnieje oraz czy użytkownik ma prawo otworzyć ten katalog.
 * Nie wyświetla żadnych komunikatów. */
bool is_directory(const char *name);

/* Zwraca zawartość katalogu podanego jako parametr. */
file_list *list_directory(const char *path);

/* Zwraca zawartość katalogu podanego jako parametr
 * oraz jego wszystkich podkatalogów. */
file_list *list_directory_recursive(char *path);

#endif
