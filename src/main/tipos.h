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

// En este archivo ponemos los tipos de datos usados por varios .c
#pragma once
#include "constantes.h"

typedef struct  {
    char ssid[K_MAX_RED_SSID];
    int rssi;
}Red;

typedef enum
{
    CONEXION_ESTABLECIDA, CONEXION_NO_ESTABLECIDA, TRANSMISION_EN_CURSO
}EstadoLeds;
