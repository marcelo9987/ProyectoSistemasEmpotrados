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


#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <WiFi.h>
#include <driver/rtc_io.h>
#include "cliente_tcp.h"
#include "constantes.h"
#include "leds.h"
#include "wifi_scanner.h"




Adafruit_ILI9341 g_tft(TFT_CS, TFT_DC, TFT_RST);

typedef enum
{
    MODO_CLIENTE_TCP, MODO_ESCANER_WIFI, MODO_REPOSO
}ModoOperacion;

typedef enum
{
    HABILITADA=0, PROCESANDO=1, BLOQUEADA=-1
} EstadoTransmision;


typedef struct
{
    volatile ModoOperacion modoActual;
    volatile EstadoTransmision estadoTransmision;
    volatile bool lecturaPermitida;
    volatile long ultimaLecturaLlave;
}Config;

volatile Config g_configuracion=
    {
    .modoActual = MODO_ESCANER_WIFI,
    .estadoTransmision = HABILITADA,
    .lecturaPermitida = true,
    .ultimaLecturaLlave = 0
};


void conectar_wifi()
{
    const char* ssid = "MiFibra-D160";
    const char* password = "S5hSqhYQ";
    IPAddress staticIp(192, 168, 1, 248);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    Serial.println("Configurando WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.config(staticIp, gateway, subnet);
    WiFi.begin(ssid, password);

    g_tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    g_tft.setCursor(0, 0);
    g_tft.setTextSize(2);
    g_tft.print("Conectando...");
    cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);
    

    while (WiFi.status() != WL_CONNECTED){}

    g_tft.fillScreen(ILI9341_BLACK);
    Serial.println("\nWiFi conectada!");
    Serial.print("IP obtenida: ");
    Serial.println(WiFi.localIP());
    cambiarSituacionLeds(CONEXION_ESTABLECIDA);
}

void inicializar_pines()
{
    pinMode(PIN_LED_NO_CONECTADO, OUTPUT);
    pinMode(PIN_LED_CONECTADO, OUTPUT);
    pinMode(PIN_LED_ESTADO_TRANSMISION, OUTPUT);
    pinMode(PIN_LLAVE, INPUT_PULLUP);
    pinMode(PIN_BOTON_DESPERTAR, INPUT_PULLDOWN);
    pinMode(PIN_BOTON_DORMIR, INPUT_PULLDOWN);
}

void IRAM_ATTR dormir()
{
    g_configuracion.modoActual=MODO_REPOSO;
}


void IRAM_ATTR llave_girada()
{
    if(g_configuracion.estadoTransmision==PROCESANDO)
    {
        Serial.println("Aún procesando...");
        return;
    }

    Serial.printf("Estado transmision: %i\n",g_configuracion.estadoTransmision);

    const long lecturaActual = millis();
    if((lecturaActual-g_configuracion.ultimaLecturaLlave)>=K_PERIODO_NO_LLAVE)
    {
        g_configuracion.ultimaLecturaLlave=lecturaActual;
        Serial.printf("Activando transmision\n");
        g_configuracion.estadoTransmision = PROCESANDO;
        g_configuracion.modoActual = MODO_CLIENTE_TCP;
        return;
    }

     Serial.printf("No activando transmision\n");

}

void IRAM_ATTR timer_escaneo()
{
    g_configuracion.lecturaPermitida = true;
}



void setup()
{
    Serial.begin(115200);

    inicializar_pines();

    esp_sleep_enable_ext0_wakeup(PIN_BOTON_DESPERTAR,1);
    rtc_gpio_pulldown_en(PIN_BOTON_DESPERTAR);
    rtc_gpio_pullup_dis(PIN_BOTON_DESPERTAR);

    attachInterrupt(digitalPinToInterrupt(PIN_LLAVE), llave_girada, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_BOTON_DORMIR), dormir, RISING);


    cambiarSituacionLeds(CONEXION_NO_ESTABLECIDA);

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    g_tft.begin();
    g_tft.setRotation(1);
    g_tft.fillScreen(ILI9341_BLACK);

    conectar_wifi();
    inicializarModoEscaner();


    hw_timer_t *timer = NULL;
    timer = timerBegin(1000000); 
    timerAttachInterrupt(timer, &timer_escaneo);
    timerAlarm(timer, K_INTERVALO_ESCANEO, true,0); 


}

bool primera_iter_bucle_scan = true;

void loop()
{

    if (g_configuracion.modoActual == MODO_ESCANER_WIFI)
    {
        
        if (g_configuracion.lecturaPermitida || primera_iter_bucle_scan==true)
        {
            inicializarModoEscaner();
            escanearYmostrar();
            g_configuracion.lecturaPermitida = false;
            primera_iter_bucle_scan = false;
        }
    }
    if (g_configuracion.modoActual == MODO_CLIENTE_TCP)
    {
        cambiarSituacionLeds(TRANSMISION_EN_CURSO);
        inicializarModoClienteTCP();
        Serial.println("\n*** CAMBIO DE MODO: Cliente TCP ***");
        
        enviarRedesAlServidor();

        if (socketConexion != -1)
        {
            desconectar_servidor(socketConexion);
            socketConexion = -1;
            cambiarSituacionLeds(CONEXION_ESTABLECIDA);
        }

        g_configuracion.estadoTransmision = HABILITADA;
                    
        g_configuracion.modoActual = MODO_ESCANER_WIFI;
        primera_iter_bucle_scan=true;
    }
    if (g_configuracion.modoActual == MODO_REPOSO)
    {
        esp_deep_sleep_start();
    }
}
