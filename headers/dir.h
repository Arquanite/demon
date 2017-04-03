#ifndef DIR_H
#define DIR_H

#include "bool.h"
#include "filelist.h"

/* Sprawdza czy podany katalog rzeczywiście jest katalogiem,
 * czy istnieje oraz czy użytkownik ma prawo otworzyć ten katalog.
 * w razie błędu wyświetla komunikat na stdout. */
bool check_directory(const char *name);

/* Sprawdza czy podany katalog rzeczywiście jest katalogiem,
 * czy istnieje oraz czy użytkownik ma prawo otworzyć ten katalog.
 * Nie wyświetla żadnych komunikatów. */
bool is_directory(const char *name);

/* Zwraca zawartość katalogu podanego jako parametr. */
file_list list_directory(const char *name);

/* Zwraca zawartość katalogu podanego jako parametr
 * oraz jego wszystkich podkatalogów. */
file_list list_directory_recursive(const char *name);

#endif
