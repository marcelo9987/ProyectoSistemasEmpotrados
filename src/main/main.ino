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
#include <driver/rtc_io.h>
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
    MODO_CLIENTE_TCP, MODO_ESCANER_WIFI, MODO_REPOSO
}ModoOperacion;

volatile ModoOperacion modoActual = MODO_ESCANER_WIFI;


volatile EstadoTransmision estadoTransmision = HABILITADA;
volatile bool lecturaPermitida = true;

volatile long ultimaLecturaLlave = 0;


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
    pinMode(PIN_DESPERTAR, INPUT_PULLDOWN);
    pinMode(PIN_DORMIR, INPUT_PULLDOWN);
}

void IRAM_ATTR _dormir()
{
    modoActual=MODO_REPOSO;
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

void IRAM_ATTR _timer_escaneo()
{
    lecturaPermitida = true;
}



void setup()
{
    Serial.begin(115200);

    _inicializar_pines();

    esp_sleep_enable_ext0_wakeup(PIN_DESPERTAR,1);
    rtc_gpio_pulldown_en(PIN_DESPERTAR);
    rtc_gpio_pullup_dis(PIN_DESPERTAR);

    attachInterrupt(digitalPinToInterrupt(PIN_LLAVE), _llave_girada, RISING); 
    attachInterrupt(digitalPinToInterrupt(PIN_DORMIR), _dormir, RISING); 


    cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);

    conectarWifi();
    inicializarModoEscaner();


    hw_timer_t *timer = NULL;
    timer = timerBegin(1000000); 
    timerAttachInterrupt(timer, &_timer_escaneo);
    timerAlarm(timer, K_INTERVALO_ESCANEO, true,0); 


}

void loop()
{

    if (modoActual == MODO_ESCANER_WIFI)
    {
        
        if (lecturaPermitida)
        {
            inicializarModoEscaner();
            scanAndDisplay();
            lecturaPermitida = false;
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
    if (modoActual == MODO_REPOSO)
    {
        esp_deep_sleep_start();
    }
}
