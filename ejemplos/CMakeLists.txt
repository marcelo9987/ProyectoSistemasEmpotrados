cmake_minimum_required(VERSION 3.10)
project(ServidorCliente_empotrados C)

#set(CMAKE_C_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# Añadimos la biblioteca para la lógica de red del cliente
add_library(red_cliente red_cliente.c)

# Ejecutable del servidor
add_executable(Practica2_P2_s servidormayus.c)

# Ejecutable del cliente de prueba
add_executable(Practica2_P2_c clientemayus.c)

# Enlazamos el cliente con la biblioteca de red
target_link_libraries(Practica2_P2_c red_cliente)


