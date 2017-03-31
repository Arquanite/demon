#include "config.h"

config default_config(){
    config DEFAULT_CONFIG = { false, 300, false, 4194304, NULL, NULL }; // nope, 300s = 5min, false, 4194304 = 2^22 = 4MB, none, none
    return DEFAULT_CONFIG;
}
