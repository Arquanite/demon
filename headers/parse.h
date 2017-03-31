#ifndef PARSE_H
#define PARSE_H

#include <stdlib.h>
#include "config.h"

int parse_time(const char *time);
size_t parse_size(const char *size);
config parse_args(int argc, char *argv[]);

#endif
