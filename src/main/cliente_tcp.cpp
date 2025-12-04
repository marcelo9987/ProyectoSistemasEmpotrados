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

#include "cliente_tcp.h"
#include "leds.h"
#include "constantes.h"
#include <Adafruit_ILI9341.h>


#include "wifi_scanner.h"
extern Adafruit_ILI9341 g_tft;

int socketConexion = -1;
unsigned long g_ultimoEnvio = 0;
int g_contadorEnvios = 0;

void conectarServidor(const char* ip, uint16_t puerto) {
    Serial.printf("Conectándose al servidor %s:%d...\n", ip, puerto);

    socketConexion = conectar_servidor(ip, puerto);

    if (socketConexion < 0) {
        Serial.println("ERRO: No fue posible conectarse al servidor.");
        cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);
        g_tft.fillScreen(ILI9341_RED);
        g_tft.setCursor(0, 0);
        g_tft.setTextColor(ILI9341_WHITE, ILI9341_RED);
        g_tft.setTextSize(2);
        g_tft.println("ERROR TCP");
        g_tft.setTextSize(1);
        g_tft.println("No conecta");
    } else {
        Serial.println("Conectado al servidor!");
        cambiarSituacionLeds(CONEXION_ESTABLECIDA);
    }
}

void enviarRedesAlServidor() {

	noInterrupts();
    if (socketConexion < 0) {
        Serial.println("Error: No hay conexión activa.");
        conectarServidor("192.168.1.19", 1234);
        if (socketConexion < 0)
		{
			return;
		}
    }

    char buffer[K_MAX_TAMANHO_LINEA];
    int len = snprintf(buffer, K_MAX_TAMANHO_LINEA, "Redes WiFi encontradas:\n");

    for (int i = 0; i < g_numRedesDetectadas && len < K_MAX_TAMANHO_LINEA - 50; i++) {
        if (strlen(g_redesDetectadas[i].ssid) == 0) continue; // ignorar SSID vacíos
        len += snprintf(buffer + len, K_MAX_TAMANHO_LINEA - len,
                        "%d: %s (%d dBm)\n", i, g_redesDetectadas[i].ssid, g_redesDetectadas[i].rssi);
    }

    if (enviar_datos(socketConexion, buffer, len) < 0) {
        Serial.println("ERROR enviando datos. Cerrando conexión.");
        desconectar_servidor(socketConexion);
        socketConexion = -1;
        cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);
        return;
    }

    interrupts();

    Serial.printf("Enviado:\n%s", buffer);
    g_contadorEnvios++;
}

void inicializarModoClienteTCP() {
    g_tft.fillScreen(ILI9341_BLACK);
    g_tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    g_tft.setTextSize(2);
    g_tft.setCursor(0, 0);
    g_tft.println("MODO CLIENTE TCP");
    g_tft.setTextSize(1);
    g_tft.println("Conectándose al servidor...");

    conectarServidor(DIRECCION_IP_SERVIDOR, 1234);
    g_contadorEnvios = 0;
    g_ultimoEnvio = millis();
}
