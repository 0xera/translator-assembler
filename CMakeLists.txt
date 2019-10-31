# cmake_minimum_required(VERSION <specify CMake version here>)
project(untitled C)

set(CMAKE_C_STANDARD 99)


add_executable(untitled pars.c hash.c hash.h pars.h translator.c translator.h)