#include "config.h"

config default_config(){
    config DEFAULT_CONFIG = { false, 300, false, 4194304, NULL, NULL }; // invalid, 300s = 5min, non recursive, 4194304 = 2^22 = 4MB, brak katalogu, brak katalogu
    return DEFAULT_CONFIG;
}
