// =====================================================
// PROYECTO: DETECTOR DE NIVEL ANORMAL EN SUMIDERO
// Basado en Avance2_oct12a.ino + conexión WiFi (ESP32)
// CONFIGURACIÓN DE PINES:
// TRIG -> Pin 5
// ECHO -> Pin 4
// LED (Resistencia 220 ohm) -> Pin 6
//
// WiFi:
//   GET /        -> página web del monitor (pagina.h)
//   GET /estado  -> {"distancia":7.5,"estado":"ALERTA"}
// La IP se muestra en el Monitor Serial al conectarse.
// El monitoreo y el Monitor Serial funcionan aunque no haya WiFi.
// =====================================================

#include <WiFi.h>
#include <WebServer.h>
#include "pagina.h"   // generado desde ../index.html con ../generar_pagina.sh


// ---------------- WIFI (EDITA ESTO) ----------------

const char* WIFI_SSID = "TU_WIFI";   // Red de 2.4 GHz
const char* WIFI_PASS = "TU_CLAVE";


// ---------------- PINES ----------------

const int TRIG_PIN = 5;  // GPIO 5 (1.er pin lado derecho desde el USB)
const int ECHO_PIN = 4;  // GPIO 4 (4.º pin lado izquierdo desde el USB)
const int LED_PIN  = 6;  // GPIO 6 (2.º pin lado derecho, va a la resistencia)


// ---------------- CONFIGURACIÓN ----------------

// Si la distancia entre el sensor y el agua es
// MENOR o igual a este valor (en cm), consideramos
// que el nivel de agua es anormal.
const float UMBRAL_ALERTA = 8.0;

// El nivel debe mantenerse alto durante 5 segundos (5000 ms)
// para confirmar la alerta.
const unsigned long TIEMPO_CONFIRMACION = 5000;

// Realizar una medición cada 500 ms.
const unsigned long INTERVALO_MEDICION = 500;


// ---------------- VARIABLES ----------------

float distancia = 0;

bool nivelAlto = false;
bool alerta = false;

unsigned long inicioNivelAlto = 0;
unsigned long ultimaMedicion = 0;

WebServer server(80);
bool wifiConectado = false;
unsigned long ultimoReintentoWifi = 0;


// =====================================================
// FUNCIÓN PARA MEDIR DISTANCIA
// =====================================================

float medirDistancia() {
  // Asegurar que TRIG esté apagado inicialmente
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Activar el sensor con pulso de 10 microsegundos
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Recibir el eco en el pin 4
  unsigned long duracion = pulseIn(ECHO_PIN, HIGH, 30000);

  // Si no se recibió ningún eco
  if (duracion == 0) {
    return -1.0;
  }

  // Convertir tiempo a centímetros
  float distanciaCM = (duracion * 0.0343) / 2.0;

  return distanciaCM;
}


// =====================================================
// MOSTRAR INFORMACIÓN EN MONITOR SERIAL
// =====================================================

void mostrarEstado() {
  Serial.println("--------------------------------");

  Serial.print("Distancia al agua: ");
  Serial.print(distancia, 1);
  Serial.println(" cm");

  Serial.print("Umbral de alerta: ");
  Serial.print(UMBRAL_ALERTA, 1);
  Serial.println(" cm");

  if (alerta) {
    Serial.println("ESTADO: ALERTA");
    Serial.println("POSIBLE OBSTRUCCION");
  }
  else if (nivelAlto) {
    Serial.println("ESTADO: VERIFICANDO");
    Serial.println("Nivel de agua elevado...");
  }
  else {
    Serial.println("ESTADO: NORMAL");
    Serial.println("Nivel de agua dentro del rango.");
  }

  Serial.println("--------------------------------");
}


// =====================================================
// WIFI / SERVIDOR WEB
// =====================================================

const char* nombreEstado() {
  if (alerta) return "ALERTA";
  if (nivelAlto) return "VERIFICANDO";
  return "NORMAL";
}

void enviarPagina() {
  server.send_P(200, "text/html; charset=utf-8", PAGINA);
}

void enviarEstado() {
  String json = "{\"distancia\":" + String(distancia, 1) +
                ",\"estado\":\"" + nombreEstado() + "\"}";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Private-Network", "true");
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", json);
}

void gestionarWifi() {
  bool conectado = (WiFi.status() == WL_CONNECTED);

  if (conectado && !wifiConectado) {
    wifiConectado = true;
    Serial.println();
    Serial.print("WiFi conectado. Abre http://");
    Serial.println(WiFi.localIP());
    Serial.println();
  }
  else if (!conectado && wifiConectado) {
    wifiConectado = false;
    Serial.println("WiFi desconectado, reintentando...");
  }

  // Reintentar cada 10 s si no hay conexión
  if (!conectado && millis() - ultimoReintentoWifi >= 10000) {
    ultimoReintentoWifi = millis();
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);
  }
}


// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  // Espera breve para que inicie el puerto USB nativo
  delay(2000);

  // Configurar pines
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Apagar TRIG y LED inicialmente
  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.println("======================================");
  Serial.println("   SISTEMA DE MONITOREO DE SUMIDERO");
  Serial.println("   TRIG: Pin 5 | ECHO: Pin 4 | LED: 6");
  Serial.println("======================================");
  Serial.println();

  // Iniciar WiFi sin bloquear el monitoreo
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  ultimoReintentoWifi = millis();
  Serial.print("Conectando a WiFi: ");
  Serial.println(WIFI_SSID);

  server.on("/", enviarPagina);
  server.on("/estado", enviarEstado);
  server.begin();

  Serial.println("Sistema iniciado.");
  Serial.println("Esperando mediciones...");
  Serial.println();
}


// =====================================================
// LOOP PRINCIPAL
// =====================================================

void loop() {
  server.handleClient();
  gestionarWifi();

  unsigned long tiempoActual = millis();

  // ---------------------------------------------------
  // REALIZAR MEDICIÓN CADA 500 MS
  // ---------------------------------------------------
  if (tiempoActual - ultimaMedicion >= INTERVALO_MEDICION) {
    ultimaMedicion = tiempoActual;

    // Medir distancia
    float nuevaDistancia = medirDistancia();

    // -------------------------------------------------
    // COMPROBAR MEDICIÓN
    // -------------------------------------------------
    if (nuevaDistancia > 0) {
      distancia = nuevaDistancia;

      // ===============================================
      // NIVEL DE AGUA ANORMAL
      // ===============================================
      if (distancia <= UMBRAL_ALERTA) {

        // Primera detección del nivel alto
        if (!nivelAlto) {
          nivelAlto = true;
          inicioNivelAlto = tiempoActual;

          Serial.println();
          Serial.println("⚠ NIVEL DE AGUA ELEVADO");
          Serial.println("Iniciando verificacion...");
        }

        // =============================================
        // CONFIRMAR ALERTA TRAS 5 SEGUNDOS
        // =============================================
        if (nivelAlto && !alerta && (tiempoActual - inicioNivelAlto >= TIEMPO_CONFIRMACION)) {
          alerta = true;

          // Encender LED (Pin 6)
          digitalWrite(LED_PIN, HIGH);

          Serial.println();
          Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
          Serial.println("           ALERTA");
          Serial.println("    POSIBLE OBSTRUCCION");
          Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
          Serial.println();
        }
      }

      // ===============================================
      // NIVEL NORMAL
      // ===============================================
      else {
        // Si antes había una condición anormal
        if (nivelAlto || alerta) {
          Serial.println();
          Serial.println("✓ NIVEL VOLVIO A LA NORMALIDAD");
          Serial.println();
        }

        // Reiniciar estados
        nivelAlto = false;
        alerta = false;

        // Apagar LED (Pin 6)
        digitalWrite(LED_PIN, LOW);
      }

      // Mostrar información en Monitor Serial
      mostrarEstado();
    }
    else {
      Serial.println("Error: Sin respuesta del sensor (revisa que TRIG vaya al 5 y ECHO al 4).");
    }
  }
}
