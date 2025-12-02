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

#include "constantes.h"
#include "leds.h"
#include <Arduino.h>

void cambiarSituacionLeds(EstadoLeds conectado)
{
    switch (conectado)
    {
    case TRANSMISION_EN_CURSO:
        digitalWrite(PIN_ESTADO_TRANSMISION,HIGH);
        break;
    case CONEXION_ESTABLECIDA:
        digitalWrite(PIN_CONECTADO, HIGH);
        digitalWrite(PIN_NO_CONECTADO, LOW);
        digitalWrite(PIN_ESTADO_TRANSMISION, LOW);
        break;
    case CONEXION_NO_ESTABLECIDA:
        digitalWrite(PIN_CONECTADO, LOW);
        digitalWrite(PIN_NO_CONECTADO, HIGH);
        digitalWrite(PIN_ESTADO_TRANSMISION, LOW);
        break;
    default:
        break;
    }
}
