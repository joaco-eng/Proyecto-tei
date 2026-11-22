# Iteraciones — Evolución del proyecto

Organiza cada versión en su propia carpeta (`iteracion-1/`, `iteracion-2/`, ...) con fotos, código y notas. Todo lo marcado como `COMPLETAR` falta en el repositorio y en el chat de tutoría.

> No inventes iteraciones: la historia del repositorio tiene un solo commit, así que las versiones del prototipo físico hay que reconstruirlas con fotos y archivos del grupo.

## Comparación de versiones

| | Iteración 1 | Iteración 2 | Iteración 3 (final) |
| :--- | :--- | :--- | :--- |
| Descripción y fecha | COMPLETAR | COMPLETAR | COMPLETAR |
| Componentes | COMPLETAR | COMPLETAR | ESP32-S3 Super Mini, AJ-SR04M, divisor de voltaje, LED + 220 Ω, powerbank (confirmar) |
| Cómo funcionaba | COMPLETAR | COMPLETAR | Ver [README](../README.md#2-cómo-funciona-el-sistema) |
| Fotografías | COMPLETAR | COMPLETAR | COMPLETAR |
| Problemas encontrados | COMPLETAR | COMPLETAR | COMPLETAR |
| Cambios realizados | COMPLETAR | COMPLETAR | COMPLETAR |
| Justificación y mejoras observadas | COMPLETAR | COMPLETAR | COMPLETAR |

## Evolución del software (reconstruida del chat de tutoría)

| Versión | Archivo | Características | ¿Se usó? |
| :--- | :--- | :--- | :--- |
| Base de la guía | `esp32_railway.ino` y `server.js` | Tabla `lecturas`, ruta `/api/lecturas`, envío cada 10 s | Solo como referencia |
| S1 | `server.js`, `package.json`, `.gitignore`, `.env.example` adaptados | Tabla `mediciones`, ruta `/api/mediciones`, nivel calculado en el servidor | funcionamiento Sí |
| S2 | `prueba_sensor.ino`  | Sensor sin WiFi, TRIG 5 / ECHO 4, 9600 baudios, mediana de 5 lecturas | Sí: para demostrar funcionalidad de un primer prototipo |
| S3 | `monitor_sumidero.ino` | WiFi, HTTPS con `setInsecure()`, POST a `/api/mediciones` con `x-api-key`, rango válido 20–600 cm, envío cada 10 s | Sí: Funciona correctamente |
| S4 | `prueba.ino` | Diagnóstico del sensor sin WiFi | Sí |
| S5 | `prueba_vida.ino` | LED RGB del pin 48 y texto "vivo" | Sí: LED OK, monitor sin texto |
| Anterior a la final | `Avance2_oct12a.ino` | Lógica de estados con umbral, confirmación  | Sí, para Avance 2 del curso se usó esta version del codigo |
| **Final** | `sumidero_wifi/sumidero_wifi.ino` + `index.html` | Lógica de estados (≤ 8 cm durante 5 s), servidor web en la ESP32, portal `Sumidero-Setup`, Web Serial, `/estado` | Es la versión de este repositorio |

## Decisiones de diseño documentadas

| Tema | Opción descartada | Opción final | Justificación |
| :--- | :--- | :--- | :--- |
| Estructura de la tabla (versión Railway) | Una fila por dato (sensor, valor) | Una fila por medición (id, dispositivo, distancia_cm, nivel_cm, bateria_v, creado_en) | Más ordenada |
| Dónde calcular el nivel | En la ESP32 | En el servidor (`ALTURA_SENSOR_CM`) | Si cambia la altura, no hay que reprogramar la ESP32 |
| Batería | Medirla con un divisor en un pin analógico | No medirla | La powerbank entrega 5 V regulados, ese voltaje no indica la carga |
| Pin ECHO | GPIO 6 | GPIO 4 | Comodidad de conectividad en la placa |
| Velocidad serie | 115200 | 9600 | Dentro de Arduino IDE no se presentaba la opcion de 115200 baudios |
| Repositorio | Fork del ejemplo de la guía | Repositorio propio | Se creó desde cero |
| Arquitectura | Servidor en la nube (Railway + Aiven) | Servidor en la nube (Railway + Aiven) | Es la opcion estandar entregada en el curso para este proceso |
