cmake_minimum_required(VERSION 4.0)
project(ProyectoSistemasEmpotrados)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(ProyectoSistemasEmpotrados
        src/main.ino
        src/red_cliente.c
        src/red_cliente.h)
