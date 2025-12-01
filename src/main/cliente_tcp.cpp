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

#include "cliente_tcp.h"
#include "leds.h"
#include <Adafruit_ILI9341.h>

#include "wifi_scanner.h"
extern Adafruit_ILI9341 tft;

int socketConexion = -1;
unsigned long ultimoEnvio = 0;
int contadorEnvios = 0;

void conectarServidor(const char* ip, uint16_t porto) {
    Serial.printf("Conectando ao servidor %s:%d...\n", ip, porto);

    socketConexion = conectar_servidor(ip, porto);

    if (socketConexion < 0) {
        Serial.println("ERRO: Non foi posible conectar ao servidor.");
        cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);
        tft.fillScreen(ILI9341_RED);
        tft.setCursor(0, 0);
        tft.setTextColor(ILI9341_WHITE, ILI9341_RED);
        tft.setTextSize(2);
        tft.println("ERRO TCP");
        tft.setTextSize(1);
        tft.println("Non conecta");
    } else {
        Serial.println("Conectado ao servidor!");
        cambiarSituacionLeds(CONEXION_ESTABLECIDA);
    }
}

void enviarRedesAlServidor() {
    if (socketConexion < 0) {
        Serial.println("Erro: Non hai conexión activa.");
        conectarServidor("192.168.1.19", 1234);
        if (socketConexion < 0) return;
    }

    char buffer[K_MAX_TAMANHO_LINEA];
    int len = snprintf(buffer, K_MAX_TAMANHO_LINEA, "Redes WiFi encontradas:\n");

    for (int i = 0; i < numRedesDetectadas && len < K_MAX_TAMANHO_LINEA - 50; i++) {
        if (strlen(redesDetectadas[i].ssid) == 0) continue; // ignorar SSID vacíos
        len += snprintf(buffer + len, K_MAX_TAMANHO_LINEA - len,
                        "%d: %s (%d dBm)\n", i, redesDetectadas[i].ssid, redesDetectadas[i].rssi);
    }

    if (enviar_datos(socketConexion, buffer, len) < 0) {
        Serial.println("ERRO enviando datos. Pechando conexión.");
        desconectar_servidor(socketConexion);
        socketConexion = -1;
        cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);
        return;
    }

    Serial.printf("Enviado:\n%s", buffer);
    contadorEnvios++;
}

void inicializarModoClienteTCP() {
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println("MODO CLIENTE TCP");
    tft.setTextSize(1);
    tft.println("Conectando ao servidor...");

    conectarServidor("192.168.1.19", 1234);
    contadorEnvios = 0;
    ultimoEnvio = millis();
}
