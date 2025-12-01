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

#pragma once

#include <Arduino.h>
#include "conectividad_cliente_tcp.h"

#define K_MAX_TAMANHO_LINEA 1000
#define K_NUM_ENVIOS 5
#define K_TIEMPO_ESPERA_ENTRE_ENVIOS 2000

extern int socketConexion;
extern unsigned long ultimoEnvio;
extern int contadorEnvios;

void conectarServidor(const char* ip, uint16_t porto);
void enviarRedesAlServidor();
void inicializarModoClienteTCP();


