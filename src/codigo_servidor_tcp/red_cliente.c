/*
      Copyright (C) 2025 Marcelo Fort Muñoz     <mfortm@alumnos.nebrija.es>
      Copyright (C) 2025Víctor Arrollo Marquez  <varroyom@alumnos.nebrija.es>

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <https://www.gnu.org/licenses/>.

 */

#include "red_cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int conectar_servidor(const char *ip, uint16_t puerto) {
    int socket_fd;
    struct sockaddr_in serv_addr;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error en la función socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(puerto);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        fprintf(stderr, "Error: La dirección ip (%s) no es válida\n", ip);
        close(socket_fd);
        return -1;
    }

    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error en la función connect");
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

int enviar_datos(int socket_fd, const char *datos, size_t len) {
    if (send(socket_fd, datos, len, 0) < 0) {
        perror("Error en la función send");
        return -1;
    }
    return 0;
}

void desconectar_servidor(int socket_fd) {
    if (close(socket_fd) == -1) {
        perror("Error en la función close");
    }
}

