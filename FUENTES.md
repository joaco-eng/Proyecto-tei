# Declaración de fuentes e inteligencia artificial

> Este documento declara las librerías, el código externo y las herramientas de inteligencia artificial utilizadas en el proyecto. Lo marcado como `COMPLETAR` debe responderlo el grupo.

## 1. Librerías y herramientas

### 1.1 Firmware de la ESP32 (versión final)

Archivo: [`monitor_sumidero.ino`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/esp32/monitor_sumidero/monitor_sumidero.ino)

| Librería | Versión | Para qué sirve | De dónde proviene |
| :--- | :--- | :--- | :--- |
| `WiFi.h` | La del paquete esp32 | Conexión a la red WiFi | Incluida en el ESP32 Arduino core (Espressif, https://github.com/espressif/arduino-esp32) |
| `WiFiClientSecure.h` | La del paquete esp32 | Conexión cifrada (HTTPS) con el servidor | Incluida en el ESP32 Arduino core |
| `HTTPClient.h` | La del paquete esp32 | Envío de las mediciones por POST | Incluida en el ESP32 Arduino core |
| ArduinoJson | 7.4.3 | Armar el JSON que envía la ESP32 | Gestor de librerías de Arduino, autor Benoit Blanchon (https://arduinojson.org) |

### 1.2 Servidor (Railway)

Archivos: [`server.js`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/server.js), [`package.json`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/package.json), [`package-lock.json`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/package-lock.json)

| Paquete | Versión instalada | Para qué sirve | De dónde proviene |
| :--- | :--- | :--- | :--- |
| express | 4.22.3 (`^4.21.0` en `package.json`) | Servidor web y API | npm |
| pg | 8.23.1 (`^8.13.0`) | Conexión a PostgreSQL | npm |
| dotenv | 16.6.1 (`^16.4.5`) | Leer variables del archivo `.env` en pruebas locales | npm |
| Node.js | `>=18` según `package.json`; 26.13.1 en el computador de desarrollo | Ejecutar el servidor | https://nodejs.org |
| PostgreSQL | Servicio administrado | Base de datos de las mediciones | Aiven (https://aiven.io) |

### 1.3 Página web (dashboard)

Archivo: [`public/index.html`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/public/index.html)

| Recurso | Versión | Para qué sirve | De dónde proviene |
| :--- | :--- | :--- | :--- |
| Chart.js | 4.4.1 (versión fija) | Gráfico del nivel de agua | CDN jsDelivr (https://cdn.jsdelivr.net/npm/chart.js@4.4.1) |
| Fetch API | Integrada en el navegador | Consultar `/api/mediciones` cada 5 s | API del navegador |

### 1.4 Versiones anteriores

Archivos en [`versiones-anteriores/`](versiones-anteriores/): `sumidero_wifi.ino`, `index.html`, `pagina.h`, `generar_pagina.sh`. En esa versión la ESP32 funcionaba como servidor web local.

| Librería / API | Para qué servía | De dónde proviene |
| :--- | :--- | :--- |
| `WebServer.h` | Servidor HTTP en la placa (`/`, `/estado`, `/guardar`, `/olvidar`) | ESP32 Arduino core |
| `DNSServer.h` | Portal cautivo `Sumidero-Setup` para configurar la red WiFi | ESP32 Arduino core |
| `Preferences.h` | Guardar la red WiFi en la memoria de la placa | ESP32 Arduino core |
| Web Serial, Fetch, Web Audio, Notifications, localStorage | Leer la placa por USB, consultar `/estado`, sonido y notificación de alerta, recordar la IP | APIs integradas del navegador (sin librerías externas) |
| GitHub Pages | Alojar la página para el modo USB: https://joaco-eng.github.io/Proyecto-tei/ | GitHub |

### 1.5 Entorno de desarrollo

| Herramienta | Versión | Observación |
| :--- | :--- | :--- |
| Arduino IDE | 2.3.11 | Programación y carga del firmware |
| ESP32 Arduino core (paquete `esp32`) | 3.3.13 en el computador de desarrollo | Un log de carga anterior mostraba la versión 4.0.0. COMPLETAR: confirmar cuál se usó en la carga final. |
| esptool | 5.3.1 | La usa el IDE para cargar el programa |
| Placa seleccionada | ESP32S3 Dev Module | Chip ESP32-S3 (QFN56), 4 MB de flash |
| Node.js / npm | 26.13.1 / 11.20.0 | Ejecutar y probar el servidor en local |
| Git | 2.56.1 | Control de versiones |
| Visual Studio Code | 1.146.0 | Edición de código |
| GitHub | — | Repositorios [Proyecto-tei](https://github.com/joaco-eng/Proyecto-tei) (documentación) y [Monitor-Sumidero](https://github.com/JoaquinZavalaM/Monitor-Sumidero) (código y diseño 3D) |
| Railway | Servicio en la nube | Alojamiento del servidor |
| Aiven | Servicio en la nube | Alojamiento de PostgreSQL |
| Fusion 360 | COMPLETAR | Diseño 3D del encapsulado |

## 2. Código externo

| Fuente | Qué parte se tomó | Observación |
| :--- | :--- | :--- |
| Guía del curso: «ESP32 conectado a una base de datos en la nube — Aiven (PostgreSQL) + Railway + GitHub», Fernando Méndez, TEI201, revisada el 14 de noviembre de 2026 | `package.json`, `server.js`, `.gitignore`, `.env.example`, `public/index.html`, `esp32_railway.ino` y consultas SQL | El grupo adaptó el código: tabla `mediciones`, ruta `/api/mediciones`, variable `ALTURA_SENSOR_CM` y cálculo del nivel en el servidor |
| Repositorio de ejemplo https://github.com/ferjomendez/esp32-railway-demo | El mismo código de la guía | No se hizo fork; el grupo creó su propio repositorio |
| `Avance2_oct12a.ino` | Base de `sumidero_wifi.ino` (su encabezado dice "Basado en Avance2_oct12a.ino + conexión WiFi") | Autor: Joaquin Zavala / Claude |
| `monitor_sumidero.ino` | Firmware final completo |  |
| Portal cautivo de `sumidero_wifi.ino` | Funciones `iniciarPortal`, `enviarPortal`, `guardarRed`, `olvidarRed` |  |
| Otros tutoriales o proyectos | — | Ninguno |

## 3. Inteligencia artificial

### 3.1 Claude (Anthropic) en el chat de tutoría — 16 al 21 de noviembre de 2026

* **Modelo:** Claude Opus 5.5.
* **Para qué se usó:** como tutor paso a paso para montar la arquitectura de la guía, siguiendo las reglas del prompt de la guía (el grupo ejecuta y prueba; nunca se comparten claves).

| Qué generó o adaptó | Cómo se verificó o modificó |
| :--- | :--- |
| `server.js` adaptado: tabla `mediciones`, ruta `/api/mediciones`, `ALTURA_SENSOR_CM`, cálculo del nivel en el servidor, `bateria_v` opcional. También `package.json`, `.gitignore` y `.env.example` | El grupo los ejecutó. Hubo errores al nombrar los archivos (`package.jsn`, `server.jsn`) que se corrigieron. Las dependencias quedaron instaladas con `npm install` y `node --check server.js` no muestra errores. El servidor quedó publicado en Railway. |
| Un prompt para Claude Code que pedía construir ese servidor | COMPLETAR: si se usó y qué resultó |
| `prueba_sensor.ino` (primera versión: sensor sin WiFi, mediana de 5 lecturas) | El grupo lo modificó (ECHO al pin 4, 9600 baudios) |
| Versión modificada de `monitor_sumidero.ino` (muestra cada 2 s, explica por qué falla cada lectura, alerta si no hay WiFi en 20 s) | No se usó: el firmware final no incluye esos cambios |
| `prueba.ino` (diagnóstico del sensor sin WiFi) | Se cargó en la placa y se usó para diagnosticar el sensor |
| `prueba_vida.ino` (parpadeo del LED RGB del pin 48 y texto "vivo") | Se cargó en la placa. El LED parpadeó; el texto no llegó al monitor |
| Comandos de terminal (`git clone`, `Set-ExecutionPolicy -Scope CurrentUser RemoteSigned`, lectura del puerto COM4 en PowerShell) y diagnóstico de errores de instalación y de puerto | Se ejecutaron y se mostró la salida |

Corrección hecha por la propia IA: primero sugirió medir la batería de la powerbank con un divisor de voltaje; después corrigió que la powerbank entrega 5 V regulados y ese voltaje no indica la carga. La medición de batería se descartó (`bateria_v = null`).

### 3.2 Claude Code — 22 de noviembre de 2026

* **Modelo:** Claude Opus 5.5, en la aplicación de escritorio de Claude (pestaña Code), con acceso al repositorio local Monitor-Sumidero.
* **Instalación:** con el comando de PowerShell indicado en https://code.claude.com/docs/en/setup.
* **Para qué se usó:** revisar el código real, corregir errores, ejecutar pruebas del servidor y completar la documentación con datos verificados.

| Qué hizo | Cómo se verificó |
| :--- | :--- |
| Corrigió `SERVER_URL` en `monitor_sumidero.ino` para enviar a `/api/mediciones` (antes enviaba a la raíz y recibía `404`) | Prueba con `curl`: la raíz responde `404` y la ruta correcta responde `401` sin clave |
| Quitó de `server.js` el registro temporal de diagnóstico de la API key y agregó el rechazo de todas las solicitudes cuando `API_KEY` no está configurada | `node --check server.js` y pruebas locales (Testing, Prueba 2.2) |
| Movió las credenciales del firmware (red WiFi, contraseña y API key) a `secrets.h`, excluido de git, y creó la plantilla `secrets.h.example` | Se comprobó que ningún dato real quedara en los archivos publicados |
| Agregó `.gitattributes` y subió el modelo, los renders y los planos del diseño 3D a `diseno-3d/` | Se comprobó que los archivos subidos son idénticos a los originales y que los enlaces abren en GitHub |
| Ejecutó las pruebas de seguridad y validación de la API (Testing, Prueba 2) | Resultados registrados en Testing |
| Calculó el promedio, el error y la desviación estándar de la Prueba 1 | Las mediciones las tomó el grupo con el prototipo; la IA solo hizo los cálculos |
| Completó las secciones de `software/`, `testing/`, `diseno-3d/`, `iteraciones/` y este documento | Los datos se tomaron del código, del historial de git, de los planos y de las versiones instaladas. El grupo revisó el texto antes de publicarlo. |

El firmware modificado no se compiló con la IA (no había compilador de línea de comandos); debe cargarse y comprobarse desde Arduino IDE.

### 3.3 Claude (Anthropic) en claude.ai — 21 de noviembre de 2026

* **Modelo:** Claude Sonnet 5.5.
* **Para qué se usó:** leer el repositorio y el informe de recopilación para redactar **borradores** de documentación: `README.md`, `FUENTES.md`, las plantillas de las carpetas `hardware/`, `software/`, `diseno-3d/`, `testing/`, `iteraciones/`, `documentacion/` y un esquemático en borrador.
* **Cómo se verificó:** los borradores de `software/`, `testing/`, `diseno-3d/`, `iteraciones/` y `FUENTES.md` se contrastaron después con el código, los planos y las pruebas, usando Claude Code (ver 3.2). `README.md`, `hardware/` y `documentacion/`: COMPLETAR.

### 3.4 Código final del repositorio

| Archivo | ¿Se usó IA? | Herramienta, para qué y cómo se verificó |
| :--- | :--- | :--- |
| `monitor_sumidero.ino` | Sí, en parte | Claude Code corrigió la URL del servidor y separó las credenciales en `secrets.h` (3.2). |
| `secrets.h.example`, `.gitattributes` | Sí | Creados por Claude Code (3.2) |
| `server.js`, `package.json`, `.gitignore`, `.env.example` | Sí | Adaptados de la guía con Claude en el chat de tutoría (3.1); Claude Code ajustó la validación de la API key (3.2). Verificados con pruebas locales y en Railway. |
| `public/index.html` | No | Basado en la guía del curso. |
| `versiones-anteriores/` (`sumidero_wifi.ino`, `index.html`, `pagina.h`, `generar_pagina.sh`) | No |  |
| Documentación (README de cada carpeta y `FUENTES.md`) | Sí | Borradores con Claude en claude.ai revisados por el grupo y corregidos |
| Diseño 3D (modelo, renders, planos) | Si parcialmente para generar precisamente los planos | Parcialmente para generar precisamente los planos |
| Informes y presentación | COMPLETAR | COMPLETAR |

### 3.5 Otras herramientas de IA

Claude.
