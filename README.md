# 📡 Escáner WiFi y Cliente TCP (ESP32)

Este proyecto implementa un sistema embebido que funciona como un **Escáner de Redes Inalámbricas** y **Cliente TCP** para la recolección y envío de datos telemétricos. El sistema está diseñado para operar en diferentes modos de energía y funcionalidad, controlados mediante entradas físicas (llave/interruptor y botones).

El software ha sido desarrollado por Marcelo Fort Muñoz y Víctor Arrollo Marquez y está distribuido bajo la licencia **GNU General Public License (GPL) v3.0 o posterior**.

---

## 🚀 Modos de Operación

El dispositivo opera según una máquina de estados finitos (FSM) gestionada por la variable `g_configuracion.modoActual`.

| Modo | Descripción | Activación |
| :--- | :--- | :--- |
| **MODO_ESCANER_WIFI** | Modo por defecto. El sistema escanea periódicamente las redes WiFi cercanas (`K_INTERVALO_ESCANEO` = 15s) y muestra la información (SSID, RSSI) en una pantalla TFT. | Por defecto al iniciar. Al finalizar el envío TCP. |
| **MODO_CLIENTE_TCP** | Modo de transmisión. Establece una conexión con un servidor TCP remoto para enviar los datos de las redes escaneadas. | Activado por la interrupción RISING del `PIN_LLAVE`. |
| **MODO_REPOSO** | Modo de bajo consumo (Deep Sleep) para ahorrar batería. El dispositivo solo puede ser despertado por una interrupción externa. | Activado por la interrupción RISING del `PIN_BOTON_DORMIR`. |

---

## 🛠️ Hardware y Pines

El proyecto está diseñado para un microcontrolador ESP32 que interactúa con varios periféricos, incluyendo una pantalla TFT, LEDs de estado y entradas físicas (llave/botón).

### Pines Utilizados

| Componente | Pin (Definición) | Tipo | Función |
| :--- | :--- | :--- | :--- |
| **LED Conexión** | `PIN_LED_CONECTADO` (37) | Salida | Indica que la conexión WiFi está establecida. |
| **LED No Conexión** | `PIN_LED_NO_CONECTADO` (36) | Salida | Indica que la conexión WiFi no está establecida. |
| **LED Transmisión** | `PIN_LED_ESTADO_TRANSMISION` (38) | Salida | Indica que el dispositivo está en `MODO_CLIENTE_TCP` (Transmisión). |
| **Llave/Interruptor** | `PIN_LLAVE` (21) | Entrada (PULLUP) | Activa la transición al `MODO_CLIENTE_TCP`. |
| **Botón Despertar** | `PIN_BOTON_DESPERTAR` (GPIO\_NUM\_2) | Entrada (PULLDOWN) | Despierta el dispositivo del `MODO_REPOSO`. |
| **Botón Dormir** | `PIN_BOTON_DORMIR` (7) | Entrada (PULLDOWN) | Activa el `MODO_REPOSO` (Deep Sleep). |

### Pines SPI y TFT

| Componente | Pin (Definición) | Función |
| :--- | :--- | :--- |
| **SPI Clock** | `SPI_SCK` (12) | Reloj para comunicación SPI. |
| **SPI MOSI** | `SPI_MOSI` (11) | Master Out Slave In. |
| **SPI MISO** | `SPI_MISO` (13) | Master In Slave Out. |
| **TFT Chip Select** | `TFT_CS` (10) | Selección del chip de la pantalla. |
| **TFT Data/Command** | `TFT_DC` (8) | Alterna entre datos y comandos. |
| **TFT Reset** | `TFT_RST` (4) | Reset de la pantalla. |

---

## 💻 Configuración de Conectividad

### WiFi Estática
El dispositivo se conecta a la red WiFi configurada con credenciales estáticas:
* **SSID:** `SSID`
* **Contraseña:** `CONTRASEÑA`
* **IP Estática:** `192.168.1.248`
* **Gateway:** `192.168.1.1`
* **Subnet:** `255.255.255.0`

### Servidor TCP
Los datos escaneados se envían al siguiente servidor:
* **Dirección IP:** `192.168.1.19` (`DIRECCION_IP_SERVIDOR`)
* **Puerto:** `1234`

---

## 📂 Estructura del Código

| Archivo | Descripción Principal |
| :--- | :--- |
| `main.ino` | Lógica principal (Setup, Loop) y gestión de la Máquina de Estados (FSM). Inicializa periféricos (SPI, TFT, Timers) y maneja las interrupciones (llave, dormir). |
| `constantes.h` | Define tiempos, pines GPIO, límites de conectividad y la dirección del servidor TCP. |
| `tipos.h` | Define las estructuras de datos clave, como la estructura `Red` (SSID y RSSI) y el enumerador `EstadoLeds`. |
| `wifi_scanner.h`/`.cpp` | Implementa el escaneo de redes WiFi y la visualización de los resultados en la pantalla TFT (`escanearYmostrar()`). |
| `cliente_tcp.h`/`.cpp` | Gestiona la lógica del `MODO_CLIENTE_TCP`, incluyendo la conexión al servidor (`conectarServidor()`) y el formateo/envío de los datos de las redes (`enviarRedesAlServidor()`). |
| `conectividad_cliente_tcp.cpp` | Funciones de bajo nivel para la conectividad TCP (creación de socket, conexión, envío, desconexión). |
| `leds.h`/`.cpp` | Funciones para controlar los LEDs indicadores de estado (`cambiarSituacionLeds()`). |

## ⚙️ Instrucciones de Compilación y Uso

Esta sección describe los pasos necesarios para compilar, cargar y ejecutar el código en el dispositivo **ESP32**.

### 1. Requisitos de Software

Asegúrate de tener instalado el entorno de desarrollo y las dependencias necesarias:

* **Arduino IDE** (Recomendado) o **PlatformIO**.
* **Soporte para ESP32:** Debes tener instalada la placa ESP32 en el gestor de tarjetas de tu IDE.

### 2. Librerías Necesarias

Este proyecto depende de las siguientes librerías, las cuales deben ser instaladas a través del **Gestor de Librerías** del Arduino IDE:

* **Adafruit GFX Library**
* **Adafruit ILI9341**
* **WiFi**

### 3. Configuración del Entorno

Antes de compilar, verifica la configuración de los parámetros estáticos en los archivos del proyecto.

#### A. Pines y Tiempos (`constantes.h`)

Revisa y ajusta los pines GPIO y los tiempos si tu hardware difiere de la configuración base:

* **Pines de la Pantalla TFT** (`TFT_CS`, `TFT_DC`, `TFT_RST`).
* **Pines de los LEDs** (`PIN_LED_CONECTADO`, `PIN_LED_NO_CONECTADO`, etc.).
* **Tiempo entre lecturas de la llave** (`K_PERIODO_NO_LLAVE`): 1000 ms.
* **Intervalo de escaneo WiFi** (`K_INTERVALO_ESCANEO`): 15,000,000 us (15 segundos).

#### B. Conectividad WiFi (`main.ino`)

Asegúrate de que las credenciales estáticas coincidan con tu red local, modificándolas en el archivo `main.ino` si es necesario:

```cpp
    const char* ssid = "SSUD"; // Ajusta a tu SSID
    const char* password = "CONTRASEÑA"; // Ajusta a tu contraseña
    IPAddress staticIp(192, 168, 1, 248); // Ajusta la IP estática deseada
```
#### C. Servidor TCP/IP
Compila el archivo `/src/codigo_servidor_tcp/servidorTCP.c` y ejecutalo con los parámetros: ./servidorTCP <PUERTO> <ARCHIVO_LOG>
