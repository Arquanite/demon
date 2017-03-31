#include <stdio.h>
#include <stdlib.h> // EXIT_SUCCESS i EXIT_FAILURE
//#include <sys/types.h>
#include <sys/stat.h> // umask
#include <syslog.h> // syslogów
#include <dirent.h> // opendir
#include <unistd.h> // fork
#include <string.h>

#define true 1
#define false 0
typedef int bool;

typedef struct {
    bool is_valid;
    int sleep_time;
    bool recursive_sync;
    int mmap_size_threshold;
    char *source_dir;
    char *dest_dir;
} config;

const config DEFAULT_CONFIG = { false, 300, false, 4194304, NULL, NULL }; // nope, 300s = 5min, false, 4194304 = 2^22 = 4MB, none, none

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

int check_directory(const char *name){
    DIR* dir = opendir(name);
    if(dir){
        printf("Katalog %s istnieje i ma się dobrze\n", name);
        closedir(dir);
        return 1;
    }
    else {
        perror(name);
        return 0;
    }
}

int parse_time(const char *time){
    int seconds = 0;
    for(int i; i<strlen(time)-1; i++){
        if(time[i] >=48 && time[i] <= 57){
            seconds *= 10;
            seconds += (time[i]-48);
        }
        else return -1;
    }
    char last = time[strlen(time)-1];
    if(last >=48 && last <= 57){
        seconds *= 10;
        seconds += (last-48);
    }
    else {
        switch (last) {
        case 's':
            // nothing lol
            break;
        case 'm':
            seconds *= 60;
            break;
        case 'h':
            seconds *= 3600; // 60s * 60m = 1h
            break;
        default:
            return -1;
            break;
        }
    }
    return seconds;
}

config parse_args(int argc, char *argv[]){
    config c = DEFAULT_CONFIG;
    if(argc <3) return c;
    int dir_count = 0;
    for(int i=1; i<argc; i++){
        char *arg = argv[i];
        if(arg[0] == '-'){
            char opt = arg[1];
            if(strlen(arg) != 2){
                printf("BŁĄÐ: nieznana opcja: %s\n", arg);
                return c;
            }
            switch (opt) {
            case 's':
                c.sleep_time = parse_time(argv[i+1]);
                if(c.sleep_time == -1){
                    printf("BŁĄÐ: nieprawidłowy format czasu: %s\n", argv[i+1]);
                    return c;
                }
                i++;
                break;

            case 'R':
                c.recursive_sync = true;
                break;

            case 'm':
                c.sleep_time = strtol(argv[i+1], NULL, 10);
                break;

            default:
                printf("BŁĄÐ: nieznana opcja %c\n", opt);
                return c;
                break;
            }
        }
        else {
            printf("%s\n", arg);
        }
    }
    c.is_valid = true;
    return c;
}

int main(int argc, char *argv[]){
    /*if(argc < 3){
        printf("Jesteś debilem debilu :u\n");
        return 1;
    }*/

    config c = parse_args(argc, argv);

    printf("is_valid: %d \n"
           "sleep_time: %d \n"
           "recursive_sync: %d \n"
           "mmap_size_threshold: %d \n"
           ,c.is_valid, c.sleep_time, c.recursive_sync, c.mmap_size_threshold);

    if(!c.is_valid){
        printf("BŁĄÐ: nieprawidłowa składnia\n");
    }
    return 666;
    if(!(check_directory(argv[1]) && check_directory(argv[2]))){
        return 1;
    }

    openlog("demon_log", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Start programu");
    widelec();
    printf("Wyglada na to, ze dziala\n");

    while(1){
        syslog(LOG_INFO, "Biegam sobie xd");
        sleep(5);
        syslog(LOG_INFO, "Dalej se biegam");
    }

    closelog();
    return 0;
}

