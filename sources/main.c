#include <stdio.h>
#include <stdlib.h> // EXIT_SUCCESS i EXIT_FAILURE
//#include <sys/types.h>
#include <sys/stat.h> // umask
#include <syslog.h> // syslogów
#include <dirent.h> // opendir
#include <unistd.h> // fork
#include <string.h>
#include <sys/file.h>
#include <errno.h>

#include "bool.h"
#include "config.h"
#include "parse.h"
#include "file.h"
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

}

int main(int argc, char *argv[]){

    config c = parse_args(argc, argv);

    if(!c.is_valid){
        printf("BŁĄÐ: nieprawidłowa składnia\n");
        return EXIT_FAILURE;
    }

    if(!(check_directory(c.source_dir) && check_directory(c.dest_dir))){
        printf("Któryś z katalogów nie istnieje. Proszę podać poprawne katalogi.\n");
        return EXIT_FAILURE;
    }

    int pid_file = open("/tmp/demon.pid", O_CREAT | O_RDWR, 0666);
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if(rc){
        if(EWOULDBLOCK == errno){
            printf("Instancja już istnieje\n");
            return 0;
        }
        else{
            printf("Uruchomiłeś szatana\n");
        }
    }

    FILE *fp = fopen("/tmp/demon2.pid", "w");
    fprintf(fp, "%d", getpid());
    close(fp);

    sync_all(c);
    printf("Ukończono synchronizację. Przechodzę w stan uśpienia na %d sekund.\n", c.sleep_time);

    printf("Odsyłam demona do sali 106...\n"); return 0; //pilnuje demona żeby nie uciek :u

    openlog("demon_log", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Start programu");
    widelec();
    printf("Wyglada na to, ze dziala\n");

    while(true){
        syslog(LOG_INFO, "Biegam sobie xd");
        sleep(5);
        syslog(LOG_INFO, "Dalej se biegam");
    }

    closelog();

    return 0;
}

