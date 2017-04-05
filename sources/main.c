/**
 ** This file is part of the demon666 project.
 ** Copyright 2017 Adrian Kryński & Tobiasz Dzieszkowski <link to github here>.
 ** All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **
 ** 1. Redistributions of source code must retain the above copyright notice,
 **    this list of conditions and the following disclaimer.
 **
 ** 2. Redistributions in binary form must reproduce the above copyright
 **    notice, this list of conditions and the following disclaimer in the
 **    documentation and/or other materials provided with the distribution.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 ** EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 ** DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
 ** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 ** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 ** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 ** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 ** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 ** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 ** SUCH DAMAGE.
 **/

#include <stdio.h>
#include <stdlib.h> // EXIT_SUCCESS i EXIT_FAILURE
//#include <sys/types.h>
#include <sys/stat.h> // umask
#include <syslog.h> // syslogów
#include <dirent.h> // opendir
#include <unistd.h> // fork
#include <string.h>

#include "bool.h"
#include "config.h"
#include "parse.h"
#include "dir.h"
#include "sync.h"
#include "filelist.h"

//funkcja forkująca rodzica
void widelec(){
    syslog(LOG_INFO, "Użycie widelca");
    //forkowanie rodzica
    pid_t pid, sid;
    pid = fork();
    if(pid < 0){
        syslog(LOG_ERR, "Error: could not fork parent process");
        exit(EXIT_FAILURE);
    }
    if(pid > 0){
        exit(EXIT_SUCCESS);
    }
    //zmiana maski plików
    umask(0);
    //tworzenie SIDa dla dziecka
    sid = setsid();
    if(sid < 0){
        syslog(LOG_ERR, "Error: could not create sid for child process");
        exit(EXIT_FAILURE);
    }
    //zmiana katalogu
    if((chdir("/")) < 0) {
        syslog(LOG_ERR, "Error: could not change working directory");
    }
    //zamykanie STD
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //TU TRZEBA ZROBIĆ INICJALIZACJĘ NASZEGO DEMONA (specyficzna dla naszego)
}

int main(int argc, char *argv[]){
    /*if(argc < 3){
        printf("Jesteś debilem debilu :u\n");
        return 1;
    }*/

    config c = parse_args(argc, argv);

    // debug info, wyjebuje się kiedy nie podasz folderów, nie jest to problem, bo domyślnie się to nie wyświetla
    printf("is_valid: %d \n"
           "sleep_time: %d \n"
           "recursive_sync: %d \n"
           "mmap_size_threshold: %zd \n"
           "source_dir: %s \n"
           "dest_dir: %s \n"
           ,c.is_valid, c.sleep_time, c.recursive_sync, c.mmap_size_threshold, c.source_dir, c.dest_dir);

    if(!c.is_valid){
        printf("BŁĄÐ: nieprawidłowa składnia\n");
        return EXIT_FAILURE;
    }

    if(!(check_directory(c.source_dir) && check_directory(c.dest_dir))){
        return EXIT_FAILURE;
    }

    // Przykład użycia listy

    file_list *list = list_create();
    list_add(list, "ddd", "ffff", true);
    list_add(list, "121", "123", true);
    list_add(list, "1234", "1233", true);

    file_list *bb = list_create();
    list_add(bb, "121bbbb", "1bbb23", true);
    list_add(bb, "bbbb1234", "123bbb3", true);

    list_append(list, bb);

    file_list *begin = list;
    while(list->next != NULL){
        list = list->next;
        printf("%s\n", list->name);
    }

    list_remove_all(begin);

    // End of przykład
    list = list_directory_recursive(c.source_dir);
    begin = list;
    while(list->next != NULL){
        list = list->next;
        printf("%s/%s\n", list->path, list->name);
    }
    list_remove_all(begin);

    printf("Odsyłam demona do sali 106...\n"); return 0; //pilnuje demona żeby nie uciek :u

 /*   openlog("demon_log", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Start programu");
    widelec();
    printf("Wyglada na to, ze dziala\n");

    while(true){
        syslog(LOG_INFO, "Biegam sobie xd");
        sleep(5);
        syslog(LOG_INFO, "Dalej se biegam");
    }

    closelog(); */

    return 0;
}

