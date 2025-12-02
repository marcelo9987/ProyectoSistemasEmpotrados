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

#include <WiFi.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "cliente_tcp.h"
#include "wifi_scanner.h"
#include "leds.h"
#include "constantes.h"



// --- SPI ---
#define SPI_MOSI 11
#define SPI_MISO 13
#define SPI_SCK 12

// TFT
#define TFT_CS 10
#define TFT_DC 8
#define TFT_RST 4
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

// --- WiFi ---
const char* ssid = "MiFibra-D160";
const char* password = "S5hSqhYQ";
IPAddress staticIP(192, 168, 1, 248);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// --- Modo de operación ---
typedef enum
{
    MODO_CLIENTE_TCP, MODO_ESCANER_WIFI
}ModoOperacion;


ModoOperacion modoActual = MODO_ESCANER_WIFI;



void conectarWifi()
{
    Serial.println("Configurando WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.config(staticIP, gateway, subnet);
    WiFi.begin(ssid, password);
    cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);

    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.print("Conectando...");

    while (WiFi.status() != WL_CONNECTED){}

    tft.fillScreen(ILI9341_BLACK);
    Serial.println("\nWiFi conectada!");
    Serial.print("IP obtida: ");
    Serial.println(WiFi.localIP());
    cambiarSituacionLeds(CONEXION_ESTABLECIDA);
}

void _inicializar_pines()
{
    pinMode(PIN_NO_CONECTADO, OUTPUT);
    pinMode(PIN_CONECTADO, OUTPUT);
    pinMode(PIN_ESTADO_TRANSMISION, OUTPUT);
}

void setup()
{
    Serial.begin(115200);

    _inicializar_pines();

    cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);

    conectarWifi();
    inicializarModoEscaner();
}

void loop()
{
    static unsigned long ultimoCambioModo = 0;
    const long INTERVALO_CAMBIO_MODO = 30000;

    static unsigned long ultimoEscaneo = 0;
    const long INTERVALO_ESCANEO = 5000;

    // Cambiar de modo cada 30s
    if (millis() - ultimoCambioModo >= INTERVALO_CAMBIO_MODO)
    {
        ultimoCambioModo = millis();
        if (modoActual == MODO_CLIENTE_TCP)
        {
            if (socketConexion != -1)
            {
                desconectar_servidor(socketConexion);
                socketConexion = -1;
                cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);
            }
            modoActual = MODO_ESCANER_WIFI;
            inicializarModoEscaner();
            Serial.println("\n*** CAMBIO DE MODO: Escáner WiFi ***");
        }
        else
        {
            modoActual = MODO_CLIENTE_TCP;
            inicializarModoClienteTCP();
            Serial.println("\n*** CAMBIO DE MODO: Cliente TCP ***");
        }
    }

    if (modoActual == MODO_ESCANER_WIFI)
    {
        if (millis() - ultimoEscaneo >= INTERVALO_ESCANEO)
        {
            ultimoEscaneo = millis();
            scanAndDisplay();
        }
    }
    else if (modoActual == MODO_CLIENTE_TCP)
    {
        if (millis() - ultimoEnvio >= K_TIEMPO_ESPERA_ENTRE_ENVIOS)
        {
            enviarRedesAlServidor();
            ultimoEnvio = millis();
        }
    }
}
