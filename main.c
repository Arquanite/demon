#include <stdio.h>
#include <stdlib.h>

//funkcja forkująca rodzica
widelec(){
    pid_t pid;
    pid = fork();
    if(pid < 0) {
        exit(EXIT_FAILURE);
    }
    if(pid > 0){
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, int *argv[]){
    printf("Wyglada na to, ze dziala\n");
    return 0;
}
