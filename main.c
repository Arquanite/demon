#include <stdio.h>
#include <stdlib.h> // EXIT_SUCCESS i EXIT_FAILURE
//#include <sys/types.h>
#include <sys/stat.h> // umask
#include <syslog.h>  // syslogów
#include <unistd.h> // fork

//funkcja forkująca rodzica
void widelec(){
	syslog(LOG_INFO, "Użycie widelca");
	
	pid_t pid, sid;
	pid = fork();
	if(pid < 0){
		syslog(LOG_ERR, "Error: could not fork parent process");
		exit(EXIT_FAILURE);
	}
	if(pid > 0){
		exit(EXIT_SUCCESS);
	}
	umask(0);

	sid = setsid();
	if(sid < 0){
		syslog(LOG_ERR, "Error: could not create sid for child process");
		exit(EXIT_FAILURE);
	}

	if((chdir("/")) < 0) {
		syslog(LOG_ERR, "Error: could not change working directory");
	}
}

int main(int argc, int *argv[]){
	if(argc < 3){
		printf("Jesteś debilem debilu :u\n");
		return 1;
	}
	openlog("demon_log", LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "Start programu");
	widelec();
	printf("Wyglada na to, ze dziala\n");

	closelog();
	return 0;
}
