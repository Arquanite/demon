#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include "bool.h"

typedef struct {
    bool is_valid;
    int sleep_time;
    bool recursive_sync;
    size_t mmap_size_threshold;
    char *source_dir;
    char *dest_dir;
} config;

config default_config();

#endif
