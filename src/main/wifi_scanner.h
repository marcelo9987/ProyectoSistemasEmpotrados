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

#include <Arduino.h>
#include <WiFi.h>

#include "tipos.h"


extern Red g_redesDetectadas[K_MAX_REDES];
extern int g_numRedesDetectadas;

/**
 * @brief Escanea las redes WiFi disponibles y las muestra en la pantalla TFT.
 */
void escanearYmostrar();

/**
 * @brief Dibuja la pantalla inicial del modo escáner de WiFi en la TFT.
 */
void inicializarModoEscaner();
