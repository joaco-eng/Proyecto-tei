# Sistema detector de nivel anormal en sumidero

Sistema IoT basado en una ESP32-S3 Super Mini y un sensor ultrasónico impermeable AJ-SR04M, diseñado para monitorear el nivel de agua en sumideros urbanos y detectar posibles situaciones de riesgo por acumulación excesiva de agua.

El dispositivo obtiene mediciones de distancia, las procesa y las transmite mediante WiFi a un servidor externo alojado en Railway. Esta información puede utilizarse para visualizar el nivel de agua y generar alertas digitales mediante una plataforma web.

> Curso: TEI201 — Taller de diseño en ingeniería · Entrega: Avance 3

## Integrantes del equipo

- Olivia Betinyani
- Joaquín Calderón
- Joaquín Zavala
- Antonia Uribe

## Enlaces rápidos

* **Dashboard:** https://joaco-eng.github.io/Proyecto-tei/
* **Servidor externo:** Railway (servicio de recepción de mediciones mediante HTTPS).
* **Documentación por carpeta:** [Hardware](hardware/README.md) · [Software](software/README.md) · [Diseño 3D](diseno-3d/README.md) · [Testing](testing/README.md) · [Iteraciones](iteraciones/README.md) · [Documentación final](documentacion/README.md) · [Fuentes e IA](FUENTES.md)

---

## 1. ODS y problema a resolver

**ODS:** ODS 11 - Ciudades y comunidades sostenibles: Nuestro proyecto contribuye a este objetivo mediante el desarrollo de un sistema de monitoreo del nivel de agua en sumideros urbanos, que permite detectar aumentos anormales y generar alertas tempranas. De esta manera, buscamos contribuir a la seguridad de las comunidades, en concordancia con la meta 11.5, orientada a reducir el impacto de los desastres, incluidos los relacionados con el agua.

**Problema:** La acumulación excesiva de agua en los sumideros urbanos puede generar desbordamientos y anegamientos en calles y veredas, especialmente durante períodos de lluvias intensas. Estas situaciones pueden afectar la circulación de peatones y vehículos, además de representar un riesgo para la comunidad. Frente a esta problemática, nuestro proyecto busca monitorear el nivel de agua en los sumideros y generar alertas cuando se detecten niveles anormalmente elevados, permitiendo identificar oportunamente posibles situaciones de riesgo.

**Contexto de instalación:** el equipo va montado bajo una rejilla de sumidero tipo SERVIU S2, con el sensor mirando hacia abajo, hacia el agua. La caja es hermética e impresa en 3D (ver [Diseño 3D](diseno-3d/README.md)).

---

## 2. Cómo funciona el sistema

1. La ESP32-S3 utiliza el sensor ultrasónico impermeable AJ-SR04M para medir la distancia entre el sensor y la superficie del agua.

2. Para obtener una medición más confiable, el programa realiza **5 lecturas consecutivas** y calcula la **mediana** de los valores válidos. De esta manera, se reduce la influencia de lecturas erróneas.

3. El sistema considera válidas las distancias comprendidas entre **20 y 600 cm**. Si no se obtienen al menos tres lecturas válidas, se descarta la medición.

4. Cada **10 segundos**, la ESP32 transmite la distancia obtenida mediante una conexión WiFi a un servidor externo alojado en Railway, utilizando una solicitud HTTPS.

5. El servidor recibe los datos enviados por la ESP32. Estos pueden utilizarse para registrar las mediciones, visualizar la información y generar alertas digitales mediante una plataforma web.

6. Si el sensor no obtiene una medición válida o se produce un error de transmisión, el programa informa la situación mediante el Monitor Serial.

### Lógica de funcionamiento

```text
[ Sensor ultrasónico impermeable AJ-SR04M ]
                     |
                     v
             [ ESP32-S3 ]
                     |
           Realiza 5 lecturas
                     |
                     v
       [ Validación de mediciones ]
                     |
          ¿Hay 3 o más válidas?
                     |
          ├── NO → Descarta la medición
          |
          └── SÍ → Calcula la mediana
                     |
                     v
         [ Conexión WiFi ]
                     |
                     v
       [ Envío HTTPS cada 10 s ]
                     |
                     v
       [ Servidor externo Railway ]
                     |
                     v
       [ Gestión de datos recibidos ]
                     |
                     v
       [ Dashboard y alertas digitales ]
```

### Parámetros de operación

| Parámetro | Valor |
| :--- | :--- |
| Intervalo de envío | Cada 10000 ms (10 segundos) |
| Lecturas por medición | 5 |
| Procesamiento | Cálculo de la mediana |
| Distancia mínima programada | 20 cm |
| Distancia máxima programada | 600 cm |
| Lecturas válidas necesarias | Mínimo 3 de 5 |
| Puerto serie | 115200 baudios |
| Conectividad | WiFi de 2.4 GHz |
| Servidor | Railway |
| Protocolo de comunicación | HTTPS |
| Umbral de alerta digital | **COMPLETAR: confirmar configuración del servidor** |

---

## 3. Componentes

| Componente | Detalle |
| :--- | :--- |
| Microcontrolador | ESP32-S3 Super Mini |
| Sensor | AJ-SR04M (ultrasónico impermeable, sonda con cable) |
| Alimentación | Powerbank por USB (100 × 70 × 25 mm). Modelo exacto: **COMPLETAR** |
| PCB perforada | Soporta la ESP32 y las conexiones del sensor (60 × 40 mm, 38 mm de alto) |
| Encapsulado | Caja impresa en 3D (PLA), diseñada en Fusion 360, con tapa fijada mediante tornillos M3 e insertos de bronce. Ver [Diseño 3D](diseno-3d/README.md) |

Lista de materiales con costos: [hardware/README.md](hardware/README.md#lista-de-materiales-bom).

---

## 4. Conexiones (pinout)

| Señal | Pin / GPIO | Detalle de conexión |
| :--- | :--- | :--- |
| **TRIG** | GPIO 5 | 1.er pin lado derecho (visto desde el conector USB) |
| **ECHO** | GPIO 4 | 4.º pin lado izquierdo (visto desde el conector USB) |
| **VCC sensor** | 5 V | Alimentación del sensor ultrasónico impermeable |
| **GND** | GND | Tierra común entre la ESP32 y el sensor |
| **Comunicación** | WiFi de la ESP32 | Transmisión inalámbrica de las mediciones al sistema web |

Esquema: [hardware/esquematico.svg](hardware/esquematico.svg).

---

## 5. Puesta en marcha

### 5.1 Conectar el prototipo

1. Conecta el sensor ultrasónico impermeable AJ-SR04M a la ESP32-S3 Super Mini según la tabla de conexiones: VCC al pin de 5 V, GND a tierra, TRIG al GPIO 5 y ECHO al GPIO 4.
2. Verifica que todas las conexiones estén correctamente instaladas.
3. Alimenta la ESP32 mediante la powerbank o el cable USB del computador.


### 5.2 Cargar el código en la ESP32

1. Instala **Arduino IDE** y el paquete de placas **esp32 (Espressif)** desde el Gestor de placas.

2. Instala la librería **ArduinoJson versión 7** desde el Gestor de Bibliotecas. Las librerías `WiFi.h`, `WiFiClientSecure.h` y `HTTPClient.h` vienen incluidas en el paquete ESP32.

3. Abre el archivo `monitor_sumidero.ino`, correspondiente al programa final del dispositivo.

4. Configura los datos necesarios para la conexión WiFi y la comunicación con el servidor:

   - `WIFI_SSID`: nombre de la red WiFi.
   - `WIFI_PASS`: contraseña de la red.
   - `SERVER_URL`: dirección del servidor externo.
   - `API_KEY`: clave utilizada para autorizar el envío de mediciones.
   - `DISPOSITIVO`: identificador de la ESP32.

   **Importante:** las contraseñas y claves reales no deben publicarse en GitHub.

5. Configura las opciones de la placa en Arduino IDE:

| Opción | Valor |
| :--- | :--- |
| Board | ESP32S3 Dev Module |
| USB CDC On Boot | Enabled |
| USB Mode | Hardware CDC and JTAG |
| CPU Frequency | 240MHz (WiFi) |
| Flash Size | 4MB (32Mb) |
| Flash Mode | QIO 80MHz |
| Partition Scheme | Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS) |
| PSRAM | Disabled |
| Upload Speed | 921600 |

**Nota:** comprobar que estas opciones coincidan con la configuración utilizada para cargar el programa final.

6. Selecciona el puerto correspondiente a la ESP32 y presiona **Subir**.

7. Abre el Monitor Serial a **115200 baudios** para comprobar las mediciones y la comunicación con el servidor.

### 5.3 Conexión WiFi y consulta de datos

**Conexión WiFi**

1. La ESP32 utiliza las credenciales configuradas en el programa para conectarse automáticamente a una red WiFi de **2.4 GHz**.

2. Una vez establecida la conexión, la ESP32 obtiene una dirección IP dentro de la red y queda habilitada para comunicarse con el servidor mediante Internet.

3. Cada 10 segundos, el dispositivo procesa las mediciones del sensor y envía los datos válidos a una API externa alojada en **Railway**, utilizando una solicitud HTTPS.

4. Si se pierde la conexión WiFi, el programa intenta restablecerla para continuar enviando información.

5. El Monitor Serial permite verificar si la conexión está activa y observar las respuestas del servidor ante cada envío.

**Consulta de datos mediante la página web**

1. La plataforma web permite consultar las mediciones recibidas por el sistema y visualizar la información del sumidero.

2. A diferencia de la versión anterior, la ESP32 no funciona como servidor web local. Su función es enviar las mediciones al servidor externo.

3. El dashboard debe comunicarse con el servidor para obtener los datos y presentar las mediciones, el historial y las alertas correspondientes.

4. Al utilizar un servidor externo accesible por Internet, el dispositivo que consulta el dashboard no necesita estar conectado a la misma red WiFi de la ESP32.

**Enlace del dashboard:** https://joaco-eng.github.io/Proyecto-tei/

**COMPLETAR:** confirmar que la versión final del dashboard obtiene los datos directamente del servidor Railway.

### 5.4 Comunicación con el servidor

La ESP32 transmite las mediciones a una API alojada en Railway mediante solicitudes HTTPS de tipo POST.

El programa utiliza una clave de autenticación para autorizar el envío de información.

**Datos enviados al servidor:**

```json
{
  "dispositivo": "esp32-sumidero",
  "distancia_cm": 45.5,
  "bateria_v": null
}
```

Este ejemplo representa el formato de los datos enviados:

- `dispositivo`: identificador de la ESP32.
- `distancia_cm`: distancia medida por el sensor, expresada en centímetros.
- `bateria_v`: valor de batería, actualmente sin medición.

La información se envía cada 10 segundos, siempre que exista una medición válida y conexión a Internet.

El programa muestra en el Monitor Serial la respuesta del servidor, permitiendo identificar si el envío fue recibido o si ocurrió algún error.

**Servidor utilizado:** Railway.

**Dirección de la API:** https://monitor-sumidero-production.up.railway.app

**Almacenamiento de datos:** COMPLETAR según la implementación del servidor.

**Consulta de mediciones históricas:** COMPLETAR.

**Procesamiento de alertas y umbrales:** COMPLETAR según la lógica implementada en el servidor.

---

