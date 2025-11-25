/*
  Copyright (C) 2025. Marcelo Fort Muñoz y Víctor Arrollo Marquez

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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "red_cliente.h"

#define K_MAX_TAMANHO_LINEA 1000

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Es necesario introducir la dirección IP y el número de puerto por línea de comandos.\n");
        printf("Uso: .\\%s IP PUERTO\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // --

    long _numPuertoServidor = strtol(argv[2], NULL, 10);
    if (_numPuertoServidor <= 0 || _numPuertoServidor > 65535)
    {
        printf("Error: El puerto debe ser un número entre 1 y 65535\n");
        exit(EXIT_FAILURE);
    }

    u_int16_t numPuertoServidor = (u_int16_t) _numPuertoServidor;
    char *dir_ip = argv[1];

    // --

    int socketConexion = conectar_servidor(dir_ip, numPuertoServidor);
    if (socketConexion < 0)
    {
        fprintf(stderr, "Cliente.- No se pudo conectar al servidor.\n");
        exit(EXIT_FAILURE);
    }

    printf("Cliente.- Conectado al servidor %s:%d.\n", dir_ip, numPuertoServidor);
    printf("Cliente.- Enviando datos de ejemplo...\n");

    // --

    // Bucle para enviar datos de ejemplo periódicamente
    for (int i = 0; i < 5; i++) {
        char buffer[K_MAX_TAMANHO_LINEA];
        // Creamos un mensaje de ejemplo. En un ESP32, aquí leerías un sensor.
        int len = snprintf(buffer, K_MAX_TAMANHO_LINEA, "Paquete de datos #%d desde el cliente simulado.\n", i + 1);

        // Mandamos los datos al servidor usando la biblioteca
        if (enviar_datos(socketConexion, buffer, len) < 0) {
            desconectar_servidor(socketConexion);
            exit(EXIT_FAILURE);
        }

        printf("Enviado: <%s>", buffer);
        sleep(2); // Esperamos 2 segundos entre envíos
    }

    // --

    // Cerramos el socket para indicar al servidor que hemos terminado
    desconectar_servidor(socketConexion);

    // --

    printf("\nCliente.- Finalizo el programa.\n\n");

    return (EXIT_SUCCESS);
}
