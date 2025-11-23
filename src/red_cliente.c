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

