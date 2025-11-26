#include <WiFi.h>

#define PIN_ROJO 35
#define PIN_AZUL 36
#define PIN_AMARILLO 37
#define PIN_LLAVE 38

const long int TIEMPO_DEBOUNCE = 200; 


const char* ssid = "MiFibra-D160";
const char* password = "S5hSqhYQ";

IPAddress staticIP(192, 168, 64, 248);     
IPAddress gateway(192, 168, 64, 1);       
IPAddress subnet(255, 255, 255, 0);       

volatile bool interrupcionRecibida = false;
volatile bool estadoLlaveActivado = false;
unsigned long tiempoUltimaPulsacion = 0; 

void cambiarSituacion(bool conectado)
{
    if(conectado)
    {
        digitalWrite(PIN_AZUL,HIGH);
        digitalWrite(PIN_ROJO,LOW);
        return;
    }

    digitalWrite(PIN_AZUL,LOW);
    digitalWrite(PIN_ROJO,HIGH);
}

// Función de interrupción (ISR): 
void IRAM_ATTR interrupcionLlave()
{
    interrupcionRecibida = true;
}

void setup()
{
    // ... Configuración de Pins ...
    pinMode(PIN_ROJO,OUTPUT);
    pinMode(PIN_AZUL,OUTPUT);
    pinMode(PIN_AMARILLO,OUTPUT); 
    pinMode(PIN_LLAVE, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PIN_LLAVE),interrupcionLlave,FALLING);

    Serial.begin(115200);
    delay(1000);

    WiFi.config(staticIP, gateway, subnet);
    WiFi.mode(WIFI_STA); 
    Serial.print("Intentando conectar a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    Serial.println("\nConectando");
    cambiarSituacion(false);

    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500); 
    }

    cambiarSituacion(true);

    Serial.println("\nConectado a la red WiFi");
    Serial.print("IP ESP32 Local: ");
    Serial.println(WiFi.localIP()); 
}

void loop()
{
    if(interrupcionRecibida)
    {
        if ((millis() - tiempoUltimaPulsacion) > (long)TIEMPO_DEBOUNCE)
        {

            estadoLlaveActivado = !estadoLlaveActivado;

            Serial.printf("Diferencia: %d\n",(millis() - tiempoUltimaPulsacion));

            // 1. Debouncing:
            noInterrupts();
            interrupcionRecibida = false;
            interrupts();
            tiempoUltimaPulsacion = millis(); 
           
            
            // 2. Acción principal
            Serial.print("LLave accionada (Debounced). Novo estado: ");
            Serial.println(estadoLlaveActivado);
            
            digitalWrite(PIN_AMARILLO, estadoLlaveActivado);
        } 
        else 
        {
            
            noInterrupts();
            interrupcionRecibida = false;
            interrupts();
        }
    }
}