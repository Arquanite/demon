#include "sync.h"
#include "file.h"
#include "timestamp.h"

#include <fcntl.h> //open, read
#include <sys/mman.h> //mmap
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //close
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <dirent.h> //directory

void create_file(char* path){
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH; // file permissions
    int fd = open (path, O_WRONLY | O_EXCL | O_CREAT, mode); //create file
    if (fd == -1) {
        printf("An error has occurred");
                perror("open");
        exit(EXIT_FAILURE);
    }
    close(fd);
}

ssize_t write_all (int fd, const void* buffer, size_t count) {
    size_t left_to_write = count;
    while (left_to_write > 0) {
        size_t written = write (fd, buffer, count);
        if (written == -1)
            /* An error occurred; bail.  */
            return -1;
        else
            /* Keep count of how much more we need to write.  */
            left_to_write -= written;
    }
    /* We should have written no more than COUNT bytes!   */
    assert (left_to_write == 0);
    /* The number of bytes written is exactly COUNT.  */
    return count;
}

void read_file(char* path1, char* path2){
    unsigned char buffer[16];
    size_t offset = 0;
    size_t bytes_read;
    int i;
    //open the file for reading
    int fd = open (path1, O_RDONLY);
    create_file (path2);
    int fd2 = open (path2, O_WRONLY);
    //read part by part until reads less than it should
    do {
        bytes_read = read(fd, buffer, sizeof(buffer));
        //print offset follwed by bytes
//        printf ("0x%zx :", offset);
        write_all(fd2, buffer, bytes_read);
//        printf ("\n");
        //keep count of position in file
        offset += bytes_read;
    } while(bytes_read == sizeof(buffer));
    //done, close file descriptor
    close(fd);
    close(fd2);
}

void remove_files(char *source_path, char *dest_path){
    file_list *list = read_directory(dest_path, true);
    file_list *reversed = list_reverse(list);
    file_list *begin = reversed;
    list_remove_all(list);
    while(reversed->next != NULL){
        reversed = reversed->next;
        int len = strlen(reversed->path) + strlen(reversed->name) - strlen(dest_path) + strlen(source_path) + 2;
        char source_file[len];
        snprintf(source_file, len, "%s%s/%s", source_path, reversed->path + strlen(dest_path), reversed->name);
        len = strlen(reversed->path) + strlen(reversed->name) + 3;
        char file_to_remove[len];
        snprintf(file_to_remove, len, "%s/%s", reversed->path, reversed->name);
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
    list_remove_all(begin);
}

void copy_map(char *source_path, char *dest_path){
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


void sync_all(char *source_path, char *dest_path, bool recursive){
    remove_files(source_path, dest_path);
    file_list *list = read_directory(source_path, recursive);
    file_list *begin = list;
    while(list->next != NULL){
        list = list->next;
        int len = strlen(list->path) + strlen(list->name) - strlen(source_path) + strlen(dest_path) + 2;
        char full_dest_path[len];
        snprintf(full_dest_path, len, "%s%s/%s", dest_path, list->path + strlen(source_path), list->name);
        len = strlen(list->path);
        len += strlen(list->name) + 2;
        char full_path[len];
        snprintf(full_path, len, "%s/%s", list->path, list->name);

        if(exists(full_dest_path)){
            if(list->type == DIRECTORY){
                if(!compare_timestamp(full_path, full_dest_path)){
                    clone_timestamp(full_path, full_dest_path);
                }
            }
            else if(list->type == REGULAR_FILE){
                if(!compare_timestamp(full_path, full_dest_path)){
                    remove(full_dest_path);
                    read_file(full_path, full_dest_path);
                    clone_timestamp(full_path, full_dest_path);
                }
            }
        }
        else{
            if(list->type == DIRECTORY){
                mkdir(full_dest_path, 0700);
                clone_timestamp(full_path, full_dest_path);
            }
            else{
                read_file(full_path, full_dest_path);
                clone_timestamp(full_path, full_dest_path);
            }
        }
//        if(access(full_dest_path, F_OK) == -1){
//            if(compare_timestamp(full_path, full_dest_path)){
//                if(list->type == DIRECTORY){
//                    mkdir(full_dest_path, 0700);
//                    clone_timestamp(full_path, full_dest_path);
//                }
//                else{
//                   read_file(full_path, full_dest_path);
//                   clone_timestamp(full_path, full_dest_path);
//               }
//               printf("dupa %s : %d\n", full_dest_path, list->type);
//            }
//        }

    }
    list_remove_all(begin);
}

