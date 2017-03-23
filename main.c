#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
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
