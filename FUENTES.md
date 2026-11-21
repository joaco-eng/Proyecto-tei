# Declaración de fuentes e inteligencia artificial

> Este documento debe reflejar lo que el grupo realmente hizo. Todo lo marcado como `COMPLETAR` lo tiene que responder el grupo; no se completó por ellos porque no consta en el repositorio ni en el chat de tutoría.

## 1. Librerías

### 1.1 Firmware de la ESP32 (`sumidero_wifi/sumidero_wifi.ino`, versión final)

| Librería | Versión | Para qué sirve | De dónde proviene |
| :--- | :--- | :--- | :--- |
| `WiFi.h` | La del paquete esp32 | Conexión a la red WiFi y modo punto de acceso | Incluida en el ESP32 Arduino core (Espressif, https://github.com/espressif/arduino-esp32) |
| `WebServer.h` | La del paquete esp32 | Servidor HTTP en la placa (`/`, `/estado`, `/guardar`, `/olvidar`) | Incluida en el ESP32 Arduino core |
| `DNSServer.h` | La del paquete esp32 | Portal cautivo: redirige cualquier dirección a la página de configuración | Incluida en el ESP32 Arduino core |
| `Preferences.h` | La del paquete esp32 | Guardar la red WiFi en la memoria de la placa | Incluida en el ESP32 Arduino core |

No se instalaron librerías externas con el Gestor de librerías para la versión final.

**Entorno de desarrollo**

| Herramienta | Versión | Observación |
| :--- | :--- | :--- |
| ESP32 Arduino core (paquete `esp32`) | 4.0.0 | Aparece en la ruta del log de carga (`...\packages\esp32\hardware\esp32\4.0.0\...`). COMPLETAR: confirmar en el Gestor de placas. |
| esptool | v5.3.1 | La usa el IDE para cargar el programa (log de carga) |
| Arduino IDE | COMPLETAR | En el chat se dijo "no el 2", pero las capturas parecen del IDE 2.x |
| Placa seleccionada | ESP32S3 Dev Module | Chip ESP32-S3 (QFN56), 4 MB de flash |
| Git, VS Code, GitHub | COMPLETAR (versiones) | Control de versiones y edición |
| GitHub Pages | — | Aloja la página para el modo USB: https://joaco-eng.github.io/Proyecto-tei/ |

### 1.2 Página web (`index.html`)

No usa librerías ni recursos externos (no carga scripts ni estilos desde internet). Usa solo APIs integradas del navegador:

| API | Para qué sirve |
| :--- | :--- |
| Web Serial | Leer los datos de la placa por USB (solo Chrome/Edge de escritorio) |
| Fetch | Consultar `/estado` de la placa por WiFi |
| Web Audio | Sonido de alarma |
| Notifications | Notificación del navegador al declararse una alerta |
| localStorage | Recordar la última IP usada |

### 1.3 Versión con servidor en la nube (Railway + Aiven)

Se trabajó en el chat de tutoría pero **no está en este repositorio**. COMPLETAR: si se incluye en la entrega, dejar esta tabla; si se descartó, borrar esta sección y explicarlo en `iteraciones/`.

| Librería / paquete | Versión | Para qué sirve | De dónde proviene |
| :--- | :--- | :--- | :--- |
| `WiFiClientSecure.h`, `HTTPClient.h` | La del paquete esp32 | Enviar los datos por HTTPS | ESP32 Arduino core |
| ArduinoJson | v7 (versión exacta: COMPLETAR) | Armar el JSON que envía la ESP32 | Gestor de librerías, Benoit Blanchon |
| express | ^4.21.0 | Servidor web y API | npm |
| pg | ^8.13.0 | Conexión a PostgreSQL | npm |
| dotenv | ^16.4.5 | Leer variables del archivo `.env` en pruebas locales | npm |
| Node.js | >= 18 (versión instalada: COMPLETAR) | Ejecutar el servidor | https://nodejs.org |
| Chart.js | COMPLETAR (la guía lo carga desde https://cdn.jsdelivr.net/npm/chart.js) | Gráficos en la página de la guía | CDN jsDelivr |

## 2. Código externo

| Fuente | Qué parte se tomó | Observación |
| :--- | :--- | :--- |
| Guía del curso: «ESP32 conectado a una base de datos en la nube — Aiven (PostgreSQL) + Railway + GitHub», Fernando Méndez, TEI201, revisada el 14 de noviembre de 2026 | `package.json`, `server.js`, `.gitignore`, `.env.example`, `public/index.html`, `esp32_railway.ino` y consultas SQL (solo para la versión Railway/Aiven) | El grupo adaptó el código (tabla `mediciones`, ruta `/api/mediciones`, variable `ALTURA_SENSOR_CM`) |
| Repositorio de ejemplo https://github.com/ferjomendez/esp32-railway-demo | El mismo código de la guía | No se hizo fork; el grupo creó su propio repositorio |
| `Avance2_oct12a.ino` | Base del firmware final (el encabezado de `sumidero_wifi.ino` dice "Basado en Avance2_oct12a.ino + conexión WiFi") | COMPLETAR: quién lo escribió y con qué herramienta. No está en el repositorio. |
| `monitor_sumidero.ino` (versión con WiFi y envío a Railway) | Todo el archivo | COMPLETAR: autoría. Lo pegó un integrante en el chat; no lo generó la IA allí. Contiene claves: **no subirlo a GitHub**. |
| Portal cautivo y servidor web de `sumidero_wifi.ino` | Funciones `iniciarPortal`, `enviarPortal`, `guardarRed`, `olvidarRed` | COMPLETAR: ¿se basaron en algún ejemplo del paquete esp32, tutorial o repositorio? Indicar la URL. |
| Otros tutoriales o proyectos | — | COMPLETAR |

## 3. Inteligencia artificial

### 3.1 Claude (Anthropic) en el chat de tutoría — 16 al 21 de noviembre de 2026

* **Modelo:** Claude Opus 5.5.
* **Para qué se usó:** como tutor paso a paso para montar la arquitectura de la guía, siguiendo las reglas del prompt de la guía (el grupo ejecuta y prueba; nunca se comparten claves).

| Qué generó o adaptó | Cómo se verificó o modificó |
| :--- | :--- |
| `server.js` adaptado: tabla `mediciones`, ruta `/api/mediciones`, `ALTURA_SENSOR_CM`, cálculo del nivel en el servidor, `bateria_v` opcional. También `package.json`, `.gitignore` y `.env.example` | El grupo los ejecutó. Hubo errores al nombrar los archivos (`package.jsn`, `server.jsn`) que se corrigieron. COMPLETAR: resultado final de `npm install` y `node --check server.js` |
| Un prompt para Claude Code que pedía construir ese servidor | COMPLETAR: si se usó y qué resultó |
| `prueba_sensor.ino` (primera versión: sensor sin WiFi, mediana de 5 lecturas) | El usuario lo modificó (ECHO al pin 4, 9600 baudios) |
| Versión modificada de `monitor_sumidero.ino` (muestra cada 2 s, explica por qué falla cada lectura, alerta si no hay WiFi en 20 s) | No se confirmó que el grupo la usara |
| `prueba.ino` (diagnóstico del sensor sin WiFi) | Se cargó en la placa. Resultado: "el sensor no responde" (captura del monitor serie) |
| `prueba_vida.ino` (parpadeo del LED RGB del pin 48 y texto "vivo") | Se cargó en la placa. El LED parpadeó; el texto no llegó al monitor |
| Comandos de terminal (`git clone`, `Set-ExecutionPolicy -Scope CurrentUser RemoteSigned`, lectura del puerto COM4 en PowerShell) y diagnóstico de errores de instalación y de puerto | Se ejecutaron y se mostró la salida |

Corrección hecha por la propia IA: primero sugirió medir la batería de la powerbank con un divisor de voltaje; después corrigió que la powerbank entrega 5 V regulados y ese voltaje no indica la carga. La medición de batería se descartó (`bateria_v = null`).

### 3.2 Claude Code
Se instaló con el comando de PowerShell indicado en https://code.claude.com/docs/en/setup. COMPLETAR: si se usó finalmente, qué generó y cómo se verificó.

### 3.3 Claude (Anthropic) en claude.ai — 21 de noviembre de 2026
* **Modelo:** Claude Sonnet 5.5.
* **Para qué se usó:** leer el repositorio y el informe de recopilación para redactar **borradores** de documentación: `README.md`, `FUENTES.md`, las plantillas de las carpetas `hardware/`, `software/`, `diseno-3d/`, `testing/`, `iteraciones/`, `documentacion/` y un esquemático en borrador (`hardware/esquematico.svg`).
* **Cómo se verificó:** COMPLETAR (el grupo debe contrastar cada dato con el prototipo real y reemplazar los `COMPLETAR`).

### 3.4 Código final del repositorio
| Archivo | ¿Se usó IA? | Herramienta, para qué y cómo se verificó |
| :--- | :--- | :--- |
| `sumidero_wifi/sumidero_wifi.ino` | COMPLETAR | COMPLETAR |
| `index.html` y `sumidero_wifi/pagina.h` | COMPLETAR | COMPLETAR |
| `generar_pagina.sh` | COMPLETAR | COMPLETAR |
| Informes, presentación y diseño 3D | COMPLETAR | COMPLETAR |

### 3.5 Otras herramientas de IA
ChatGPT, Gemini, Copilot u otras: COMPLETAR (si no se usó ninguna, escribirlo explícitamente).
