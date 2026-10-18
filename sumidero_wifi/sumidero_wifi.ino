// =====================================================
// DETECTOR DE NIVEL ANORMAL EN SUMIDERO - VERSION WIFI
// Requiere una placa ESP32 (core "esp32" de Espressif).
// TRIG -> Pin 5 | ECHO -> Pin 4 | LED (220 ohm) -> Pin 6
//
// La placa se conecta a tu WiFi y:
//   GET /        -> sirve la pagina web (pagina.h)
//   GET /estado  -> {"distancia":7.5,"estado":"ALERTA"}
// Abre http://IP_DE_LA_PLACA (la IP sale en el Monitor Serial).
// =====================================================

#include <WiFi.h>
#include <WebServer.h>
#include "pagina.h"   // generado desde ../index.html (ver README)

// ---------------- EDITA ESTO ----------------
const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASS = "TU_CLAVE";

// ---------------- PINES ----------------
const int TRIG_PIN = 5;
const int ECHO_PIN = 4;
const int LED_PIN  = 6;

// ---------------- CONFIGURACION ----------------
const float UMBRAL_ALERTA = 8.0;
const unsigned long TIEMPO_CONFIRMACION = 5000;
const unsigned long INTERVALO_MEDICION = 500;

float distancia = 0;
bool nivelAlto = false;
bool alerta = false;
unsigned long inicioNivelAlto = 0;
unsigned long ultimaMedicion = 0;

WebServer server(80);

float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duracion = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duracion == 0) return -1.0;
  return (duracion * 0.0343) / 2.0;
}

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

void setup() {
  Serial.begin(9600);
  delay(2000);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.print("Conectando a WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Listo. Abre http://");
  Serial.println(WiFi.localIP());

  server.on("/", enviarPagina);
  server.on("/estado", enviarEstado);
  server.begin();
}

void loop() {
  server.handleClient();

  // Si se cae el WiFi, reintentar sin detener el monitoreo
  if (WiFi.status() != WL_CONNECTED) WiFi.reconnect();

  unsigned long ahora = millis();
  if (ahora - ultimaMedicion < INTERVALO_MEDICION) return;
  ultimaMedicion = ahora;

  float nueva = medirDistancia();
  if (nueva <= 0) {
    Serial.println("Error: Sin respuesta del sensor (revisa TRIG 5 / ECHO 4).");
    return;
  }
  distancia = nueva;

  if (distancia <= UMBRAL_ALERTA) {
    if (!nivelAlto) {
      nivelAlto = true;
      inicioNivelAlto = ahora;
    }
    if (!alerta && (ahora - inicioNivelAlto >= TIEMPO_CONFIRMACION)) {
      alerta = true;
      digitalWrite(LED_PIN, HIGH);
    }
  } else {
    nivelAlto = false;
    alerta = false;
    digitalWrite(LED_PIN, LOW);
  }

  Serial.print(distancia, 1);
  Serial.print(" cm - ");
  Serial.println(nombreEstado());
}
