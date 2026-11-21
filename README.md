# Sistema detector de nivel anormal en sumidero

Sistema embebido (ESP32-S3) que mide con un sensor ultrasónico la distancia al agua de un sumidero y avisa de una posible obstrucción o desborde mediante un LED y una página web (banner, sonido y notificación).

> Curso: TEI201 — Taller de diseño en ingeniería · Entrega: Avance 3

> **Todo lo marcado como `COMPLETAR` es información que no estaba en el repositorio ni en el chat de tutoría. Busca `COMPLETAR` en todo el repositorio antes de entregar.**

## Integrantes

| Nombre completo | Rol en el proyecto |
| :--- | :--- |
| COMPLETAR | COMPLETAR |
| COMPLETAR | COMPLETAR |

## Enlaces rápidos

* **Dashboard (conexión por USB):** https://joaco-eng.github.io/Proyecto-tei/
* **Dashboard (conexión por WiFi):** `http://<IP de la placa>` (la IP aparece en el Monitor Serial al conectarse)
* Documentación por carpeta: [Hardware](hardware/README.md) · [Software](software/README.md) · [Diseño 3D](diseno-3d/README.md) · [Testing](testing/README.md) · [Iteraciones](iteraciones/README.md) · [Documentación final](documentacion/README.md) · [Fuentes e IA](FUENTES.md)

---

## 1. ODS y problema a resolver

**ODS:** COMPLETAR (número, nombre y por qué el proyecto contribuye a él).

**Problema:** COMPLETAR (qué ocurre cuando un sumidero se obstruye, a quién afecta, datos o cifras con su fuente).

**Contexto de instalación:** el equipo va montado bajo una rejilla de sumidero tipo SERVIU S2, con el sensor mirando hacia abajo, hacia el agua. La caja es hermética e impresa en 3D (ver [Diseño 3D](diseno-3d/README.md)).

---

## 2. Cómo funciona el sistema

1. La ESP32 dispara el sensor ultrasónico cada 500 ms y calcula la distancia entre el sensor y la superficie del agua.
2. Si la distancia es **≤ 8.0 cm**, el sistema pasa a **VERIFICANDO**. Si el nivel se mantiene así **5 s seguidos**, declara **ALERTA** y enciende el LED (pin 6). Si el agua baja antes, vuelve a **NORMAL**.
3. La propia ESP32 actúa como servidor web: entrega la página del monitor y el estado actual en `/estado` (JSON).
4. La página replica la alerta del LED con banner, sonido y notificación del navegador. Puede recibir los datos **por USB** (Web Serial) o **por WiFi**.
5. Si el sensor no recibe eco, el sistema no lo interpreta como "nivel normal": informa el error por el Monitor Serial.

El monitoreo, el LED y el Monitor Serial funcionan aunque no haya WiFi.

### Lógica de estados

```
[ Lectura Sensor ]
        │
        ├── (Distancia > 8.0 cm) ───> [ Estado NORMAL ] ──> LED OFF
        │
        └── (Distancia <= 8.0 cm) ──> [ Estado VERIFICANDO ]
                                              │
                                   ¿Se mantiene >= 5 s?
                                         ├── SÍ ──> [ Estado ALERTA ] ──> LED ON
                                         └── NO ──> Continúa verificando
```

### Parámetros de operación

| Parámetro | Valor |
| :--- | :--- |
| Umbral de alerta | distancia ≤ 8.0 cm |
| Confirmación | 5000 ms continuos en nivel crítico |
| Frecuencia de muestreo | una lectura cada 500 ms (`millis()`) |
| Puerto serie | 9600 baudios |

---

## 3. Componentes

| Componente | Detalle | Estado |
| :--- | :--- | :--- |
| Microcontrolador | ESP32-S3 Super Mini | Confirmado (chip ESP32-S3 en el log de carga) |
| Sensor | AJ-SR04M (ultrasónico, sonda con cable) | Confirmado |
| Divisor de voltaje en ECHO | El sensor va a 5 V y la ESP32 trabaja a 3.3 V | Valores de las resistencias: COMPLETAR |
| LED + resistencia de 220 Ω | Indicador de alerta, pin 6 | Confirmado |
| Alimentación | Powerbank por USB (100 × 70 × 25 mm) | Modelo: COMPLETAR |
| PCB perforada | Soporta la ESP32 y el divisor (60 × 40 mm, 38 mm de alto) | Confirmado |
| Encapsulado | Caja impresa en 3D (PLA), diseñada en Fusion 360, tapa con tornillos M3 e insertos de bronce | Ver [Diseño 3D](diseno-3d/README.md) |

Lista de materiales con costos: [hardware/README.md](hardware/README.md#lista-de-materiales-bom).

---

## 4. Conexiones (pinout)

| Señal | Pin / GPIO | Detalle de conexión |
| :--- | :--- | :--- |
| **TRIG** | GPIO 5 | 1.er pin lado derecho (visto desde el conector USB) |
| **ECHO** | GPIO 4 | 4.º pin lado izquierdo (visto desde el conector USB), a través del divisor de voltaje |
| **LED** | GPIO 6 | 2.º pin lado derecho, en serie con resistencia de 220 Ω |
| **VCC sensor** | 5 V | |
| **GND** | GND | Común entre ESP32, sensor y LED |

Esquema: [hardware/esquematico.svg](hardware/esquematico.svg).

---

## 5. Puesta en marcha

### 5.1 Conectar el prototipo
1. Conecta el sensor y el LED según la tabla anterior (ECHO siempre pasa por el divisor de voltaje).
2. Alimenta la placa con la powerbank o con el cable USB del computador.

### 5.2 Cargar el código en la ESP32
1. Instala el **Arduino IDE** y, en el Gestor de placas, el paquete **esp32 (Espressif)**. La versión usada según el log de carga fue **4.0.0** (COMPLETAR: confirmar en *Herramientas → Placa → Gestor de placas*).
2. No hay que instalar librerías adicionales: `WiFi`, `WebServer`, `DNSServer` y `Preferences` vienen con el paquete esp32.
3. Abre `sumidero_wifi/sumidero_wifi.ino` (el archivo `pagina.h` debe estar en la misma carpeta).
4. Configura *Herramientas* así:

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

5. Elige el puerto de la placa y pulsa **Subir**. Abre el Monitor Serial a **9600** baudios para ver el estado.

### 5.3 Abrir la página y consultar los datos

**Por USB**
1. Abre https://joaco-eng.github.io/Proyecto-tei/ en **Chrome o Edge de escritorio** (Web Serial no funciona en otros navegadores).
2. Cierra antes el Monitor Serial del IDE (si no, el puerto queda ocupado).
3. Pulsa **Conectar placa** y elige el puerto.

**Por WiFi (sin claves en el código)**
1. Primer uso: la placa crea la red **`Sumidero-Setup`** (sin clave). Conéctate desde el celular o el computador; se abre sola la página de configuración (si no, entra a `http://192.168.4.1`).
2. Elige tu red de 2.4 GHz, escribe la clave y pulsa **Guardar y conectar**. La red queda guardada en la memoria de la placa.
3. En el Monitor Serial (9600) aparece `WiFi conectado. Abre http://<IP>`. Abre esa dirección desde un equipo en la misma red; la placa sirve la página y se conecta sola.
4. Para cambiar de red entra a `http://<IP>/olvidar`: la placa borra la red guardada y vuelve a abrir `Sumidero-Setup`. También lo hace sola si no logra conectarse a la red guardada.

### 5.4 Interfaz de red

* `GET /` → página web.
* `GET /estado` → `{"distancia":7.5,"estado":"ALERTA"}` (estados: `NORMAL`, `VERIFICANDO`, `ALERTA`).
* `GET /olvidar` → borra la red WiFi guardada y reinicia.

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
