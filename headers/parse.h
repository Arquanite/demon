#ifndef PARSE_H
#define PARSE_H

#include <stdlib.h> // size_t
#include "config.h"

/* Zamienia czas z postaci tekstowej np. 30, 20s, 15m czy 12h
 * na postać całkowitoliczbową wyrażoną w sekundach */
int parse_time(const char *time);

/* Zamienia rozmiar z postaci tekstowej np. 23, 3k, 4M czy 1G
 * na postać całkowitoliczbową wyrażoną w bajtach */
size_t parse_size(const char *size);

/* Na podstawie parametrów przekazanych do programu generuje
 * strukturę config określającą jego bieżącą konfigurację */
config parse_args(int argc, char *argv[]);

#endif
