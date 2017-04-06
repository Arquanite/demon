cmake_minimum_required(VERSION 2.8)

project(demon666)
include_directories("headers")
add_executable(${PROJECT_NAME} "sources/main.c"
                               "headers/parse.h" "sources/parse.c"
                               "headers/config.h" "sources/config.c"
                               "headers/file.h" "sources/file.c"
                               "headers/filelist.h" "sources/filelist.c"
                               "headers/sync.h" "sources/sync.c"
                               "headers/timestamp.h" "sources/timestamp.c"
                               "headers/bool.h"
                               "headers/filetype.h")
