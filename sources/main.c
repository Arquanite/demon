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

#include <fcntl.h>
#include <signal.h>

#include "bool.h"
#include "config.h"
#include "parse.h"
#include "file.h"
#include "sync.h"
#include "filelist.h"
#include "help.h"

bool SYNC_IN_PROGRESS = false;
bool TIME_TO_DIE = false;

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

int lock(){
    int lock_file = open("demon.lock", O_CREAT | O_RDWR, 0666);
    struct flock lockp;
    fcntl(lock_file, F_GETLK, &lockp);
    if(lockp.l_type == 2){
        lockp.l_type = 1;
        if(fcntl(lock_file, F_SETLKW, &lockp) != -1) return 0; // Udało się zablokować
        else return -1; // Nie można zablokować
    }
    else return lockp.l_pid; // Zwraca pid procesu, który założył blokadę
}

void sig_force_sync(){
    if(SYNC_IN_PROGRESS) printf("Wymuszenie w trakcie synchronizacji\n"); //! zmień na syslog
    else printf("Wymuszona synchronizacja\n"); //! zmień na syslog
}

void sig_stop(){
    printf("Bezpieczne zakończenie pracy\n"); //! zmień na syslog
    if(SYNC_IN_PROGRESS) TIME_TO_DIE = true;
    else exit(EXIT_SUCCESS);
}

void sig_kill(){
    printf("Koniec pracy demona\n"); //! zmień na syslog
    exit(EXIT_SUCCESS);
}

void podlacz_kable(){
    signal(SIGUSR1, sig_force_sync);
    signal(SIGUSR2, sig_stop);
    signal(SIGTERM, sig_kill);
}

int main(int argc, char *argv[]){
    int pid = lock();
    podlacz_kable();

    if(argc == 2 && argv[1][0] == '-' && argv[1][1] == 'h'){
        help(true);
        return EXIT_FAILURE;
    }

    if(pid == -1){
        printf("BŁĄÐ: nie można zablokować pliku demon.pid");
        return EXIT_FAILURE;
    }
    if(pid != 0){
        if(argc != 2 || argv[1][0] != '-'){
            printf("Demon jest już uruchomiony, użyj jednej z podanych opcji:\n");
            help_actions();
            return EXIT_FAILURE;
        }
        else {
            switch (argv[1][1]){
            case 'F':
                printf("Wysyłam SIGUSR1\n");
                kill(pid, SIGUSR1);
                return EXIT_SUCCESS;
            case 'S':
                printf("Wysyłam SIGUSR2\n");
                kill(pid, SIGUSR2);
                return EXIT_SUCCESS;
            case 'K':
                printf("Wysyłam SIGTTERM\n");
                kill(pid, SIGTERM);
                return EXIT_SUCCESS;
            }
            return EXIT_FAILURE;
        }
    }

    config c = parse_args(argc, argv);

    if(!c.is_valid){
        printf("BŁĄÐ: nieprawidłowa składnia\n");
        help(false);
        return EXIT_FAILURE;
    }

    if(!(check_directory(c.source_dir) && check_directory(c.dest_dir))){
        printf("Któryś z katalogów nie istnieje. Proszę podać poprawne katalogi.\n");
        return EXIT_FAILURE;
    }

    //sync_all(c);
    //printf("Ukończono synchronizację. Przechodzę w stan uśpienia na %d sekund.\n", c.sleep_time);

//    printf("Odsyłam demona do sali 106...\n"); return 0; //pilnuje demona żeby nie uciek :u

    openlog("demon_log", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Start programu");
    //widelec();
    printf("Wyglada na to, ze dziala\n");

    while(true){
        printf("SYNCHRO\n");
        fflush(stdout);
        SYNC_IN_PROGRESS = true;
        for(int i=0; i<10; i++){
            printf("%d%%\n", (i+1)*10);
            fflush(stdout);
            sleep(1);
        }
        printf("\nPRZERWA\n");
        fflush(stdout);
        SYNC_IN_PROGRESS = false;
        if(TIME_TO_DIE) sig_kill();
        sleep(5);
    }

    closelog();
    return 0;
}
