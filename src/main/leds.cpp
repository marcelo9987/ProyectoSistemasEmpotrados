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

#include "leds.h"
#include <Arduino.h>
#include "constantes.h"

void cambiarSituacionLeds(const EstadoLeds conectado)
{
    switch (conectado)
    {
    case TRANSMISION_EN_CURSO:
        digitalWrite(PIN_LED_ESTADO_TRANSMISION,HIGH);
        break;
    case CONEXION_ESTABLECIDA:
        digitalWrite(PIN_LED_CONECTADO, HIGH);
        digitalWrite(PIN_LED_NO_CONECTADO, LOW);
        digitalWrite(PIN_LED_ESTADO_TRANSMISION, LOW);
        break;
    case CONEXION_NO_ESTABLECIDA:
        digitalWrite(PIN_LED_CONECTADO, LOW);
        digitalWrite(PIN_LED_NO_CONECTADO, HIGH);
        digitalWrite(PIN_LED_ESTADO_TRANSMISION, LOW);
        break;
    default:
        break;
    }
}
