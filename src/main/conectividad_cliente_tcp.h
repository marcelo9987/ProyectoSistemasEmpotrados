/*
      Copyright (C) 2025 Marcelo Fort Muñoz      <mfortm@alumnos.nebrija.es>
      Copyright (C) 2025 Víctor Arrollo Marquez  <varroyom@alumnos.nebrija.es>

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
 
#pragma once



#include <stdint.h>
#include <stddef.h>

/**
 * @brief Crea un socket y se conecta a un servidor.
 *
 * @param ip La dirección IP del servidor.
 * @param puerto El puerto del servidor.
 * @return El descriptor del socket de conexión en caso de éxito, -1 en caso de error.
 */
int conectar_servidor(const char *ip, uint16_t puerto);

/**
 * @brief Envía un bloque de datos a través de un socket.
 *
 * @param socket_fd El descriptor del socket de conexión.
 * @param datos Los datos a enviar.
 * @param len La longitud de los datos a enviar.
 * @return 0 en caso de éxito, -1 en caso de error.
 */
int enviar_datos(int socket_fd, const char *datos, size_t len);

/**
 * @brief Cierra la conexión del socket.
 *
 * @param socket_fd El descriptor del socket a cerrar.
 */
void desconectar_servidor(int socket_fd);
