# Software — Sistema de monitoreo de sumideros

Esta carpeta contiene la documentación técnica y los programas utilizados para el monitoreo del nivel de agua en sumideros urbanos.

El sistema utiliza una **ESP32-S3 Super Mini** conectada a un **sensor ultrasónico impermeable AJ-SR04M**, que permite medir la distancia entre el sensor y la superficie del agua.

Las mediciones son procesadas por la ESP32 y transmitidas mediante WiFi a un servidor externo alojado en **Railway**, donde pueden almacenarse y consultarse a través de una plataforma web.

El sistema permite monitorear las variaciones del nivel de agua y está pensado para generar alertas digitales ante condiciones anormalmente elevadas; esa parte aún no está programada (sección 3.6).

**Dashboard del proyecto:** https://monitor-sumidero-production.up.railway.app/

**Repositorio del código:** https://github.com/JoaquinZavalaM/Monitor-Sumidero

---

## 1. Archivos del software

| Archivo | Función |
| :--- | :--- |
| [`esp32/monitor_sumidero/monitor_sumidero.ino`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/esp32/monitor_sumidero/monitor_sumidero.ino) | Programa que controla la ESP32, realiza las mediciones del sensor y transmite los datos al servidor. |
| [`esp32/monitor_sumidero/secrets.h.example`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/esp32/monitor_sumidero/secrets.h.example) | Plantilla de las credenciales del firmware (`WIFI_SSID`, `WIFI_PASS`, `API_KEY`). Se copia como `secrets.h`, que está en `.gitignore` y no se publica. |
| [`server.js`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/server.js) | Servidor Node.js + Express que recibe las mediciones, calcula el nivel de agua, las guarda en PostgreSQL y las entrega al dashboard. |
| [`package.json`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/package.json) / [`package-lock.json`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/package-lock.json) | Dependencias del servidor. Proyecto `monitor-sumidero` v1.0.0, Node.js ≥ 18. Comando de inicio: `npm start` (`node server.js`). |
| [`public/index.html`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/public/index.html) | Página web (dashboard) con el gráfico y la tabla de mediciones. La sirve el mismo servidor Express. |
| [`.env.example`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/.env.example) | Plantilla de las variables privadas del servidor (`DATABASE_URL`, `DB_CA_CERT`, `API_KEY`, `ALTURA_SENSOR_CM`) con valores de ejemplo. El `.env` real está en `.gitignore` y no se publica. |
| `README.md` | Documentación técnica, funcionamiento e instrucciones de instalación del software. |

---

## 2. Funcionamiento del firmware de la ESP32

El archivo `monitor_sumidero.ino` contiene el programa principal utilizado para controlar la ESP32-S3 Super Mini y obtener las mediciones del sensor impermeable AJ-SR04M.

### 2.1 Captura de mediciones

1. La ESP32 activa el sensor ultrasónico mediante el pin TRIG (GPIO 5), con un pulso de 20 µs.
2. El sensor emite una onda ultrasónica que rebota en la superficie del agua.
3. La onda regresa al sensor y su señal es recibida mediante ECHO (GPIO 4). El tiempo de espera máximo es de 40 ms, lo que equivale a unos 6,8 m.
4. A partir del tiempo de retorno y la velocidad del sonido (0,0343 cm/µs, dividido por 2 por la ida y la vuelta), la ESP32 calcula la distancia hasta la superficie del agua.
5. La distancia obtenida se expresa en centímetros.

### 2.2 Procesamiento de las mediciones

Para reducir la influencia de lecturas erróneas, el programa realiza **5 mediciones consecutivas**, separadas por intervalos de 60 milisegundos.

Posteriormente:

1. Comprueba que las mediciones se encuentren dentro del rango programado de 20 a 600 cm.
2. Descarta las lecturas fuera de ese rango o aquellas en las que no se detecta un eco válido.
3. Ordena las mediciones válidas de menor a mayor.
4. Calcula la mediana para obtener una distancia representativa.
5. Si se obtienen menos de 3 mediciones válidas, descarta el resultado y no lo transmite.

La utilización de la mediana permite reducir el efecto de valores aislados producidos por ecos incorrectos o variaciones momentáneas durante la medición.

### 2.3 Envío de datos

Cada **10 segundos**, la ESP32 obtiene una medición procesada y la transmite mediante WiFi a un servidor externo alojado en Railway. Para medir ese intervalo usa `millis()`, por lo que el programa no queda bloqueado con `delay`.

La comunicación se realiza mediante una solicitud HTTPS de tipo POST, enviando la información en formato JSON. La distancia se redondea a 1 decimal.

Si se pierde la conexión WiFi, el programa intenta restablecerla para continuar transmitiendo las mediciones.

El Monitor Serial permite observar las distancias medidas, los errores y las respuestas recibidas desde el servidor.

### 2.4 Parámetros de operación

| Parámetro | Valor |
| :--- | :--- |
| Microcontrolador | ESP32-S3 Super Mini |
| Sensor | AJ-SR04M ultrasónico impermeable |
| Pin TRIG | GPIO 5 |
| Pin ECHO | GPIO 4 (con divisor de voltaje, porque el sensor funciona a 5 V) |
| Lecturas por medición | 5 |
| Lecturas válidas necesarias | Mínimo 3 |
| Procesamiento | Cálculo de la mediana |
| Intervalo entre lecturas | 60 ms |
| Distancia mínima programada | 20 cm |
| Distancia máxima programada | 600 cm |
| Intervalo de envío | 10000 ms (10 segundos) |
| Monitor Serial | 115200 baudios |
| Conectividad | WiFi de 2.4 GHz |
| Protocolo de comunicación | HTTPS |
| Servidor | Railway |

---

## 3. Servidor, base de datos y comunicación

### 3.1 Arquitectura del sistema

El sistema utiliza una arquitectura de comunicación que permite enviar las mediciones del sensor a una plataforma digital.

El funcionamiento general es el siguiente:

```text
[ Sensor ultrasónico impermeable AJ-SR04M ]
                       |
                       v
               [ ESP32-S3 ]
                       |
             Procesa las mediciones
             y calcula la mediana
                       |
                       v
              [ Conexión WiFi ]
                       |
                       v
     [ Envío por HTTPS: POST /api/mediciones ]
                       |
                       v
     [ Servidor Railway (Node.js + Express) ]
          Calcula el nivel de agua
                       |
                       v
     [ Base de datos PostgreSQL en Aiven ]
                       |
                       v
     [ Consulta de registros: GET /api/mediciones ]
                       |
                       v
               [ Dashboard ]
                       |
         Gráfica y tabla de mediciones
```

La ESP32 no se conecta directamente a la base de datos. Su función es obtener las mediciones y enviarlas al servidor Railway.

El servidor recibe la información y se comunica con la base de datos para almacenar y consultar los registros.

**Tecnología del servidor:** Node.js (≥ 18) con Express 4, en el archivo [`server.js`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/server.js). El mismo servidor entrega la API y la página web (carpeta `public/`).

### 3.2 Comunicación con Railway

La ESP32 se conecta a Internet mediante WiFi y envía las mediciones al servidor utilizando solicitudes HTTPS de tipo POST.

**Dirección del servidor:**

https://monitor-sumidero-production.up.railway.app/

**Ruta de recepción de mediciones:** `POST /api/mediciones`, es decir:
`https://monitor-sumidero-production.up.railway.app/api/mediciones`

La ruta exige el encabezado `x-api-key` y el cuerpo en JSON (`Content-Type: application/json`).

Respuestas del servidor:

- `201`: medición guardada. Devuelve `{"ok": true, "id": ..., "nivel_cm": ...}`.
- `400`: datos inválidos.
- `401`: clave incorrecta.
- `500`: error del servidor o de la base de datos.

Para autorizar la comunicación, el programa utiliza una clave de API que se envía mediante el encabezado `x-api-key`. El servidor la compara con la variable de entorno `API_KEY`.

Esta clave permite que el servidor identifique las solicitudes autorizadas y evite recibir información de dispositivos no configurados.

Las contraseñas y claves utilizadas no deben publicarse en GitHub.

### 3.3 Formato de los datos enviados

El programa transmite la información mediante un mensaje JSON.

Los campos utilizados son:

| Campo | Descripción |
| :--- | :--- |
| `dispositivo` | Identificador de la ESP32 que realiza la medición. Es obligatorio. |
| `distancia_cm` | Distancia medida por el sensor, en centímetros. Es obligatorio y debe ser numérico. |
| `bateria_v` | Voltaje de batería. Hoy no se mide, porque el equipo se alimenta con un powerbank por USB, y se envía `null`. |

**Ejemplo de información enviada:**

```json
{
  "dispositivo": "esp32-sumidero",
  "distancia_cm": 45.5,
  "bateria_v": null
}
```

El valor de distancia presentado es ilustrativo.

El servidor recibe la información y devuelve una respuesta que permite comprobar si el envío se realizó correctamente, por ejemplo `{"ok":true,"id":7,"nivel_cm":54.5}`.

### 3.4 Almacenamiento de datos

El sistema se desarrolló tomando como referencia la arquitectura propuesta en la guía de TEI201, que utiliza una base de datos PostgreSQL alojada en Aiven.

En esta arquitectura, Railway recibe las mediciones y se conecta a PostgreSQL para registrar la información obtenida por la ESP32.

Esto permite conservar los datos y recuperarlos posteriormente para analizar las variaciones del nivel de agua.

**Base de datos utilizada:** PostgreSQL alojado en Aiven. El servidor se conecta con la librería `pg` mediante SSL. Si existe la variable `DB_CA_CERT`, usa el certificado CA de Aiven para verificar el servidor; si no, la conexión va cifrada pero sin verificar el certificado.

**Nombre de la tabla y estructura de almacenamiento:** tabla `mediciones`. El servidor la crea automáticamente al iniciar (`CREATE TABLE IF NOT EXISTS`):

```sql
CREATE TABLE IF NOT EXISTS mediciones (
  id SERIAL PRIMARY KEY,
  dispositivo VARCHAR(50) NOT NULL,
  distancia_cm REAL NOT NULL,
  nivel_cm REAL NOT NULL,
  bateria_v REAL,
  creado_en TIMESTAMPTZ DEFAULT NOW()
);
```

Los registros que contempla la plataforma incluyen:

| Dato | Columna | Descripción |
| :--- | :--- | :--- |
| ID | `id` | Identificador de cada registro almacenado. |
| Dispositivo | `dispositivo` | Identificador del equipo que envió la medición. |
| Distancia (cm) | `distancia_cm` | Distancia entre el sensor y la superficie del agua. |
| Nivel (cm) | `nivel_cm` | Nivel de agua calculado por el servidor. |
| Batería (V) | `bateria_v` | Voltaje de batería, cuando esté disponible. |
| Fecha | `creado_en` | Fecha y hora en que el servidor guardó la medición. |

**Conversión de distancia a nivel de agua:** la hace el servidor al recibir cada medición:

```text
nivel_cm = max(0, ALTURA_SENSOR_CM − distancia_cm)
```

El resultado se redondea a 1 decimal. `ALTURA_SENSOR_CM` es una variable de entorno con la distancia desde el sensor hasta el fondo del sumidero; el valor de ejemplo es 100 cm. Si el agua está más abajo que el fondo configurado, el nivel queda en 0. Si falta la variable, el servidor rechaza la medición con error 500.

### 3.5 Consulta de registros

El servidor permite que la plataforma web solicite información sobre las mediciones recibidas.

**Rutas utilizadas por el servidor final:**

| Método y ruta | Función |
| :--- | :--- |
| `GET /` | Entrega el dashboard (`public/index.html`). |
| `POST /api/mediciones` | Recibe una medición de la ESP32. Requiere `x-api-key`. |
| `GET /api/mediciones?limite=N` | Devuelve las últimas N mediciones, la más nueva primero. Por defecto entrega 50 y como máximo 500. |
| `GET /api/salud` | Verifica que el servidor responda y llegue a la base de datos. Devuelve `{"ok":true,"db":"conectada"}`. |

El objetivo de esta comunicación es permitir que el usuario visualice tanto las mediciones recientes como los registros históricos almacenados.

### 3.6 Procesamiento de alertas

El sistema está pensado para analizar las mediciones del sensor e identificar niveles de agua anormalmente elevados.

En el diseño propuesto, cuando se detecta una condición de riesgo se inicia un período de verificación durante el cual se comprueba si el nivel elevado se mantiene.

Si la condición persiste durante el tiempo establecido, se confirma el estado de alerta y se comunica mediante la plataforma web.

Este mecanismo permitiría reducir la generación de falsas alertas ocasionadas por variaciones momentáneas en las mediciones.

**Umbral de alerta:** no implementado en la versión actual del código.

**Tiempo de confirmación:** no implementado en la versión actual del código.

**Ubicación de la lógica de alerta (servidor o dashboard):** en la versión actual no existe lógica de alerta ni en `server.js` ni en `public/index.html`. El servidor solo calcula y guarda `nivel_cm`, y el dashboard solo lo muestra.

---

## 4. Dashboard y visualización de datos

El dashboard constituye la interfaz web del sistema de monitoreo y permite presentar las mediciones de manera clara y organizada.

**Enlace del dashboard:**

https://monitor-sumidero-production.up.railway.app/

**Código de la página:** [`public/index.html`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/public/index.html)

### 4.1 Información presentada

La página web contempla los siguientes elementos:

- Gráfico de línea del nivel de agua en centímetros (Chart.js).
- Tabla con los registros de mediciones.
- Identificador de cada registro.
- Identificador del dispositivo.
- Distancia medida por el sensor.
- Nivel de agua calculado.
- Voltaje de batería, cuando esté disponible (si no, se muestra "-").
- Fecha y hora de las mediciones.
- Hora de la última actualización y número de mediciones cargadas.

### 4.2 Consulta de mediciones históricas

El dashboard está diseñado para consultar las mediciones anteriores y representarlas mediante gráficas y tablas.

Esto permite observar cómo varía el nivel de agua a lo largo del tiempo e identificar aumentos anormales que podrían representar situaciones de riesgo.

**Funcionamiento de la consulta histórica:**

1. La página llama a `GET /api/mediciones?limite=30` y recibe las 30 mediciones más recientes desde PostgreSQL.
2. Invierte el orden para el gráfico, de modo que la más antigua queda a la izquierda.
3. En la tabla las muestra de la más nueva a la más antigua.
4. Los textos se insertan con `textContent` para evitar que se ejecute código HTML enviado en los datos.
5. Si la consulta falla, la página muestra "Error al cargar datos".

Al momento de esta revisión (22-11-2026), la base de datos de Aiven no responde (`ENOTFOUND`), por lo que la consulta devuelve "Error al leer". Hay que reactivar o volver a crear el servicio en Aiven y actualizar `DATABASE_URL` en Railway.

### 4.3 Alertas digitales

El sistema contempla la comunicación de alertas cuando se detectan condiciones anormalmente elevadas del nivel de agua.

Estas alertas buscan informar oportunamente sobre posibles situaciones de riesgo y facilitar el monitoreo de los sumideros.

**Tipo de alerta implementada en la versión final:** ninguna por ahora. El dashboard muestra el nivel en el gráfico y en la tabla, pero no hay indicador visual ni notificación de alerta.

**Frecuencia de actualización de la página:** cada 5 segundos (`setInterval(actualizar, 5000)`). Como la ESP32 envía cada 10 segundos, cada medición nueva aparece en el dashboard en un máximo de unos 5 segundos.

---

## 5. Librerías, dependencias y herramientas

### 5.1 Firmware de la ESP32

| Librería | Función | Instalación |
| :--- | :--- | :--- |
| `WiFi.h` | Conexión de la ESP32 a una red WiFi. | Incluida en el paquete ESP32 |
| `WiFiClientSecure.h` | Comunicación mediante HTTPS. | Incluida en el paquete ESP32 |
| `HTTPClient.h` | Envío de solicitudes HTTP al servidor. | Incluida en el paquete ESP32 |
| `ArduinoJson.h` | Construcción de los datos en formato JSON. | ArduinoJson 7 (versión instalada: 7.4.3) |

### 5.2 Servidor y base de datos

| Tecnología | Función | Versión en `package.json` | Versión instalada (`package-lock.json`) |
| :--- | :--- | :--- | :--- |
| Node.js | Ejecución del servidor. | `>=18` | — |
| Express | Gestión de solicitudes y funcionamiento de la API. | ^4.21.0 | 4.22.3 |
| `pg` | Conexión entre Node.js y PostgreSQL. | ^8.13.0 | 8.23.1 |
| `dotenv` | Configuración de variables de entorno. | ^16.4.5 | 16.6.1 |
| PostgreSQL | Almacenamiento de mediciones. | Servicio administrado por Aiven | No se pudo verificar porque la base no responde |
| Chart.js | Creación de gráficas en el dashboard. | 4.4.1, cargado por CDN (jsDelivr) | 4.4.1 |
| Railway | Alojamiento del servidor en Internet. | Servicio en la nube | — |
| Aiven | Alojamiento de PostgreSQL. | Servicio en la nube | — |

**Versiones efectivamente utilizadas por el servidor:** Express 4.22.3, pg 8.23.1 y dotenv 16.6.1, según [`package-lock.json`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/package-lock.json). En [`package.json`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/package.json) aparecen como `^4.21.0`, `^8.13.0` y `^16.4.5`.

### 5.3 Herramientas utilizadas

- **Arduino IDE:** programación y carga del firmware en la ESP32.
- **ESP32 Arduino Core:** soporte para el microcontrolador ESP32-S3.
- **Node.js:** entorno de ejecución del servidor.
- **Railway:** plataforma de alojamiento del servidor.
- **Aiven:** servicio de base de datos PostgreSQL.
- **GitHub:** almacenamiento y documentación del código fuente.
- **Navegador web:** consulta del dashboard.

**Versión utilizada del paquete ESP32 Arduino Core:** 3.3.13 (esp32 de Espressif Systems).

Las fuentes externas, librerías y herramientas de inteligencia artificial utilizadas se documentan en [FUENTES.md](../FUENTES.md).

---

## 6. Instalación y puesta en marcha

### 6.1 Preparación del entorno

Para instalar y ejecutar el firmware se requieren:

- Arduino IDE.
- Paquete de placas ESP32 de Espressif (versión 3.3.13).
- Librería ArduinoJson versión 7.
- ESP32-S3 Super Mini.
- Sensor ultrasónico impermeable AJ-SR04M.
- Cable USB.
- Red WiFi de 2.4 GHz con acceso a Internet.

### 6.2 Cargar el código en la ESP32

1. Abrir Arduino IDE.
2. Instalar el paquete de placas ESP32 desde el Gestor de placas.
3. Instalar ArduinoJson versión 7 desde el Gestor de Bibliotecas.
4. Abrir el archivo [`monitor_sumidero.ino`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/esp32/monitor_sumidero/monitor_sumidero.ino).
5. En la misma carpeta, copiar [`secrets.h.example`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/esp32/monitor_sumidero/secrets.h.example) como `secrets.h` y completar `WIFI_SSID`, `WIFI_PASS` y `API_KEY` (la misma clave configurada en Railway). `SERVER_URL` ya apunta a `https://monitor-sumidero-production.up.railway.app/api/mediciones`.
6. Seleccionar la placa `ESP32S3 Dev Module`.
7. Seleccionar el puerto USB correspondiente.
8. Compilar y cargar el programa.
9. Abrir el Monitor Serial a **115200 baudios**.
10. Verificar que se establezca la conexión WiFi y que las mediciones se transmitan al servidor. Debe aparecer `POST ... -> 201`.

Las conexiones físicas del sensor se encuentran documentadas en [Hardware](../hardware/README.md).

### 6.3 Conexión WiFi

La ESP32 se conecta automáticamente a una red WiFi de 2.4 GHz mediante las credenciales configuradas en `secrets.h`.

Una vez conectada, obtiene una dirección IP local que permite identificarla dentro de la red.

Sin embargo, en la versión final del proyecto, la ESP32 no funciona como servidor web local.

Su función es utilizar la conexión a Internet para transmitir las mediciones a Railway.

Debido a que el servidor está alojado en Internet, el computador o celular utilizado para consultar el dashboard no necesita encontrarse conectado a la misma red WiFi de la ESP32.

### 6.4 Configuración del servidor

La implementación se basa en la guía de TEI201, que propone un servidor Node.js con Express alojado en Railway y conectado a una base de datos PostgreSQL.

El procedimiento de referencia contempla:

1. Instalar Node.js y las dependencias del proyecto.
2. Configurar el servidor para recibir las mediciones.
3. Establecer la conexión con la base de datos.
4. Configurar las variables de entorno necesarias.
5. Publicar el servidor mediante Railway.
6. Verificar que la API reciba las mediciones de la ESP32.
7. Comprobar que los datos queden almacenados.
8. Acceder al dashboard para consultar los registros.

**Variables de entorno utilizadas por el servidor final** (plantilla en [`.env.example`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/.env.example)):

| Variable | Obligatoria | Descripción |
| :--- | :--- | :--- |
| `DATABASE_URL` | Sí | Cadena de conexión de Aiven PostgreSQL (`postgres://USUARIO:CLAVE@HOST:PUERTO/defaultdb`). Lo que va después de `?` se ignora, porque el SSL se configura en el código. |
| `DB_CA_CERT` | No | Certificado CA de Aiven en una sola línea, con los saltos escritos como `\n`. Si está, se verifica el certificado de la base. |
| `API_KEY` | Sí | Clave compartida con la ESP32 para el encabezado `x-api-key`. |
| `ALTURA_SENSOR_CM` | Sí | Distancia del sensor al fondo del sumidero, en cm. Se usa para calcular el nivel. |
| `PORT` | No | Puerto del servidor. Railway lo define solo; en local se usa 3000. |

**Instrucciones específicas para ejecutar el servidor adaptado:**

En local:

```bash
git clone https://github.com/JoaquinZavalaM/Monitor-Sumidero.git
cd Monitor-Sumidero
npm install
cp .env.example .env    # completar con los datos reales
npm start               # equivale a: node server.js
```

Luego abrir `http://localhost:3000` (dashboard) y `http://localhost:3000/api/salud` (prueba de la base de datos). Al iniciar, el servidor crea la tabla `mediciones` si no existe.

En Railway:

1. Conectar el repositorio de GitHub.
2. Cargar `DATABASE_URL`, `API_KEY`, `ALTURA_SENSOR_CM` y, si se usa, `DB_CA_CERT` en la pestaña *Variables*.
3. Railway ejecuta `npm start` automáticamente.
4. Generar el dominio público en *Settings → Networking*.

Las contraseñas y claves privadas deben mantenerse fuera del repositorio público.

### 6.5 Verificación del funcionamiento

1. Encender la ESP32 con el sensor conectado.
2. Verificar que la placa establezca conexión WiFi.
3. Comprobar que el sensor entregue mediciones válidas.
4. Revisar en el Monitor Serial las solicitudes enviadas a Railway.
5. Confirmar que el servidor reciba la información.
6. Abrir el dashboard.
7. Comprobar que las mediciones se visualicen correctamente.
8. Verificar que los registros históricos puedan recuperarse.
9. Simular un aumento del nivel de agua para comprobar el funcionamiento de las alertas.

**Resultados de la verificación final:** COMPLETAR con evidencia de las pruebas (capturas del Monitor Serial y del dashboard). Revisión del servidor del 22-11-2026:

- `GET /` responde 200 y el dashboard carga.
- `GET /api/salud` y `GET /api/mediciones` fallan porque la base de Aiven no se encuentra (`ENOTFOUND`).
- En la revisión se detectó que el firmware enviaba el POST a la raíz del servidor (respuesta 404), porque a `SERVER_URL` le faltaba `/api/mediciones`. Se corrigió en el firmware; falta confirmar el envío con la base de datos activa.
- El paso 9 no se puede probar porque las alertas no están implementadas.

---

## 7. Seguridad y consideraciones técnicas

### 7.1 Protección de credenciales

El sistema utiliza credenciales privadas para conectarse a la red WiFi y autorizar la comunicación con el servidor.

Estas credenciales no deben publicarse en GitHub:

- **Firmware:** las credenciales van en `secrets.h`, que está en `.gitignore`. En el repositorio solo se publica la plantilla `secrets.h.example`, con valores de ejemplo.
- **Servidor:** las credenciales van en `.env`, también en `.gitignore`, o en las variables de Railway. En el repositorio solo se publica `.env.example`.

En caso de publicar accidentalmente una contraseña o clave de API, esta debe cambiarse o revocarse.

El servidor responde 401 a cualquier solicitud con una clave incorrecta. Si la variable `API_KEY` no está configurada, rechaza todas las solicitudes. Además, las consultas SQL usan parámetros (`$1`, `$2`…) para evitar inyección SQL, y los errores internos quedan en el log sin mostrarse al cliente.

### 7.2 Comunicación HTTPS

El firmware utiliza HTTPS para transmitir las mediciones.

En la versión compartida del programa se utiliza `setInsecure()`, que permite establecer una conexión cifrada sin verificar el certificado del servidor.

Esta configuración facilita las pruebas del prototipo, pero debería reemplazarse por una validación adecuada del certificado para una implementación definitiva.

### 7.3 Validación de mediciones

El programa descarta las lecturas fuera del rango establecido o aquellas que no cuentan con suficientes mediciones válidas.

El servidor también valida lo que recibe. Rechaza con 400 los mensajes sin `dispositivo`, con `distancia_cm` no numérica o con `bateria_v` que no sea numérica ni nula.

Esto permite reducir la transmisión y el almacenamiento de valores incorrectos.

### 7.4 Limitaciones y mejoras futuras

- Implementar la lógica de alertas (umbral y tiempo de confirmación).
- Comprobar la precisión del sensor bajo diferentes condiciones.
- Evaluar la estabilidad de la conexión WiFi.
- Verificar la persistencia de los registros almacenados.
- Evaluar el tiempo de respuesta de las alertas.
- Comprobar el funcionamiento continuo del sistema.
- Mejorar la seguridad de las comunicaciones.
- Evaluar el comportamiento del dispositivo en condiciones reales de instalación.

Las pruebas y resultados correspondientes se documentan en [Testing](../testing/README.md).

---

## 8. Referencias

El desarrollo del sistema de comunicación se realizó tomando como referencia la guía:

**Méndez, Fernando.** *ESP32 conectado a una base de datos en la nube: Aiven (PostgreSQL) + Railway (servidor y página web) + GitHub*. Material de TEI201, 2026.

El servidor fue adaptado al sistema de monitoreo de sumideros según lo informado por el equipo.

El código fuente completo se encuentra en el repositorio [JoaquinZavalaM/Monitor-Sumidero](https://github.com/JoaquinZavalaM/Monitor-Sumidero).

Las herramientas externas, librerías y usos de inteligencia artificial se documentan en [FUENTES.md](../FUENTES.md).
