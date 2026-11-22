# Sistema detector de nivel anormal en sumidero

Sistema embebido (ESP32-S3) que mide con un sensor ultrasónico la distancia al agua de un sumidero y avisa de una posible obstrucción o desborde mediante un LED y una página web (banner, sonido y notificación).

> Curso: TEI201 — Taller de diseño en ingeniería · Entrega: Avance 3

> **Todo lo marcado como `COMPLETAR` es información que no estaba en el repositorio ni en el chat de tutoría. Busca `COMPLETAR` en todo el repositorio antes de entregar.**

## Integrantes del equipo

- Olivia Betinyani
- Joaquín Calderón
- Joaquín Zavala
- Antonia Uribe


## Enlaces rápidos

* **Dashboard (conexión por USB):** https://joaco-eng.github.io/Proyecto-tei/
* **Dashboard (conexión por WiFi):** `http://<IP de la placa>` (la IP aparece en el Monitor Serial al conectarse)
* Documentación por carpeta: [Hardware](hardware/README.md) · [Software](software/README.md) · [Diseño 3D](diseno-3d/README.md) · [Testing](testing/README.md) · [Iteraciones](iteraciones/README.md) · [Documentación final](documentacion/README.md) · [Fuentes e IA](FUENTES.md)

---

## 1. ODS y problema a resolver

**ODS:** ODS 11 - Ciudades y comunidades sostenibles: Nuestro proyecto contribuye a este objetivo mediante el desarrollo de un sistema de monitoreo del nivel de agua en sumideros urbanos, que permite detectar aumentos anormales y generar alertas tempranas. De esta manera, buscamos contribuir a la seguridad de las comunidades, en concordancia con la meta 11.5, orientada a reducir el impacto de los desastres, incluidos los relacionados con el agua.

**Problema:** La acumulación excesiva de agua en los sumideros urbanos puede generar desbordamientos y anegamientos en calles y veredas, especialmente durante períodos de lluvias intensas. Estas situaciones pueden afectar la circulación de peatones y vehículos, además de representar un riesgo para la comunidad. Frente a esta problemática, nuestro proyecto busca monitorear el nivel de agua en los sumideros y generar alertas cuando se detecten niveles anormalmente elevados, permitiendo identificar oportunamente posibles situaciones de riesgo.

**Contexto de instalación:** el equipo va montado bajo una rejilla de sumidero tipo SERVIU S2, con el sensor mirando hacia abajo, hacia el agua. La caja es hermética e impresa en 3D (ver [Diseño 3D](diseno-3d/README.md)).

---

## 2. Cómo funciona el sistema

1. La ESP32 dispara el sensor ultrasónico cada 500 ms y calcula la distancia entre el sensor y la superficie del agua.
2. Si la distancia es **≤ 8.0 cm**, el sistema pasa a **VERIFICANDO**. Si el nivel se mantiene así **5 segundos**, declara **ALERTA**. Si el agua baja antes, vuelve a **NORMAL**.
3. La propia ESP32 actúa como servidor web: entrega la página del monitor y el estado actual en `/estado` (JSON).
4. La página muestra la alerta con banner, sonido y notificación del navegador. Puede recibir los datos **por USB** (Web Serial) o **por WiFi**.
5. Si el sensor no recibe eco, el sistema no lo interpreta como "nivel normal": informa el error por el Monitor Serial.

El monitoreo y el Monitor Serial funcionan aunque no haya WiFi.

### Lógica de estados

```text
[ Lectura del sensor ultrasónico impermeable ]
                    |
                    v
            [ Procesamiento ESP32 ]
                    |
                    ├── (Distancia > 8.0 cm) ──→ [ Estado NORMAL ]
                    |
                    └── (Distancia <= 8.0 cm) ─→ [ Estado VERIFICANDO ]
                                                    |
                                            ¿Se mantiene >= 5 s?
                                                    |
                                      ├── SÍ → [ Estado ALERTA ]
                                      |
                                      └── NO → Continúa verificando
                                                (Si el nivel vuelve
                                                a normal, se reinicia)
                    |
                    v
         [ Servidor web de la ESP32 ]
                    |
                    v
         [ Transmisión de datos por WiFi ]
                    |
                    v
              [ Dashboard ]
                    |
                    ├── Muestra distancia medida
                    ├── Muestra estado del sistema
                    └── Genera alerta visual,
                        sonora y notificación
```

## Parámetros de operación

| Parámetro | Valor |
| :--- | :--- |
| Umbral de alerta | distancia ≤ 8.0 cm |
| Confirmación | 5000 ms continuos en nivel crítico |
| Frecuencia de muestreo | una lectura cada 500 ms (`millis()`) |
| Puerto serie | 9600 baudios **Verificar en codigo final** |

---

## 3. Componentes

| Componente | Detalle |
| :--- | :--- |
| Microcontrolador | ESP32-S3 Super Mini |
| Sensor | AJ-SR04M (ultrasónico impermeable, sonda con cable) |
| Divisor de voltaje en ECHO | El sensor funciona a 5 V y la ESP32 a 3.3 V. Valores de las resistencias: **COMPLETAR** |
| Alimentación | Powerbank por USB (100 × 70 × 25 mm). Modelo exacto: **COMPLETAR** |
| PCB perforada | Soporta la ESP32 y el divisor de voltaje (60 × 40 mm, 38 mm de alto) |
| Encapsulado | Caja impresa en 3D (PLA), diseñada en Fusion 360, con tapa fijada mediante tornillos M3 e insertos de bronce. Ver [Diseño 3D](diseno-3d/README.md) |

Lista de materiales con costos: [hardware/README.md](hardware/README.md#lista-de-materiales-bom).

---

## 4. Conexiones (pinout)

| Señal | Pin / GPIO | Detalle de conexión |
| :--- | :--- | :--- |
| **TRIG** | GPIO 5 | 1.er pin lado derecho (visto desde el conector USB) |
| **ECHO** | GPIO 4 | 4.º pin lado izquierdo (visto desde el conector USB), a través del divisor de voltaje |
| **VCC sensor** | 5 V | Alimentación del sensor ultrasónico impermeable |
| **GND** | GND | Tierra común entre la ESP32, el sensor y el divisor de voltaje |
| **Comunicación** | WiFi de la ESP32 | Transmisión inalámbrica de las mediciones al sistema web |

Esquema: [hardware/esquematico.svg](hardware/esquematico.svg).

---

## 5. Puesta en marcha

### 5.1 Conectar el prototipo

1. Conecta el sensor ultrasónico impermeable AJ-SR04M a la ESP32-S3 Super Mini según la tabla de conexiones: VCC al pin de 5 V, GND a tierra, TRIG al GPIO 5 y ECHO al GPIO 4.
2. Verifica que todas las conexiones estén correctamente instaladas.
3. Alimenta la ESP32 mediante la powerbank o el cable USB del computador.

### 5.2 Cargar el código en la ESP32

1. Instala el **Arduino IDE** y, en el Gestor de placas, el paquete **esp32 (Espressif)**. La versión registrada en el log de carga fue **4.0.0** (confirmar en *Herramientas → Placa → Gestor de placas*).
2. No es necesario instalar librerías adicionales: `WiFi`, `WebServer`, `DNSServer` y `Preferences` vienen incluidas en el paquete esp32.
3. Abre `sumidero_wifi/sumidero_wifi.ino` (el archivo `pagina.h` debe encontrarse en la misma carpeta).
4. Configura *Herramientas* de la siguiente manera:

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

5. Selecciona el puerto correspondiente a la ESP32 y pulsa **Subir**.
6. Abre el Monitor Serial a **9600 baudios** para verificar las mediciones del sensor y el estado de la conexión.

### 5.3 Abrir la página y consultar los datos

**Por USB**

1. Abre https://joaco-eng.github.io/Proyecto-tei/ en **Chrome o Edge de escritorio** (utilizando Web Serial).
2. Cierra previamente el Monitor Serial de Arduino IDE para evitar que el puerto quede ocupado.
3. Pulsa **Conectar placa** y selecciona el puerto correspondiente a la ESP32.
4. Una vez establecida la conexión, la página mostrará las mediciones del sensor y el estado del sistema en tiempo real.

**Por WiFi (sin claves en el código)**   **Hay q actualizarlo al servidor** 

1. En el primer uso, la ESP32 crea una red llamada **`Sumidero-Setup`** (sin contraseña). Conéctate desde un celular o computador. Si la página de configuración no se abre automáticamente, ingresa a `http://192.168.4.1`.
2. Selecciona una red WiFi de **2.4 GHz**, ingresa su contraseña y pulsa **Guardar y conectar**. La configuración queda almacenada en la memoria de la ESP32.
3. En el Monitor Serial (9600 baudios) aparecerá el mensaje `WiFi conectado. Abre http://<IP>`, indicando la dirección IP asignada a la placa.
4. Ingresa a esa dirección IP desde un dispositivo conectado a la misma red WiFi. La ESP32 funciona como servidor web local y permite consultar las mediciones y el estado del sistema.
5. La página actualiza periódicamente los datos del sensor y muestra las alertas cuando se detecta un nivel de agua anormalmente elevado.
6. Para cambiar de red WiFi, ingresa a `http://<IP>/olvidar`. La ESP32 eliminará la configuración guardada y volverá a crear la red `Sumidero-Setup`. Esto también ocurre si no consigue conectarse a la red almacenada.

### 5.4 Interfaz de red

La ESP32 funciona como servidor web local, permitiendo consultar las mediciones del sensor y el estado del sistema mediante las siguientes rutas:

- `GET /` → Entrega la página web de monitoreo.
- `GET /estado` → Devuelve las mediciones y el estado actual del sistema en formato JSON.
- `GET /olvidar` → Elimina la configuración WiFi almacenada y reinicia la placa.

**Ejemplo de respuesta de `/estado`:**

```json
{
  "distancia": 7.5,
  "estado": "ALERTA"
}
```

**Estados posibles:**

- **NORMAL:** El nivel de agua se encuentra dentro del rango establecido.
- **VERIFICANDO:** Se detecta un nivel elevado y el sistema comprueba si la condición persiste.
- **ALERTA:** Se confirma un nivel de agua anormalmente elevado y se comunica la alerta a la página web.

---

## 6. Estructura del repositorio

```
├── README.md                  ← este archivo
├── FUENTES.md                 ← librerías, código externo e inteligencia artificial
├── index.html                 ← página web del monitor
├── generar_pagina.sh          ← regenera sumidero_wifi/pagina.h desde index.html
├── sumidero_wifi/             ← firmware de la ESP32 (.ino) y página incrustada (pagina.h)
├── software/                  ← documentación técnica del código, dependencias y configuración
├── hardware/                  ← esquemático, BOM, fotografías, justificación de componentes
├── diseno-3d/                 ← modelo Fusion 360, renders y planos técnicos
├── testing/                   ← protocolo, resultados y análisis de fallas
├── iteraciones/               ← evolución del proyecto (una carpeta por versión)
└── documentacion/             ← reporte final (8 páginas) y presentación del avance 3
```

Si editas `index.html`, ejecuta `./generar_pagina.sh` para actualizar `sumidero_wifi/pagina.h` antes de volver a cargar el firmware.
