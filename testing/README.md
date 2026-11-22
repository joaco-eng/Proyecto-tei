# Testing

> Esta carpeta documenta las pruebas del sistema de monitoreo de sumideros: la precisión del sensor, medida con el prototipo, y el funcionamiento de la API del servidor, probada el 22-11-2026.

## Protocolo general

| Aspecto | Detalle |
| :--- | :--- |
| Prototipo probado | ESP32-S3 Super Mini + sensor AJ-SR04M, versión con envío por WiFi a Railway. COMPLETAR: iteración del prototipo. |
| Firmware | [`esp32/monitor_sumidero/monitor_sumidero.ino`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/esp32/monitor_sumidero/monitor_sumidero.ino), commit [`b5022ec`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/commit/b5022ec) |
| Servidor | [`server.js`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/server.js), commit [`b5022ec`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/commit/b5022ec), en Railway: https://monitor-sumidero-production.up.railway.app/ |
| Alimentación | Powerbank por USB |
| Instrumentos | Huincha o regla (COMPLETAR: precisión), Monitor Serial a 115200 baudios, `curl` para las pruebas de la API |
| Condiciones | COMPLETAR (lugar, temperatura, tipo de superficie reflectante, agua real o placa) |
| Repeticiones | 5 mediciones por punto. Cada medición del firmware ya es la mediana de 5 lecturas del sensor. |

## Prueba 1 — Precisión del sensor

**Objetivo:** comparar la distancia real con la medida por el sensor.
**Procedimiento:** se ubicó una superficie plana a distancias conocidas frente al sensor y se registraron 5 mediciones por punto en el Monitor Serial (`Distancia: X cm`).

### 1.1 Mediciones registradas

| Distancia real (cm) | Medición 1 | Medición 2 | Medición 3 | Medición 4 | Medición 5 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 30 | 31,7 | 32,0 | 30,0 | 30,0 | 29,8 |
| 60 | 60,0 | 60,0 | 62,0 | 60,0 | 61,0 |
| 100 | 100,2 | 99,8 | 100,0 | 100,0 | 100,0 |

### 1.2 Resultados

| Distancia real (cm) | Medida promedio (cm) | Error (cm) | Error relativo | Variación entre lecturas, máx − mín (cm) | Desviación estándar (cm) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 30 | 30,70 | +0,70 | 2,3 % | 2,2 | 1,06 |
| 60 | 60,60 | +0,60 | 1,0 % | 2,0 | 0,89 |
| 100 | 100,00 | 0,00 | 0,0 % | 0,4 | 0,14 |

*Error = promedio − distancia real. Error relativo = |error| / distancia real. Desviación estándar muestral (n = 5).*

**Nota:** no se midieron puntos bajo 20 cm. El firmware descarta toda lectura menor a 20 cm (`DIST_MIN_CM = 20.0`), porque el AJ-SR04M tiene una zona ciega de unos 20–25 cm según su fabricante. Por eso el sensor debe instalarse a más de 20 cm del nivel máximo de agua que se quiera detectar.

**Conclusión:** en el rango probado (30 a 100 cm) el error promedio fue menor a 1 cm y el error relativo no superó el 2,3 %, lo que es suficiente para monitorear el nivel de un sumidero. A 100 cm la medición fue prácticamente exacta (error 0,0 cm y variación de solo 0,4 cm). A corta distancia el sensor mide levemente de más (+0,7 cm a 30 cm) y con algo más de dispersión, lo que es coherente con la cercanía a su zona ciega. Se recomienda instalar el sensor de modo que el agua quede normalmente a más de 30 cm de él.

## Prueba 2 — Seguridad y validación de la API

**Objetivo:** verificar que el servidor esté disponible, que solo acepte mediciones de dispositivos autorizados y que rechace datos con formato incorrecto.

**Alcance:** esta prueba evalúa el servidor de forma independiente de la placa. El envío desde la ESP32 hasta la base de datos no forma parte de esta prueba.

### 2.1 Pruebas del servidor en Railway (22-11-2026)

Ejecutadas con `curl` contra `https://monitor-sumidero-production.up.railway.app`.

| Verificación | Solicitud | Resultado esperado | Resultado obtenido | Estado |
| :--- | :--- | :--- | :--- | :--- |
| Dashboard disponible | `GET /` | `200` | `200` (0,49 s) | ✅ Aprobada |
| POST sin clave | `POST /api/mediciones` sin `x-api-key` | `401` | `401` `{"error":"API key invalida"}` | ✅ Aprobada |
| POST con clave incorrecta | `POST /api/mediciones` con `x-api-key: clave-incorrecta` | `401` | `401` `{"error":"API key invalida"}` | ✅ Aprobada |
| Ruta inexistente | `POST /` | `404` | `404` | ✅ Aprobada |

### 2.2 Validación de datos (servidor ejecutado localmente)

Se ejecutó `server.js` en un computador local con una clave de prueba, para comprobar la validación sin modificar los datos reales.

| Caso | Cuerpo enviado | Resultado esperado | Resultado obtenido | Estado |
| :--- | :--- | :--- | :--- | :--- |
| Falta `dispositivo` | `{"distancia_cm":50}` | `400` | `400` `Falta 'dispositivo' o 'distancia_cm' no es numero` | ✅ Aprobada |
| `distancia_cm` como texto | `{"dispositivo":"p","distancia_cm":"50"}` | `400` | `400` `Falta 'dispositivo' o 'distancia_cm' no es numero` | ✅ Aprobada |
| `bateria_v` como texto | `{"dispositivo":"p","distancia_cm":50,"bateria_v":"x"}` | `400` | `400` `'bateria_v' debe ser numero o null` | ✅ Aprobada |
| Error interno sin exponer detalles | `{"dispositivo":"p","distancia_cm":50,"bateria_v":null}` sin base de datos conectada | `500` con mensaje genérico | `500` `{"error":"Error al guardar"}` (el detalle queda solo en el log) | ✅ Aprobada |
| Clave incorrecta | `x-api-key: otra` | `401` | `401` | ✅ Aprobada |
| Variable `API_KEY` vacía, POST sin clave | sin `x-api-key` | `401` | `401` | ✅ Aprobada |

**Conclusión:** las 10 verificaciones fueron aprobadas. El servidor está publicado y responde. Solo acepta mediciones que traen la clave correcta, también cuando la variable `API_KEY` no está configurada. Rechaza con `400` los datos con formato incorrecto y no muestra detalles internos cuando ocurre un error. Esto protege la base de datos de envíos no autorizados o mal formados.
