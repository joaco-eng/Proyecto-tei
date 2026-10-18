# Sistema Detector de Nivel Anormal en Sumidero

Sistema embebido (ESP32) de monitoreo de nivel de agua y prevención de desbordes/obstrucciones, con una página web que replica la alerta del LED.

Página para conexión por USB: https://joaco-eng.github.io/Proyecto-tei/

---

## 1. Asignación de Pines (Pinout)

| Señal | Pin / GPIO | Detalle de Conexión |
| :--- | :--- | :--- |
| **TRIG** | Pin 5 | 1.er pin lado derecho (visto desde el conector USB) |
| **ECHO** | Pin 4 | 4.º pin lado izquierdo (visto desde el conector USB) |
| **LED** | Pin 6 | 2.º pin lado derecho (en serie con resistencia de 220 Ω) |

---

## 2. Parámetros de Operación

* **Umbral de alerta:** distancia ≤ 8.0 cm entre el sensor ultrasónico y la superficie del agua.
* **Confirmación:** 5000 ms (5 s) continuos en nivel crítico para declarar alerta.
* **Frecuencia de muestreo:** una lectura cada 500 ms con `millis()`.
* **Puerto serie:** 9600 baudios.

---

## 3. Lógica de Estados

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

---

## 4. Archivos

| Archivo | Descripción |
| :--- | :--- |
| `sumidero_wifi/sumidero_wifi.ino` | Firmware: el código `Avance2_oct12a` más WiFi y servidor web. |
| `sumidero_wifi/pagina.h` | La página web incrustada en el firmware (generada). |
| `index.html` | Página web del monitor (alerta con banner, sonido y notificación). |
| `generar_pagina.sh` | Regenera `pagina.h` desde `index.html`. Ejecutar tras editar la página. |

---

## 5. Uso

### Por USB
1. Graba `sumidero_wifi.ino` (o el código original).
2. Abre la página en Chrome/Edge, pulsa **Conectar placa** y elige el puerto. Cierra antes el Monitor Serial del IDE.

### Por WiFi
1. En `sumidero_wifi.ino` cambia `WIFI_SSID` y `WIFI_PASS` (red de 2.4 GHz).
2. Graba la placa y abre el Monitor Serial a 9600.
3. Espera `WiFi conectado. Abre http://<IP>`.
4. Abre esa dirección `http://<IP>` desde un equipo en la misma red. La placa sirve la página y se conecta sola.

El monitoreo, el LED y el Monitor Serial funcionan aunque no haya WiFi.

---

## 6. Interfaz de red

* `GET /` → página web.
* `GET /estado` → `{"distancia":7.5,"estado":"ALERTA"}` (estados: `NORMAL`, `VERIFICANDO`, `ALERTA`).
