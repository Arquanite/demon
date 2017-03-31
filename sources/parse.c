#include "parse.h"

#include <string.h>
#include <stdio.h>

int parse_time(const char *time){
    int seconds = 0;
    for(int i=0; i<strlen(time)-1; i++){
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

size_t parse_size(const char *size){
    size_t bytes = 0;
    for(int i=0; i<strlen(size)-1; i++){
        if(size[i] >=48 && size[i] <= 57){
            bytes *= 10;
            bytes += (size[i]-48);
        }
        else return -1;
    }
    char last = size[strlen(size)-1];
    if(last >=48 && last <= 57){
        bytes *= 10;
        bytes += (last-48);
    }
    else {
        switch (last) {
        case 'K':
        case 'k':
            bytes *= 1024;
            break;

        case 'M':
        case 'm':
            bytes *= 1024*1024;
            break;

        case 'G':
        case 'g':
            bytes *= 1024*1024*1024;
            break;

        default:
            return -1;
            break;
        }
    }
    return bytes;
}

config parse_args(int argc, char *argv[]){
    config c;// = default_config();
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
                c.mmap_size_threshold = parse_size(argv[i+1]);
                if(c.mmap_size_threshold == -1){
                    printf("BŁĄÐ: nieprawidłowy format rozmiaru: %s\n", argv[i+1]);
                    return c;
                }
                i++;
                break;

            default:
                printf("BŁĄÐ: nieznana opcja %c\n", opt);
                return c;
                break;
            }
        }
        else {
            if(dir_count == 0) c.source_dir = arg;
            else if(dir_count == 1) c.dest_dir = arg;
            dir_count++;
        }
    }
    if(dir_count == 2) c.is_valid = true;
    return c;
}
