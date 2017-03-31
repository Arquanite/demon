cmake_minimum_required(VERSION 2.8)

project(demon666)
include_directories("headers")
add_executable(${PROJECT_NAME} "sources/main.c"
                               "sources/parse.c"
                               "headers/config.h"
                               "sources/config.c"
                               "headers/bool.h")
