
# Software — Sistema de monitoreo de sumideros

Esta carpeta contiene la documentación técnica y los programas utilizados para el monitoreo del nivel de agua en sumideros urbanos.

El sistema utiliza una **ESP32-S3 Super Mini** conectada a un **sensor ultrasónico impermeable AJ-SR04M**, que permite medir la distancia entre el sensor y la superficie del agua.

Las mediciones son procesadas por la ESP32 y transmitidas mediante WiFi a un servidor externo alojado en **Railway**, donde pueden almacenarse y consultarse a través de una plataforma web.

El sistema permite monitorear las variaciones del nivel de agua y generar alertas digitales ante condiciones anormalmente elevadas.

**Dashboard del proyecto:** https://monitor-sumidero-production.up.railway.app/

---

## 1. Archivos del software

| Archivo | Función |
| :--- | :--- |
| `monitor_sumidero.ino` | Programa que controla la ESP32, realiza las mediciones del sensor y transmite los datos al servidor. |
| `server.js` o equivalente | Código del servidor que recibe, procesa y administra las mediciones. **COMPLETAR: nombre y ubicación del archivo real.** |
| `package.json` | Archivo que contiene las dependencias necesarias para ejecutar el servidor. **COMPLETAR: adjuntar versión utilizada.** |
| `public/index.html` o equivalente | Código de la página web que permite visualizar las mediciones y los registros del sistema. **COMPLETAR: nombre y ubicación del archivo real.** |
| `.env.example` | Archivo de ejemplo para configurar las variables privadas del servidor sin publicar contraseñas. **COMPLETAR: adjuntar si corresponde.** |
| `README.md` | Documentación técnica, funcionamiento e instrucciones de instalación del software. |

---

## 2. Funcionamiento del firmware de la ESP32

El archivo `monitor_sumidero.ino` contiene el programa principal utilizado para controlar la ESP32-S3 Super Mini y obtener las mediciones del sensor impermeable AJ-SR04M.

### 2.1 Captura de mediciones

1. La ESP32 activa el sensor ultrasónico mediante el pin TRIG (GPIO 5).
2. El sensor emite una onda ultrasónica que rebota en la superficie del agua.
3. La onda regresa al sensor y su señal es recibida mediante ECHO (GPIO 4).
4. A partir del tiempo de retorno y la velocidad del sonido, la ESP32 calcula la distancia hasta la superficie del agua.
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

Cada **10 segundos**, la ESP32 obtiene una medición procesada y la transmite mediante WiFi a un servidor externo alojado en Railway.

La comunicación se realiza mediante una solicitud HTTPS de tipo POST, enviando la información en formato JSON.

Si se pierde la conexión WiFi, el programa intenta restablecerla para continuar transmitiendo las mediciones.

El Monitor Serial permite observar las distancias medidas, los errores y las respuestas recibidas desde el servidor.

### 2.4 Parámetros de operación

| Parámetro | Valor |
| :--- | :--- |
| Microcontrolador | ESP32-S3 Super Mini |
| Sensor | AJ-SR04M ultrasónico impermeable |
| Pin TRIG | GPIO 5 |
| Pin ECHO | GPIO 4 |
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
             [ Envío por HTTPS ]
                       |
                       v
            [ Servidor Railway ]
                       |
                       v
          [ Base de datos PostgreSQL ]
              [ Aiven - confirmar ]
                       |
                       v
           [ Consulta de registros ]
                       |
                       v
               [ Dashboard ]
                       |
             Gráficas, mediciones
                 y alertas
```

La ESP32 no se conecta directamente a la base de datos. Su función es obtener las mediciones y enviarlas al servidor Railway.

El servidor recibe la información y se comunica con la base de datos para almacenar y consultar los registros.

**Tecnología del servidor:** Node.js con Express, según la guía utilizada como referencia. **COMPLETAR: confirmar implementación final.**

### 3.2 Comunicación con Railway

La ESP32 se conecta a Internet mediante WiFi y envía las mediciones al servidor utilizando solicitudes HTTPS de tipo POST.

**Dirección del servidor:**

https://monitor-sumidero-production.up.railway.app/

**Ruta de recepción de mediciones:** COMPLETAR según el código del servidor adaptado.

Para autorizar la comunicación, el programa utiliza una clave de API que se envía mediante el encabezado `x-api-key`.

Esta clave permite que el servidor identifique las solicitudes autorizadas y evite recibir información de dispositivos no configurados.

Las contraseñas y claves utilizadas no deben publicarse en GitHub.

### 3.3 Formato de los datos enviados

El programa transmite la información mediante un mensaje JSON.

Los campos utilizados son:

| Campo | Descripción |
| :--- | :--- |
| `dispositivo` | Identificador de la ESP32 que realiza la medición. |
| `distancia_cm` | Distancia medida por el sensor, expresada en centímetros. |
| `bateria_v` | Voltaje de batería. Actualmente no se realiza esta medición, por lo que se envía un valor nulo. |

**Ejemplo de información enviada:**

```json
{
  "dispositivo": "esp32-sumidero",
  "distancia_cm": 45.5,
  "bateria_v": null
}
```

El valor de distancia presentado es ilustrativo.

El servidor recibe la información y devuelve una respuesta que permite comprobar si el envío se realizó correctamente.

### 3.4 Almacenamiento de datos

El sistema se desarrolló tomando como referencia la arquitectura propuesta en la guía de TEI201, que utiliza una base de datos PostgreSQL alojada en Aiven.

En esta arquitectura, Railway recibe las mediciones y se conecta a PostgreSQL para registrar la información obtenida por la ESP32.

Esto permite conservar los datos y recuperarlos posteriormente para analizar las variaciones del nivel de agua.

**Base de datos utilizada:** PostgreSQL mediante Aiven. **COMPLETAR: confirmar servicio utilizado en el proyecto.**

**Nombre de la tabla y estructura de almacenamiento:** COMPLETAR según el código real del servidor.

Los registros que contempla la plataforma incluyen:

| Dato | Descripción |
| :--- | :--- |
| ID | Identificador de cada registro almacenado. |
| Dispositivo | Identificador del equipo que envió la medición. |
| Distancia (cm) | Distancia entre el sensor y la superficie del agua. |
| Nivel (cm) | Nivel de agua calculado por el sistema. |
| Batería (V) | Voltaje de batería, cuando esté disponible. |
| Fecha | Fecha y hora asociadas a la medición. |

**Conversión de distancia a nivel de agua:** COMPLETAR según el procedimiento implementado en el servidor.

### 3.5 Consulta de registros

El servidor permite que la plataforma web solicite información sobre las mediciones recibidas.

Según la guía utilizada como referencia, el servidor dispone de rutas para consultar registros anteriores y comprobar su conexión con la base de datos.

**Rutas utilizadas por el servidor final:** COMPLETAR.

El objetivo de esta comunicación es permitir que el usuario visualice tanto las mediciones recientes como los registros históricos almacenados.

### 3.6 Procesamiento de alertas

El sistema analiza las mediciones del sensor para identificar niveles de agua anormalmente elevados.

Cuando se detecta una condición de riesgo, se inicia un período de verificación durante el cual se comprueba si el nivel elevado se mantiene.

Si la condición persiste durante el tiempo establecido, se confirma el estado de alerta y se comunica mediante la plataforma web.

Este mecanismo permite reducir la generación de falsas alertas ocasionadas por variaciones momentáneas en las mediciones.

**Umbral de alerta:** COMPLETAR según la programación final.

**Tiempo de confirmación:** COMPLETAR según la programación final.

**Ubicación de la lógica de alerta (servidor o dashboard):** COMPLETAR.

---

## 4. Dashboard y visualización de datos

El dashboard constituye la interfaz web del sistema de monitoreo y permite presentar las mediciones de manera clara y organizada.

**Enlace del dashboard:**

https://monitor-sumidero-production.up.railway.app/

### 4.1 Información presentada

La página web contempla los siguientes elementos:

- Gráfico del nivel de agua en centímetros.
- Tabla con los registros de mediciones.
- Identificador de cada registro.
- Identificador del dispositivo.
- Distancia medida por el sensor.
- Nivel de agua calculado.
- Voltaje de batería, cuando esté disponible.
- Fecha y hora de las mediciones.

### 4.2 Consulta de mediciones históricas

El dashboard está diseñado para consultar las mediciones anteriores y representarlas mediante gráficas y tablas.

Esto permite observar cómo varía el nivel de agua a lo largo del tiempo e identificar aumentos anormales que podrían representar situaciones de riesgo.

**Funcionamiento de la consulta histórica:** COMPLETAR después de verificar la conexión con la base de datos.

### 4.3 Alertas digitales

El sistema contempla la comunicación de alertas cuando se detectan condiciones anormalmente elevadas del nivel de agua.

Estas alertas buscan informar oportunamente sobre posibles situaciones de riesgo y facilitar el monitoreo de los sumideros.

**Tipo de alerta implementada en la versión final:** COMPLETAR según el dashboard.

**Frecuencia de actualización de la página:** COMPLETAR según el código final.

---

## 5. Librerías, dependencias y herramientas

### 5.1 Firmware de la ESP32

| Librería | Función | Instalación |
| :--- | :--- | :--- |
| `WiFi.h` | Conexión de la ESP32 a una red WiFi. | Incluida en el paquete ESP32 |
| `WiFiClientSecure.h` | Comunicación mediante HTTPS. | Incluida en el paquete ESP32 |
| `HTTPClient.h` | Envío de solicitudes HTTP al servidor. | Incluida en el paquete ESP32 |
| `ArduinoJson.h` | Construcción de los datos en formato JSON. | ArduinoJson versión 7 |

### 5.2 Servidor y base de datos

La guía utilizada para desarrollar el servidor contempla las siguientes tecnologías:

| Tecnología | Función | Versión de referencia |
| :--- | :--- | :--- |
| Node.js | Ejecución del servidor. | 18 o superior |
| Express | Gestión de solicitudes y funcionamiento de la API. | ^4.21.0 |
| PostgreSQL | Almacenamiento de mediciones. | COMPLETAR |
| `pg` | Conexión entre Node.js y PostgreSQL. | ^8.13.0 |
| `dotenv` | Configuración de variables de entorno. | ^16.4.5 |
| Railway | Alojamiento del servidor en Internet. | Servicio en la nube |
| Aiven | Alojamiento de PostgreSQL. | Servicio en la nube |
| Chart.js | Creación de gráficas en el dashboard. | COMPLETAR |

**Versiones efectivamente utilizadas por el servidor:** COMPLETAR según `package.json`.

Las versiones indicadas corresponden a la guía de referencia y deben actualizarse si el servidor del equipo utiliza otras.

### 5.3 Herramientas utilizadas

- **Arduino IDE:** programación y carga del firmware en la ESP32.
- **ESP32 Arduino Core:** soporte para el microcontrolador ESP32-S3.
- **Node.js:** entorno de ejecución del servidor.
- **Railway:** plataforma de alojamiento del servidor.
- **Aiven:** servicio de base de datos propuesto en la guía.
- **GitHub:** almacenamiento y documentación del código fuente.
- **Navegador web:** consulta del dashboard.

**Versión utilizada del paquete ESP32 Arduino Core:** COMPLETAR.

Las fuentes externas, librerías y herramientas de inteligencia artificial utilizadas se documentan en [FUENTES.md](../FUENTES.md).

---

## 6. Instalación y puesta en marcha

### 6.1 Preparación del entorno

Para instalar y ejecutar el firmware se requieren:

- Arduino IDE.
- Paquete de placas ESP32 de Espressif.
- Librería ArduinoJson versión 7.
- ESP32-S3 Super Mini.
- Sensor ultrasónico impermeable AJ-SR04M.
- Cable USB.
- Red WiFi de 2.4 GHz con acceso a Internet.

### 6.2 Cargar el código en la ESP32

1. Abrir Arduino IDE.
2. Instalar el paquete de placas ESP32 desde el Gestor de placas.
3. Instalar ArduinoJson versión 7 desde el Gestor de Bibliotecas.
4. Abrir el archivo `monitor_sumidero.ino`.
5. Configurar de forma privada las credenciales de la red WiFi y los datos necesarios para conectarse al servidor.
6. Seleccionar la placa `ESP32S3 Dev Module`.
7. Seleccionar el puerto USB correspondiente.
8. Compilar y cargar el programa.
9. Abrir el Monitor Serial a **115200 baudios**.
10. Verificar que se establezca la conexión WiFi y que las mediciones se transmitan al servidor.

Las conexiones físicas del sensor se encuentran documentadas en [Hardware](../hardware/README.md).

### 6.3 Conexión WiFi

La ESP32 se conecta automáticamente a una red WiFi de 2.4 GHz mediante las credenciales configuradas en el firmware.

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

**Variables de entorno utilizadas por el servidor final:** COMPLETAR.

**Instrucciones específicas para ejecutar el servidor adaptado:** COMPLETAR según los archivos reales.

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

**Resultados de la verificación final:** COMPLETAR con evidencia de las pruebas realizadas.

---

## 7. Seguridad y consideraciones técnicas

### 7.1 Protección de credenciales

El sistema utiliza credenciales privadas para conectarse a la red WiFi y autorizar la comunicación con el servidor.

Estas credenciales no deben publicarse en GitHub.

El firmware publicado debe utilizar valores de ejemplo, mientras que las claves reales deben mantenerse en una configuración privada.

En caso de publicar accidentalmente una contraseña o clave de API, esta debe cambiarse o revocarse.

### 7.2 Comunicación HTTPS

El firmware utiliza HTTPS para transmitir las mediciones.

En la versión compartida del programa se utiliza `setInsecure()`, que permite establecer una conexión cifrada sin verificar el certificado del servidor.

Esta configuración facilita las pruebas del prototipo, pero debería reemplazarse por una validación adecuada del certificado para una implementación definitiva.

### 7.3 Validación de mediciones

El programa descarta las lecturas fuera del rango establecido o aquellas que no cuentan con suficientes mediciones válidas.

Esto permite reducir la transmisión de valores incorrectos.

### 7.4 Limitaciones y mejoras futuras

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

Las herramientas externas, librerías y usos de inteligencia artificial se documentan en [FUENTES.md](../FUENTES.md).
