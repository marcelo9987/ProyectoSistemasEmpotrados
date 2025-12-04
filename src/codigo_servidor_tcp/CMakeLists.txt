cmake_minimum_required(VERSION 4.0)
project(codigo_servidor_tcp C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(codigo_servidor_tcp
        red_cliente.c
        red_cliente.h
        servidorTCP.c)
