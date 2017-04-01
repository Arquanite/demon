#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h> // size_t
#include "bool.h"

// Struktura odpowiadająca za przechowywanie bieżącej konfiguracji programu

typedef struct {
    bool is_valid; // Określa czy dana konfiguracja jest prawidłowa
    int sleep_time; // Czas 'spania' procesu w sekundach
    bool recursive_sync; // Określa czy synchronizacja ma być rekursywna
    size_t mmap_size_threshold; // Próg odróżniający małe pliki od dużych (w bajtach)
    char *source_dir; // Ścieżka do folderu źródłowego
    char *dest_dir; // Ścieżka do folderu docelowego
} config;

config default_config(); // Funkcja zwracająca domyślną konfigurację

#endif
