# Testing

> Esta carpeta organiza las pruebas. Las tablas de datos de las pruebas 1 a 3 **están vacías**: ninguna de esas mediciones aparece en el chat de tutoría, hay que hacerlas con el prototipo. Todo lo marcado como `COMPLETAR` falta.

## Protocolo general

| Aspecto | Detalle |
| :--- | :--- |
| Prototipo probado | COMPLETAR (versión / iteración) |
| Firmware | `sumidero_wifi/sumidero_wifi.ino`, commit COMPLETAR |
| Alimentación | Powerbank por USB |
| Instrumentos | Huincha o regla (COMPLETAR: precisión), cronómetro, Monitor Serial a 9600 baudios |
| Condiciones | COMPLETAR (lugar, temperatura, tipo de superficie reflectante, agua real o placa) |
| Repeticiones | COMPLETAR (sugerido: 10 lecturas por punto) |

## Prueba 1 — Precisión del sensor

**Objetivo:** comparar la distancia real con la medida por el sensor.
**Procedimiento:** poner una superficie plana a distancias conocidas frente al sensor, anotar las lecturas del Monitor Serial y calcular el error.

| Distancia real (cm) | Medida promedio (cm) | Error (cm) | Variación entre lecturas (cm) |
| :--- | :--- | :--- | :--- |
| 5 | COMPLETAR | COMPLETAR | COMPLETAR |
| 8 | COMPLETAR | COMPLETAR | COMPLETAR |
| 15 | COMPLETAR | COMPLETAR | COMPLETAR |
| 30 | COMPLETAR | COMPLETAR | COMPLETAR |
| 60 | COMPLETAR | COMPLETAR | COMPLETAR |
| 100 | COMPLETAR | COMPLETAR | COMPLETAR |

**Importante:** el umbral de alerta es ≤ 8 cm y en el chat se indicó que el AJ-SR04M tiene una zona ciega de unos 20–25 cm (dato sin verificar). Incluye puntos por debajo de 20 cm para comprobar si el sensor mide a 8 cm. Si no mide, hay que rediseñar el montaje o el umbral, y eso es un hallazgo del informe.

**Conclusión:** COMPLETAR.

## Prueba 2 — Detección de niveles elevados

**Objetivo:** comprobar que, cuando el agua supera el umbral, se declara la alerta y la página la muestra.
**Comportamiento esperado según el código:** a ≤ 8.0 cm el estado pasa a VERIFICANDO y a los 5 s seguidos pasa a ALERTA (LED encendido, banner, sonido y notificación en la página). Si el agua baja antes de 5 s, vuelve a NORMAL sin alerta.

| Ensayo | Distancia mantenida | Tiempo hasta ALERTA (s) | LED | Banner y sonido en la web | Resultado |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | COMPLETAR | COMPLETAR | COMPLETAR | COMPLETAR | COMPLETAR |
| 2 (se retira antes de 5 s) | COMPLETAR | No debe haber alerta | COMPLETAR | COMPLETAR | COMPLETAR |
| 3 | COMPLETAR | COMPLETAR | COMPLETAR | COMPLETAR | COMPLETAR |

**Conclusión:** COMPLETAR.

## Prueba 3 — Transmisión y almacenamiento

**Objetivo:** verificar que las mediciones lleguen a la página y que el historial quede guardado.

| Verificación | Cómo se comprueba | Resultado |
| :--- | :--- | :--- |
| Llegada por USB (Web Serial) | La página muestra la distancia y el estado | COMPLETAR |
| Llegada por WiFi | `GET /estado` devuelve JSON válido y la página se actualiza cada 1 s | COMPLETAR |
| Pérdida de WiFi | Apagar el router y comprobar que la placa reintenta y el LED sigue funcionando | COMPLETAR |
| Historial guardado | COMPLETAR (ver nota) | COMPLETAR |

**Nota:** en la versión de este repositorio el historial es una lista en la página y **se pierde al recargarla**. Si la pauta exige que quede guardado, hay que (a) guardarlo en la página (por ejemplo con `localStorage`), o (b) usar la versión con base de datos (Railway + Aiven), cuyas pruebas (`/api/salud`, respuesta `201` del POST, filas en la base) tampoco constan en el chat.

## Registro de fallas encontradas

Fallas documentadas en el chat de tutoría, 16 al 21 de noviembre de 2026.

| Falla | Causa | Solución | Estado |
| :--- | :--- | :--- | :--- |
| A. El sensor no responde: 5 lecturas vacías y el mensaje "el sensor no responde" (ECHO nunca subió en 40 ms) | COMPLETAR | COMPLETAR | Abierta en el chat |
| B. El texto del programa no llega al Monitor Serial (solo aparece la línea del ROM del chip), aunque el programa corre (el LED parpadea) | COMPLETAR (sospecha: configuración USB / CDC) | COMPLETAR | Abierta en el chat |
| C. Archivos mal nombrados (`package.jsn`, `server.jsn`) | Error al escribir los nombres | Renombrar a `package.json` y `server.js` | Corrección indicada |
| D. `npm` bloqueado por la política de ejecución de PowerShell | Windows bloquea scripts `.ps1` por defecto | `Set-ExecutionPolicy -Scope CurrentUser RemoteSigned` | Resuelta |
| E. `git clone` ejecutado dentro de `C:\Windows\System32` | Terminal abierta en la carpeta equivocada | Clonar dentro de Documentos | Resuelta |

### Análisis de la falla A (sensor sin respuesta)

* **Qué ocurrió:** con el código de diagnóstico, las 5 lecturas salieron vacías y se informó que el sensor no respondía.
* **Pruebas propuestas en el chat (sin resultados registrados):** escuchar el "tic-tic" del transductor al disparar, medir 5 V con multímetro, revisar el conector del cable de la sonda, revisar la resistencia R19 del módulo (debe estar vacía para el modo TRIG/ECHO), revisar los pines 5 y 4 y el divisor de voltaje.
* **Causa encontrada y solución:** COMPLETAR. Anotar también qué cambió la única vez que el monitor sí mostró datos.
* **Relación con el problema inicial:** sin eco no hay medición y el sistema no puede detectar un nivel anormal. El firmware ya evita confundir "sin eco" con "nivel normal": no cambia de estado e informa el error.
* **Mejora aplicada:** COMPLETAR.

### Análisis de la falla B (texto no llega por USB)

* **Relación con el problema inicial:** afecta al modo Web Serial, porque la página lee ese mismo texto.
* **Hipótesis descartadas en el chat:** PSRAM mal configurada, tamaño de flash incorrecto, modo de flash.
* **Pendiente en el chat:** probar con DTR activado (`$p.DtrEnable = $true`) y alternativa de cambiar USB Mode a USB-OTG (TinyUSB). Resultado: COMPLETAR.
