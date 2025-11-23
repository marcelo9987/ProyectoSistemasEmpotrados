#include <WiFi.h>

const char* ssid = "Kiwis e Caquis";
const char* password = "35259600";

// Define a IP fixa, a máscara de subrede e a porta de enlace
IPAddress staticIP(192, 168, 64, 248);     // IP fixa (192.168.64.248)
IPAddress gateway(192, 168, 64, 1);       // Porta de enlace (Exemplo: 192.168.64.1)
IPAddress subnet(255, 255, 255, 0);       // Máscara de subrede (Moi común: 255.255.255.0)
// IPAddress primaryDNS(8, 8, 8, 8);      // DNS Primario (Opcional)
// IPAddress secondaryDNS(8, 8, 4, 4);    // DNS Secundario (Opcional)

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // 1. Configurar a IP estática antes de conectarse
    WiFi.config(staticIP, gateway, subnet);
    // Configurar tamén os DNS:
    // WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS);

    WiFi.mode(WIFI_STA); // Modo Estación (Cliente)
    Serial.print("Intentando conectar a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    Serial.println("\nConectando");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500); 
    }

    Serial.println("\nConectado á rede WiFi");
    Serial.print("IP ESP32 Local: ");
    Serial.println(WiFi.localIP()); 
}

void loop(){}
