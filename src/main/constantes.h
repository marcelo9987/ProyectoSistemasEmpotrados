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

typedef enum
{
    HABILITADA=0, PROCESANDO=1, BLOQUEADA=-1
} EstadoTransmision;

#define K_PERIODO_NO_LLAVE 400
#define K_INTERVALO_ESCANEO 10000

// --- Pines ---
#define PIN_NO_CONECTADO 36
#define PIN_CONECTADO 37
#define PIN_ESTADO_TRANSMISION 38
#define PIN_LLAVE 21
#define PIN_BOTON 46 

// --- SPI ---
#define SPI_MOSI 11
#define SPI_MISO 13
#define SPI_SCK 12

// TFT
#define TFT_CS 10
#define TFT_DC 8
#define TFT_RST 4

// --- Conectividad ----
#define DIRECCION_IP_SERVIDOR "192.168.1.19"
