#include "sync.h"
#include "file.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>

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
    do{
        bytes_read = read (fd, buffer, sizeof (buffer));
        //print offset follwed by bytes
        printf ("0x%zx :", offset);
        write_all (fd2, buffer, 16);
        for (i=0; i<bytes_read; ++i){
            printf ("%02x ", buffer[i]);
            //write_all (fd2, buffer[i], 16);
        }
        printf ("\n");
        //keep count of position in file
        offset += bytes_read;
    } while (bytes_read == sizeof (buffer));
    //done, close file descriptor
    close(fd);
    close(fd2);
}

void sync_all(char *source_path, char *dest_path){
    file_list *list = list_directory_recursive(source_path);
    file_list *begin = list;
    while(list->next != NULL){
        list = list->next;
        int len = strlen(list->path) + strlen(list->name) - strlen(source_path) + strlen(dest_path) + 2;
        char tmp[len];
        snprintf(tmp, len, "%s%s/%s", dest_path, list->path + strlen(source_path), list->name);
        printf("dupa %s : %d\n", tmp, list->type);
    }
}
