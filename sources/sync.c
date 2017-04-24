#include "sync.h"
#include "file.h"
#include "timestamp.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>



void sync_all(config c){
    /* Usuwa pliki których nie ma */
    remove_files(c);

    /* Czytanie plików do synchronizacji do listy
     * oraz tworzenie punktu powrotu (begin) */
    file_list *list = read_directory(c.source_dir, c.recursive_sync);
    file_list *begin = list;

    while(list->next != NULL){
        list = list->next;
        bool mmap_on;

        /* Tworzenie pełnej ścieżki docelowej pliku */
        int len = strlen(list->path) + strlen(list->name) - strlen(c.source_dir) + strlen(c.dest_dir) + 2;
        char full_dest_path[len];
        snprintf(full_dest_path, len, "%s%s/%s", c.dest_dir, list->path + strlen(c.source_dir), list->name);

        /* Tworzenie pełnej ścieżki źródłowej pliku */
        len = strlen(list->path);
        len += strlen(list->name) + 2;
        char full_source_path[len];
        snprintf(full_source_path, len, "%s/%s", list->path, list->name);

        /* Sprawdzamy rozmiar pliku */
        struct stat st;
        stat(full_source_path, &st);
        int size = st.st_size;
        printf("%d\n", size);
        if(size >= c.mmap_size_threshold) mmap_on = true;


        /* Porównywanie timestamp'ów oraz aktualizacja
         * plików na ich podstawie */
        if(exists(full_dest_path)){
            if(list->type == DIRECTORY){
                if(!compare_timestamp(full_source_path, full_dest_path)){
                    clone_timestamp(full_source_path, full_dest_path);
                }
            }
            else if(list->type == REGULAR_FILE){
                if(!compare_timestamp(full_source_path, full_dest_path)){
                    remove(full_dest_path);
                    copy_file(full_source_path, full_dest_path, mmap_on);
                    clone_timestamp(full_source_path, full_dest_path);
                }
            }
        }
        /* Kopiowanie pozostałych plików i/lub katalogów */
        else{
            if(list->type == DIRECTORY){
                mkdir(full_dest_path, 0700);
                clone_timestamp(full_source_path, full_dest_path);
            }
            else{
                copy_file(full_source_path, full_dest_path, mmap_on);
                clone_timestamp(full_source_path, full_dest_path);
            }
        }
    }
    /* Usuwanie listy */
    list_remove_all(begin);
}

