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




Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

const char* ssid = "MiFibra-D160";
const char* password = "S5hSqhYQ";
IPAddress staticIP(192, 168, 1, 248);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

typedef enum
{
    MODO_CLIENTE_TCP, MODO_ESCANER_WIFI
}ModoOperacion;

volatile ModoOperacion modoActual = MODO_ESCANER_WIFI;


volatile int ultimaLecturaLlave = 0; 
volatile EstadoTransmision estadoTransmision = HABILITADA;


void conectarWifi()
{
    Serial.println("Configurando WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.config(staticIP, gateway, subnet);
    WiFi.begin(ssid, password);

    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.print("Conectando...");
    cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);
    

    while (WiFi.status() != WL_CONNECTED){}

    tft.fillScreen(ILI9341_BLACK);
    Serial.println("\nWiFi conectada!");
    Serial.print("IP obtenida: ");
    Serial.println(WiFi.localIP());
    cambiarSituacionLeds(CONEXION_ESTABLECIDA);
}

void _inicializar_pines()
{
    pinMode(PIN_NO_CONECTADO, OUTPUT);
    pinMode(PIN_CONECTADO, OUTPUT);
    pinMode(PIN_ESTADO_TRANSMISION, OUTPUT);
    pinMode(PIN_LLAVE, INPUT_PULLUP);
}

void IRAM_ATTR _llave_girada()
{
    if(estadoTransmision==PROCESANDO)
    {
        return;
    }

    Serial.printf("Estado transmision: %i\n",estadoTransmision);
    
    const int lecturaActual = millis();
    if((lecturaActual-ultimaLecturaLlave)>=K_PERIODO_NO_LLAVE)
    {
        ultimaLecturaLlave=lecturaActual;
        Serial.printf("Activando transmision\n");
        estadoTransmision = PROCESANDO;
        modoActual = MODO_CLIENTE_TCP;
        return;
    }

     Serial.printf("No activando transmision\n");

}
unsigned long ultimoEscaneo = 0;

void setup()
{
    Serial.begin(115200);

    _inicializar_pines();
    
    attachInterrupt(digitalPinToInterrupt(PIN_LLAVE), _llave_girada, RISING); // Configure the interrupt

    cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);

    conectarWifi();
    inicializarModoEscaner();

    ultimoEscaneo = millis();
}

void loop()
{

    if (modoActual == MODO_ESCANER_WIFI)
    {
        
        if ((millis() - ultimoEscaneo) >= K_INTERVALO_ESCANEO)
        {
            inicializarModoEscaner();
            ultimoEscaneo = millis();
            scanAndDisplay();
        }
    }
    if (modoActual == MODO_CLIENTE_TCP)
    {
        inicializarModoClienteTCP();
        Serial.println("\n*** CAMBIO DE MODO: Cliente TCP ***");
        
        enviarRedesAlServidor();

        if (socketConexion != -1)
        {
            desconectar_servidor(socketConexion);
            socketConexion = -1;
            cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);
        }

        estadoTransmision = HABILITADA;
                    
        modoActual = MODO_ESCANER_WIFI;
    }
}
