# Software

Aquí está la documentación técnica de todo el código. El código mismo vive en la raíz del repositorio para no romper el script `generar_pagina.sh`.

> Todo lo marcado como `COMPLETAR` falta en el repositorio y en el chat de tutoría.

## Archivos

| Archivo | Descripción |
| :--- | :--- |
| [`../sumidero_wifi/sumidero_wifi.ino`](../sumidero_wifi/sumidero_wifi.ino) | **Firmware de la ESP32.** Mide con el sensor, aplica la lógica de estados, controla el LED, sirve la página y el JSON `/estado`, y gestiona el WiFi (portal de configuración). |
| [`../sumidero_wifi/pagina.h`](../sumidero_wifi/pagina.h) | Página web incrustada en el firmware (archivo **generado**, no editar a mano). |
| [`../index.html`](../index.html) | **Página web del monitor** (dashboard): estado, distancia, historial de eventos, banner de alerta, sonido y notificación. |
| [`../generar_pagina.sh`](../generar_pagina.sh) | Regenera `pagina.h` desde `index.html`. Ejecutar siempre tras editar la página. |

## Cómo funciona el firmware

| Parte | Qué hace |
| :--- | :--- |
| `medirDistancia()` | Pulso de 10 µs en TRIG y lectura del eco con `pulseIn` (tiempo máximo 30 ms). Convierte a cm con la velocidad del sonido (0.0343 cm/µs) dividida por 2. Devuelve -1 si no hay eco. |
| `loop()` | Atiende al servidor web y al WiFi, y cada 500 ms mide y actualiza el estado (NORMAL, VERIFICANDO o ALERTA). Una lectura sin eco **no** cambia el estado y se informa por el Monitor Serial. |
| `iniciarWifi()` / `iniciarPortal()` | Si hay una red guardada intenta conectarse durante 15 s; si no, crea la red `Sumidero-Setup` con portal cautivo (DNS + formulario). |
| `gestionarWifi()` | Reintenta la conexión cada 10 s si se pierde. |
| Rutas HTTP | `/` (página), `/estado` (JSON), `/guardar` (POST, guarda la red), `/olvidar` (borra la red y reinicia). |

La red WiFi se guarda en la memoria de la placa (`Preferences`), por eso **no hay claves en el código**.

## Cómo funciona la página

* **Modo USB:** Web Serial a 9600 baudios. Interpreta el texto que imprime el firmware (`ESTADO: ...`, `Distancia al agua: ...`).
* **Modo WiFi:** consulta `GET /estado` cada 1 s. Si la página está en https (GitHub Pages), el botón WiFi redirige a `http://<IP de la placa>/`, donde la placa sirve esta misma página.
* **Alerta:** banner, sonido (Web Audio) y notificación del navegador. Botón **Silenciar** y botón **Probar alerta**.
* **Historial:** lista de eventos en pantalla. **No se guarda**: se pierde al recargar la página.

## Servidor

El "servidor" de este proyecto es el `WebServer` que corre en la propia ESP32. No hay un backend aparte en este repositorio.

COMPLETAR: la versión con servidor en la nube (Node.js/Express en Railway + PostgreSQL en Aiven) se trabajó en el chat de tutoría. Decidir si se incluye en la entrega (en una carpeta `servidor-railway/`, sin claves) o si se descartó, y dejarlo escrito en [iteraciones](../iteraciones/README.md).

## Dependencias

| Dependencia | Versión | Origen |
| :--- | :--- | :--- |
| ESP32 Arduino core (paquete `esp32`) | 4.0.0 (según el log de carga; confirmar) | Gestor de placas del Arduino IDE (Espressif) |
| `WiFi`, `WebServer`, `DNSServer`, `Preferences` | Las del core | Incluidas en el core, no se instalan aparte |
| Navegador | Chrome o Edge de escritorio (para Web Serial) | — |
| Librerías externas | Ninguna | — |

Detalle de cada una: [FUENTES.md](../FUENTES.md).

## Instrucciones de configuración

1. **Placa:** conecta la ESP32-S3 al computador por USB y carga `sumidero_wifi.ino` con la configuración de la tabla del [README principal](../README.md#52-cargar-el-código-en-la-esp32). Es importante dejar **USB CDC On Boot = Enabled**; sin eso el texto del programa puede no llegar al Monitor Serial.
2. **Servidor (WiFi):** al primer arranque la placa abre la red `Sumidero-Setup`; configura tu WiFi desde ahí. La IP queda en el Monitor Serial.
3. **Dashboard:** abre `http://<IP>` (WiFi) o https://joaco-eng.github.io/Proyecto-tei/ (USB).
4. **Cambiar la página:** edita `index.html`, ejecuta `./generar_pagina.sh` y vuelve a cargar el firmware.

## Seguridad

* No subas a GitHub archivos con claves reales (WiFi, `API_KEY`, `DATABASE_URL`). El archivo `monitor_sumidero.ino` de la versión Railway las contiene.
* Si una clave llegó a subirse, hay que cambiarla: borrar el archivo no la elimina del historial de Git.
