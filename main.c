#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//funkcja forkująca rodzica
void widelec(){
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
    printf("Wyglada na to, ze dziala\n");
    return 0;
}
