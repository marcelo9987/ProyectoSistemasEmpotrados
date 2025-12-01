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

#include "wifi_scanner.h"
#include <Adafruit_ILI9341.h>
extern Adafruit_ILI9341 tft;

Red redesDetectadas[MAX_REDES];
int numRedesDetectadas = 0;

void scanAndDisplay() {
    tft.fillRect(0, 40, 320, 200, ILI9341_BLACK);
    tft.setCursor(0, 40);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);

    Serial.println("Scanning WiFi...");
    tft.println("Scanning...");

    numRedesDetectadas = WiFi.scanNetworks();
    int n = numRedesDetectadas;
    Serial.printf("Found %d networks\n", n);

    tft.fillRect(0, 40, 320, 200, ILI9341_BLACK);
    tft.setCursor(0, 40);

    if (n <= 0) {
        tft.println("No networks found");
        return;
    }

    int show = n;
    if (show > MAX_DISPLAY) show = MAX_DISPLAY;

    tft.setTextSize(1);
    for (int i = 0; i < show; i++) {
        String ssid = WiFi.SSID(i);
        int rssi = WiFi.RSSI(i);
        if (ssid.length() > MAX_RED_SSID - 1) ssid = ssid.substring(0, MAX_RED_SSID - 1);

        strncpy(redesDetectadas[i].ssid, ssid.c_str(), MAX_RED_SSID);
        redesDetectadas[i].rssi = rssi;

        Serial.printf("%d: %s (%d dBm)\n", i, redesDetectadas[i].ssid, rssi);

        tft.print(i);
        tft.print(": ");
        tft.print(ssid);
        tft.print(" ");
        tft.print(rssi);
        tft.println("dB");
    }

    tft.setTextSize(1);
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setCursor(0, 220);
    tft.print("Total networks: ");
    tft.print(n);
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
}

void inicializarModoEscaner() {
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println("WiFi Scanner");
    tft.setTextSize(1);
    tft.println("Showing top networks...");
    tft.println("");
}
