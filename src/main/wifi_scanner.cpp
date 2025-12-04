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

#include "wifi_scanner.h"

#include <Adafruit_ILI9341.h>
extern Adafruit_ILI9341 g_tft;

Red g_redesDetectadas[K_MAX_REDES];
int g_numRedesDetectadas = 0;


void escanearYmostrar() {
    g_tft.fillRect(0, 40, 320, 200, ILI9341_BLACK);
    g_tft.setCursor(0, 40);
    g_tft.setTextSize(2);
    g_tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);

    Serial.println("Escaneando las wifis...");
    g_tft.println("Escaneando...");

    g_numRedesDetectadas = WiFi.scanNetworks();
    int numero_redes_detectadas = g_numRedesDetectadas;
    Serial.printf("Se han encontrado %d redes\n", numero_redes_detectadas);

    g_tft.fillRect(0, 40, 320, 200, ILI9341_BLACK);
    g_tft.setCursor(0, 40);

    if (numero_redes_detectadas <= 0) {
        g_tft.println("No se han encontrado redes.");
        return;
    }

    int show = numero_redes_detectadas;
    if (show > K_MAX_DISPLAY) show = K_MAX_DISPLAY;

    g_tft.setTextSize(1);
    for (int i = 0; i < show; i++) {
        String ssid = WiFi.SSID(i);
        int rssi = WiFi.RSSI(i);
        if (ssid.length() > K_MAX_RED_SSID - 1) ssid = ssid.substring(0, K_MAX_RED_SSID - 1);

        strncpy(g_redesDetectadas[i].ssid, ssid.c_str(), K_MAX_RED_SSID);
        g_redesDetectadas[i].rssi = rssi;

//        Serial.printf("%d: %s (%d dBm)\n", i, g_redesDetectadas[i].ssid, rssi);

        g_tft.print(i);
        g_tft.print(": ");
        g_tft.print(ssid);
        g_tft.print(" ");
        g_tft.print(rssi);
        g_tft.println("dB");
    }

    g_tft.setTextSize(1);
    g_tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    g_tft.setCursor(0, 220);
    // g_tft.print("Numero total de redes: ");
    // g_tft.print(numero_redes_detectadas);
    g_tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
}

void inicializarModoEscaner() {
    g_tft.fillScreen(ILI9341_BLACK);
    g_tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    g_tft.setTextSize(2);
    g_tft.setCursor(0, 0);
    g_tft.println("Escaner de redes WiFi");
    g_tft.setTextSize(1);
    g_tft.println("Mostrando las primeras redes...");
    g_tft.println("");
}
