#include <WiFi.h>
#include <Ticker.h> // 1. Incluír a biblioteca Ticker

#define PIN_ROJO 35
#define PIN_AZUL 36
#define PIN_AMARILLO 37
#define PIN_LLAVE 38

const long int TIEMPO_DEBOUNCE = 200; // Tempo de debounce en ms

const char* ssid = "MiFibra-D160";
const char* password = "S5hSqhYQ";

IPAddress staticIP(192, 168, 64, 248);     
IPAddress gateway(192, 168, 64, 1);       
IPAddress subnet(255, 255, 255, 0);       

// Ticker para o debounce
Ticker debounceTimer; 

volatile bool estadoLlaveActivado = false;
volatile bool debounceActivo = false; // Variable para indicar se estamos en período de debounce

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

// 2. Función para desactivar o debounce (executada polo Ticker)
void IRAM_ATTR desactivarDebounce() 
{
    debounceActivo = false;
    // Opcional: Reactivar a interrupción no pin, aínda que por defecto segue activa 
    // despois do noInterrupts()/interrupts no ISR, só se ignora.
}


// Función de interrupción (ISR): 
void IRAM_ATTR interrupcionLlave()
{
    // A interrupción só se procesa se non estamos en período de debounce.
    if (!debounceActivo) 
    {
        debounceActivo = true; // Activar debounce
        
        // 3. Iniciar o temporizador (one-shot) para chamar a desactivarDebounce() despois de TIEMPO_DEBOUNCE
        // O tempo debe darse en segundos, polo que dividimos por 1000.
        debounceTimer.once((float)TIEMPO_DEBOUNCE / 1000.0, desactivarDebounce); 
        
        // 4. Lóxica de cambio de estado AQUI, DENTRO do ISR, porque é rápido.
        // Se a lóxica fose pesada, só se establecería unha 'flag' e procesaríase en loop().
        estadoLlaveActivado = !estadoLlaveActivado;
        
        // 5. Acción principal (Rápida)
        digitalWrite(PIN_AMARILLO, estadoLlaveActivado);
        
        // Non usar Serial.print dentro do ISR! Usar flag e facelo en loop().
        // Para demostración (só en ESP32 é viable, pero NON recomendado):
        // Serial.printf("LLave accionada (Debounced). Novo estado: %d\n", estadoLlaveActivado); 
    }
}

void setup()
{
    // ... Configuración de Pins ...
    pinMode(PIN_ROJO,OUTPUT);
    pinMode(PIN_AZUL,OUTPUT);
    pinMode(PIN_AMARILLO,OUTPUT); 
    pinMode(PIN_LLAVE, INPUT_PULLUP);

    // 6. Configurar a interrupción. O debounce faise dentro do ISR e co Ticker.
    attachInterrupt(digitalPinToInterrupt(PIN_LLAVE), interrupcionLlave, FALLING);

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
    // O loop queda case baleiro! Todo o debounce e a acción están agora no ISR + Ticker.

    // Podes engadir aquí unha impresión de estado se queres monitorizar sen bloquear o ISR.
    // Serial.println("En loop...");
    // delay(1000); // Exemplo de outra actividade
}