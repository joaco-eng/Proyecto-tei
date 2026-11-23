# Iteraciones — Evolución del proyecto

Cada versión se organiza en su propia carpeta (`iteracion-1/`, `iteracion-2/`, `iteracion-3/`) con fotos, código y notas. Lo marcado como `COMPLETAR` corresponde a fotografías del prototipo, que deben agregarse desde los archivos del grupo.

> Las iteraciones se reconstruyeron a partir de los programas guardados por el grupo (las fechas indicadas corresponden a la última modificación de cada archivo) y del historial del repositorio [Monitor-Sumidero](https://github.com/JoaquinZavalaM/Monitor-Sumidero/commits/main), que registra 5 commits entre el 16 y el 22 de noviembre de 2026.

## Comparación de versiones

| | Iteración 1 | Iteración 2 | Iteración 3 (final) |
| :--- | :--- | :--- | :--- |
| Descripción y fecha | Medidor de distancia básico, sin WiFi (31-08-2026). | Detector de nivel anormal con alerta local por LED, presentado en el Avance 2 del curso (12-10-2026). | Monitor conectado a Internet: envía las mediciones a un servidor en Railway con base de datos en Aiven y las muestra en un dashboard web (16 al 22-11-2026). |
| Componentes | Sensor ultrasónico HC-SR04 (TRIG GPIO 14, ECHO GPIO 12). Placa: COMPLETAR. | ESP32-S3 Super Mini, sensor ultrasónico (TRIG GPIO 5, ECHO GPIO 4), LED con resistencia de 220 Ω en GPIO 6. | ESP32-S3 Super Mini, sensor impermeable AJ-SR04M (TRIG GPIO 5, ECHO GPIO 4), divisor de voltaje en ECHO, powerbank por USB, encapsulado de PLA impreso en 3D. |
| Cómo funcionaba | Medía la distancia cada 200 ms y la mostraba en el Monitor Serial (115200 baudios). No detectaba eventos ni se conectaba a la red. | Medía cada 500 ms. Si la distancia era ≤ 8 cm, verificaba que se mantuviera durante 5 s antes de confirmar la alerta y encender el LED. Informaba el estado por el Monitor Serial (9600 baudios). | Cada 10 s toma 5 lecturas, calcula la mediana, descarta valores fuera de 20–600 cm y envía la distancia por HTTPS al servidor. El servidor calcula el nivel de agua, lo guarda en PostgreSQL y el dashboard lo grafica. Ver [Software](../software/README.md#2-funcionamiento-del-firmware-de-la-esp32). |
|  Renders del encapsulado en [Diseño 3D](../diseno-3d/README.md). |
| Problemas encontrados | El HC-SR04 no es impermeable, por lo que no sirve dentro de un sumidero. Solo se podía ver la medición con el computador conectado. | La alerta solo se veía en el lugar (LED) y no quedaba registro de las mediciones. El umbral de 8 cm queda dentro de la zona ciega (~20 cm) del sensor impermeable AJ-SR04M. | El sensor no respondía en algunas pruebas (sin eco). El texto no llegaba al Monitor Serial por USB. El POST se enviaba a la raíz del servidor (404) en vez de a `/api/mediciones`. |
| Cambios realizados | Se reemplazó el HC-SR04 por el AJ-SR04M impermeable y se pasó a la ESP32-S3 Super Mini con pines TRIG 5 / ECHO 4. Se agregó la lógica de estados con umbral y tiempo de confirmación. | Se agregó conexión WiFi y envío HTTPS a Railway, base de datos PostgreSQL y dashboard web. Se agregó el filtrado por mediana y el rango válido de 20–600 cm. El nivel pasó a calcularse en el servidor. | Se crearon programas de diagnóstico (`prueba.ino`, `vivo.ino`). Se corrigió la URL del servidor, se movieron las credenciales a `secrets.h` y se reforzó la validación de la clave de API. |
| Justificación y mejoras observadas | El sensor impermeable permite instalar el equipo dentro del sumidero. La confirmación de 5 s evita alertas por lecturas aisladas. | El monitoreo remoto permite revisar el sumidero desde cualquier lugar y conservar un historial para analizar la variación del nivel. | La versión final mide con error menor a 1 cm entre 30 y 100 cm y su API rechaza envíos no autorizados o mal formados (ver [Testing](../testing/README.md)). |

## Evolución del software

| Versión | Archivo | Fecha | Características | ¿Se usó? |
| :--- | :--- | :--- | :--- | :--- |
| S0 | `medidor_distancia_simple_.ino` | 31-08-2026 | HC-SR04 en GPIO 14 / 12, medición cada 200 ms, 115200 baudios, sin WiFi | Sí: primer prototipo (Iteración 1) |
| S1 | `Avance2_oct12a.ino` | 12-10-2026 | TRIG 5 / ECHO 4, LED en GPIO 6, umbral ≤ 8 cm con confirmación de 5 s, medición cada 500 ms, 9600 baudios | Sí: versión presentada en el Avance 2 del curso (Iteración 2) |
| Base de la guía | `esp32_railway.ino` y `server.js` | — | Tabla `lecturas`, ruta `/api/lecturas`, envío cada 10 s | Solo como referencia |
| S2 | `server.js`, `package.json`, `.gitignore`, `.env.example` adaptados | 16-11-2026 | Tabla `mediciones`, ruta `/api/mediciones`, nivel calculado en el servidor, dashboard en `public/index.html` | Sí: servidor publicado en Railway |
| S3 | `proyecto1.1.ino` | 16-11-2026 | Primera versión con WiFi y HTTPS (`setInsecure()`), POST con `x-api-key`, mediana de 5 lecturas, rango 20–600 cm, soporte para red sin contraseña | Sí: base del firmware final |
| S4 | `prueba.ino` (`prueba_sensor.ino`) | 20-11-2026 | Diagnóstico del sensor sin WiFi, TRIG 5 / ECHO 4, 9600 baudios | Sí: para verificar el sensor |
| S5 | `vivo.ino` (`prueba_vida.ino`) | 22-11-2026 | Prueba de vida de la placa: LED RGB del pin 48 y texto "vivo" escrito directo al USB interno (`HWCDCSerial`) | Sí: LED OK |
| **Final** | [`monitor_sumidero.ino`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/esp32/monitor_sumidero/monitor_sumidero.ino) + [`server.js`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/server.js) | 22-11-2026 | WiFi, HTTPS, POST a `/api/mediciones` con `x-api-key`, credenciales en `secrets.h`, mediana de 5 lecturas, rango 20–600 cm, envío cada 10 s, 115200 baudios | Sí: versión del repositorio (Iteración 3) |

## Decisiones de diseño documentadas

| Tema | Opción descartada | Opción final | Justificación |
| :--- | :--- | :--- | :--- |
| Sensor | HC-SR04 | AJ-SR04M impermeable | El equipo se instala dentro del sumidero, expuesto al agua |
| Estructura de la tabla | Una fila por dato (sensor, valor) | Una fila por medición (id, dispositivo, distancia_cm, nivel_cm, bateria_v, creado_en) | Más ordenada y fácil de consultar |
| Dónde calcular el nivel | En la ESP32 | En el servidor (`ALTURA_SENSOR_CM`) | Si cambia la altura de instalación, no hay que reprogramar la ESP32 |
| Filtrado de lecturas | Una lectura por medición | Mediana de 5 lecturas, mínimo 3 válidas | Reduce el efecto de ecos falsos |
| Batería | Medirla con un divisor en un pin analógico | No medirla | El powerbank entrega 5 V regulados; ese voltaje no indica la carga |
| Pin ECHO | GPIO 6 | GPIO 4 | Comodidad de conexión en la placa; GPIO 6 quedó para el LED en la Iteración 2 |
| Velocidad serie | 9600 baudios (Iteraciones 2 y diagnóstico) | 115200 baudios | Valor usado en el firmware final para mostrar las respuestas del servidor |
| Credenciales del firmware | Escritas dentro del `.ino` | Archivo `secrets.h` excluido de git | Permite publicar el código sin exponer la red WiFi ni la clave de API |
| Repositorio | Fork del ejemplo de la guía | Repositorio propio | Se creó desde cero |
| Arquitectura | Alerta solo local (LED en el sumidero) | Servidor en la nube (Railway + Aiven) con dashboard web | Permite monitorear a distancia y guardar el historial; es la arquitectura propuesta en el curso |
