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

// --- Tiempos ---
#define K_PERIODO_NO_LLAVE 1000
#define K_INTERVALO_ESCANEO 15000000

// --- Conectividad ----
#define DIRECCION_IP_SERVIDOR "192.168.1.19"
#define K_MAX_REDES 50
#define K_MAX_TAMANHO_LINEA 1000
#define K_NUM_ENVIOS 5
#define K_TIEMPO_ESPERA_ENTRE_ENVIOS 2000

// --- Pines ---
#define PIN_LED_NO_CONECTADO 36
#define PIN_LED_CONECTADO 37
#define PIN_LED_ESTADO_TRANSMISION 38

#define PIN_LLAVE 21
// #define PIN_BOTON 46
#define PIN_BOTON_DESPERTAR GPIO_NUM_2
#define PIN_BOTON_DORMIR 7

// --- SPI ---
#define SPI_MOSI 11
#define SPI_MISO 13
#define SPI_SCK 12

// TFT (Pines de control)
#define TFT_CS 10
#define TFT_DC 8
#define TFT_RST 4
// TFT (misc)
#define K_MAX_DISPLAY 15
#define K_MAX_RED_SSID 32




