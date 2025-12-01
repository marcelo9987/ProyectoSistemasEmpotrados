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
#include <WiFi.h>

#define MAX_DISPLAY 15
#define MAX_RED_SSID 32
#define MAX_REDES 50

struct Red {
    char ssid[MAX_RED_SSID];
    int rssi;
};

extern struct Red redesDetectadas[MAX_REDES];
extern int numRedesDetectadas;

void scanAndDisplay();
void inicializarModoEscaner();
