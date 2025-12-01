/*
  Copyright (C) 2025. Marcelo Fort Muñoz y Víctor Arrollo Marquez
  GPLv3
*/

#include <WiFi.h>
#include <stdio.h>
#include <stdlib.h>
#include "red_cliente.h"

// -------------------------
// Pines
// -------------------------
#define pinNoConectado 36
#define pinConectado   37

// -------------------------
// WiFi
// -------------------------
const char* ssid     = "MiFibra-D160";
const char* password = "S5hSqhYQ";

IPAddress staticIP(192, 168, 1, 248);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// -------------------------
// Cliente TCP
// -------------------------
#define K_MAX_TAMANHO_LINEA 1000
#define K_TIEMPO_ESPERA_ENTRE_ENVIOS 2000   // milisegundos

int socketConexion = -1;

// -------------------------

void cambiarSituacion(bool conectado)
{
    digitalWrite(pinConectado,  conectado ? HIGH : LOW);
    digitalWrite(pinNoConectado, conectado ? LOW  : HIGH);
}

// -------------------------

void conectarWifi()
{
    Serial.println("Configurando WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.config(staticIP, gateway, subnet);
    WiFi.begin(ssid, password);
    cambiarSituacion(false);

    Serial.print("Conectando a ");
    Serial.println(ssid);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi conectada!");
    Serial.print("IP obtida: ");
    Serial.println(WiFi.localIP());

    cambiarSituacion(true);
}

// -------------------------

void conectarServidor(const char* ip, uint16_t porto)
{
    Serial.printf("Conectando ao servidor %s:%d...\n", ip, porto);

    socketConexion = conectar_servidor(ip, porto);

    if (socketConexion < 0)
    {
        Serial.println("ERRO: Non foi posible conectar ao servidor.");
        cambiarSituacion(false);
        while (true) { delay(1000); }  // Bloquear
    }

    Serial.println("Conectado ao servidor!");
    cambiarSituacion(true);
}

// -------------------------

void enviarDatosPeriodicamente()
{
    for (int i = 0; i < 5; i++)
    {
        char buffer[K_MAX_TAMANHO_LINEA];

        int len = snprintf(buffer, K_MAX_TAMANHO_LINEA,
                           "Paquete de datos #%d enviado dende o ESP32.\n", i + 1);

        if (enviar_datos(socketConexion, buffer, len) < 0)
        {
            Serial.println("ERRO enviando datos. Pechando conexión.");
            desconectar_servidor(socketConexion);
            cambiarSituacion(false);
            return;
        }

        Serial.printf("Enviado: %s", buffer);
        delay(K_TIEMPO_ESPERA_ENTRE_ENVIOS);
    }

    Serial.println("Rematados os envíos. Pechando conexión.");
    desconectar_servidor(socketConexion);
    cambiarSituacion(false);
}

// -------------------------

void setup()
{
    Serial.begin(115200);

    pinMode(pinNoConectado, OUTPUT);
    pinMode(pinConectado,   OUTPUT);

    cambiarSituacion(false);

    conectarWifi();

    conectarServidor("192.168.1.19", 5000);

    enviarDatosPeriodicamente();
}

void loop()
{

}

