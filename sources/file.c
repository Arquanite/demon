#include "file.h"

#include <dirent.h> //katalogi
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h> //close
#include <fcntl.h> //open, read
#include <sys/mman.h> //mmap

FILE_TYPE get_file_type(const char* path){
    struct stat st;
    stat(path, &st);
    if(S_ISLNK(st.st_mode)) return SYMBOLIC_LINK;
    else if(S_ISDIR(st.st_mode)) return DIRECTORY;
    else if(S_ISCHR(st.st_mode)) return CHARACTER_DEVICE;
    else if(S_ISBLK(st.st_mode)) return BLOCK_DEVICE;
    else if(S_ISFIFO(st.st_mode))return FIFO;
    else if(S_ISSOCK(st.st_mode))return SOCKET;
    else if(S_ISREG(st.st_mode)) return REGULAR_FILE;
    else return UNKNOWN;
}

bool check_directory(const char *name){
    DIR* dir = opendir(name);
    if(dir){
        closedir(dir);
        return true;
    }
    else {
        perror(name);
        return false;
    }
}

bool is_directory(const char *name){
    DIR* dir = opendir(name);
    if(dir){
        closedir(dir);
        return true;
    }
    return false;
}

bool exists(const char *name){
    if(access(name, F_OK) == 0) return true;
    else return false;
}

file_list* read_directory(char *path, bool recursive){
    file_list *list = list_create();
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        int len = strlen(path);
        len += strlen(entry->d_name) + 2;
        char full_path[len];
        snprintf(full_path, len, "%s/%s", path, entry->d_name);
        FILE_TYPE type;
        type = get_file_type(full_path);
        if(type == REGULAR_FILE) list_add(list, entry->d_name, path, type, true);
        if(type == DIRECTORY && recursive){
            list_add(list, entry->d_name, path, type, true);
            list_append(list, read_directory(full_path, true));
        }
    }
    closedir(dir);
    return list;
}

void create_file(char* path){
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH; // file permissions
    int fd = open (path, O_WRONLY | O_EXCL | O_CREAT, mode); // tworzenie pliku
    if (fd == -1) {
        printf("An error has occurred");
                perror("open");
        exit(EXIT_FAILURE);
    }
    close(fd);
}

void copy_file_rw(char* source_path, char* dest_path){
    unsigned char buffer[16];
    size_t offset = 0;
    size_t bytes_read;
    int i;

    /* Otwieranie pliku źródłowego */
    int sfd = open (source_path, O_RDONLY);

    /* Tworzenie pliku docelowego */
    create_file (dest_path);
    int dfd = open (dest_path, O_WRONLY);
    /* Czytanie częściami do momentu gdy część
     * jest mniejsza niż powinna */
    do {
        bytes_read = read(sfd, buffer, sizeof(buffer));
        write_all(dfd, buffer, bytes_read);

        /* Pilnowanie pozycji w pliku */
        offset += bytes_read;
    } while(bytes_read == sizeof(buffer));
    /* Zamykanie file descriptor */
    close(sfd);
    close(dfd);
}

void copy_file_mmap(char *source_path, char *dest_path){
    /* Deklaracje zmiennych */
    int sfd, dfd;
    char *src, *dest;
    struct stat s;
    size_t filesize;

    /* Plik źródłowy */
    sfd = open(source_path, O_RDONLY);
    filesize = lseek(sfd, 0, SEEK_END);
    src = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, sfd, 0);

    /* Plik docelowy */
    dfd = open(dest_path, O_RDWR | O_CREAT, 0666);
    ftruncate(dfd, filesize);
    dest = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dfd, 0);

    /* Kopiowanie */
    memcpy(dest, src, filesize);
    munmap(src, filesize);
    munmap(dest, filesize);

    close(sfd);
    close(dfd);
}

void copy_file(char *source_path, char *dest_path, bool mmap_on){
    if(mmap_on) copy_file_mmap(source_path, dest_path);
    else copy_file_rw(source_path, dest_path);
}

ssize_t write_all (int fd, const void* buffer, size_t count) {
    size_t left_to_write = count;
    while (left_to_write > 0) {
        size_t written = write (fd, buffer, count);
        if (written == -1)
            /* Błąd */
            return -1;
        else
            /* Pilnowanie ile jescze zostało do zapisu  */
            left_to_write -= written;
    }
    /* Pilnowanie żeby nie zapisać za dużo   */
    assert (left_to_write == 0);
    /* Liczba zapisanych bajtów to count  */
    return count;
}

void remove_files(config c){
    /* Tworzenie i odwrócenie listy z plikami
     * katalogu docelowego oraz tworzenie punktu
     * powrotu (begin) */
    file_list *list = read_directory(c.dest_dir, c.recursive_sync);
    file_list *reversed = list_reverse(list);
    file_list *begin = reversed;
    list_remove_all(list);

    while(reversed->next != NULL){
        reversed = reversed->next;

        /* Tworzenie pełnej ścieżki pliku z katalogu źródłowego */
        int len = strlen(reversed->path) + strlen(reversed->name) - strlen(c.dest_dir) + strlen(c.source_dir) + 2;
        char source_file[len];
        snprintf(source_file, len, "%s%s/%s", c.source_dir, reversed->path + strlen(c.dest_dir), reversed->name);

        /* Tworzenie pełnej ścieżki pliku z katalogu docelowego */
        len = strlen(reversed->path) + strlen(reversed->name) + 3;
        char file_to_remove[len];
        snprintf(file_to_remove, len, "%s/%s", reversed->path, reversed->name);

        /* Jeżeli plik nie istnieje w katalogu źródłowym
         * jest on usuwany z katalogu docelowego */
        if(!exists(source_file)){
            // printf("Usuwam: %s\n", file_to_remove);   TODO sysloga
            if(reversed->type == DIRECTORY){
                rmdir(file_to_remove);
            }
            else {
                remove(file_to_remove);
            }
        }
    }
    /* Usuwanie listy */
    list_remove_all(begin);
}
