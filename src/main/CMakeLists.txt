cmake_minimum_required(VERSION 4.0)
project(main)#e

set(CMAKE_CXX_STANDARD 20)


include_directories(.)

add_executable(main
        main.ino
        cliente_tcp.cpp
        cliente_tcp.h
        conectividad_cliente_tcp.cpp
        conectividad_cliente_tcp.h
        wifi_scanner.cpp
        wifi_scanner.h
        tipos.h)
