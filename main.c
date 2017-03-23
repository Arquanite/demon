#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <dirent.h> // opendir
#include <unistd.h>

//funkcja forkująca rodzica
void widelec(){
	syslog(LOG_INFO, "Użycie widelca");
	pid_t pid, sid;
	pid = fork();
	if(pid < 0) {
		exit(EXIT_FAILURE);
	}
	if(pid > 0){
		exit(EXIT_SUCCESS);
	}
	umask(0);
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

int main(int argc, char *argv[]){
	if(argc < 3){
		printf("Jesteś debilem debilu :u\n");
		return 1;
	}
	
	if(!(check_directory(argv[1]) && check_directory(argv[2]))){
		return 1;
	}
	
	openlog("demon_log", LOG_PID | LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "Start programu");
	widelec();

	closelog();
	return 0;
}
