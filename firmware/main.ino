#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// ===== COMPLETAR SOLO EN TU COMPUTADOR =====
const char* NOMBRE_WIFI = "CONFIGURAR_HOTSPOT";
const char* PASSWORD_WIFI = "CONFIGURAR_PASSWORD";
const char* CLAVE_API = "CONFIGURAR_API_KEY";
// ==========================================

const char* URL_LECTURAS =
  "https://estacion-ambiental-production.up.railway.app/api/lecturas";

constexpr uint8_t PIN_DHT = 4;
constexpr uint8_t PIN_MQ9 = 5;
constexpr uint8_t PIN_DSM = 7;

// MQ-9: 10 kohm arriba y 10 + 4,7 kohm hacia GND.
constexpr float FACTOR_MQ9 = 24.7f / 14.7f;

constexpr uint32_t CALENTAMIENTO_MS = 60000;
constexpr uint32_t VENTANA_US = 30000000;

DHT dht(PIN_DHT, DHT11);

// Variables compartidas entre la interrupcion y el programa.
portMUX_TYPE muxDSM = portMUX_INITIALIZER_UNLOCKED;
volatile bool estadoLow = false;
volatile uint32_t ultimoCambioUs = 0;
volatile uint32_t acumuladoLowUs = 0;
volatile uint32_t pulsos = 0;

uint32_t inicioVentanaUs = 0;
uint32_t inicioMs = 0;
uint32_t ultimoIntentoWiFi = 0;
bool dsmListo = false;

// Registra el tiempo en LOW sin esperar bloqueado por cada pulso.
void ARDUINO_ISR_ATTR cambioDSM() {
  portENTER_CRITICAL_ISR(&muxDSM);

  uint32_t ahora = micros();
  bool nuevoLow = digitalRead(PIN_DSM) == LOW;

  if (estadoLow) {
    acumuladoLowUs += ahora - ultimoCambioUs;
  }

  if (nuevoLow && !estadoLow) {
    pulsos++;
  }

  estadoLow = nuevoLow;
  ultimoCambioUs = ahora;

  portEXIT_CRITICAL_ISR(&muxDSM);
}

void iniciarVentanaDSM() {
  portENTER_CRITICAL(&muxDSM);

  uint32_t ahora = micros();
  acumuladoLowUs = 0;
  pulsos = 0;
  estadoLow = digitalRead(PIN_DSM) == LOW;
  ultimoCambioUs = ahora;
  inicioVentanaUs = ahora;

  portEXIT_CRITICAL(&muxDSM);
}

// Cierra la ventana y empieza la siguiente.
// Incluye un pulso que siga en LOW al cerrar la ventana.
float cerrarVentanaDSM(uint32_t& cantidadPulsos) {
  portENTER_CRITICAL(&muxDSM);

  uint32_t ahora = micros();

  if (estadoLow) {
    acumuladoLowUs += ahora - ultimoCambioUs;
  }

  uint32_t tiempoLow = acumuladoLowUs;
  uint32_t duracion = ahora - inicioVentanaUs;
  cantidadPulsos = pulsos;

  acumuladoLowUs = 0;
  pulsos = 0;
  ultimoCambioUs = ahora;
  inicioVentanaUs = ahora;

  portEXIT_CRITICAL(&muxDSM);

  return duracion > 0 ? 100.0f * tiempoLow / duracion : NAN;
}

float leerMQ9() {
  uint32_t sumaMv = 0;

  for (uint8_t i = 0; i < 16; i++) {
    sumaMv += analogReadMilliVolts(PIN_MQ9);
    delay(1);
  }

  float voltajeGPIO = sumaMv / 16000.0f;
  return voltajeGPIO * FACTOR_MQ9;
}

// Guarda una variable por peticion, como en la guia.
bool enviarLectura(const char* sensor, float valor) {
  if (!isfinite(valor)) {
    Serial.printf("%s: lectura invalida; no se envia.\n", sensor);
    return false;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.printf("%s: sin WiFi; lectura no enviada.\n", sensor);
    return false;
  }

  String clave(CLAVE_API);
  clave.trim();

  WiFiClientSecure cliente;
  // Modo de prueba de la guia: sin verificar certificado.
  cliente.setInsecure();
  cliente.setHandshakeTimeout(10);

  HTTPClient http;
  http.setConnectTimeout(10000);
  http.setTimeout(10000);

  if (!http.begin(cliente, URL_LECTURAS)) {
    Serial.println("No se pudo preparar HTTPS.");
    return false;
  }

  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-api-key", clave);

  JsonDocument documento;
  documento["dispositivo"] = "esp32-estacion";
  documento["sensor"] = sensor;
  documento["valor"] = valor;

  String cuerpo;
  serializeJson(documento, cuerpo);

  int codigo = http.POST(cuerpo);

  Serial.printf("%s: %.3f | POST -> %d\n", sensor, valor, codigo);

  if (codigo > 0) {
    Serial.println(http.getString());
  } else {
    Serial.println(HTTPClient::errorToString(codigo));
  }

  http.end();
  return codigo == 201;
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\nESTACION COMPLETA - VERSION 4");

  dht.begin();

  analogReadResolution(12);
  analogSetPinAttenuation(PIN_MQ9, ADC_11db);

  pinMode(PIN_DSM, INPUT);
  iniciarVentanaDSM();
  attachInterrupt(digitalPinToInterrupt(PIN_DSM), cambioDSM, CHANGE);

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(NOMBRE_WIFI, PASSWORD_WIFI);

  inicioMs = millis();
  ultimoIntentoWiFi = inicioMs;

  Serial.println("DSM501A: estabilizacion inicial de 60 segundos.");
}

void loop() {
  uint32_t ahoraMs = millis();

  // La medicion sigue aunque no haya WiFi.
  if (WiFi.status() != WL_CONNECTED &&
      ahoraMs - ultimoIntentoWiFi >= 20000) {
    ultimoIntentoWiFi = ahoraMs;
    Serial.println("Reintentando WiFi...");
    WiFi.reconnect();
  }

  if (!dsmListo && ahoraMs - inicioMs >= CALENTAMIENTO_MS) {
    iniciarVentanaDSM();
    dsmListo = true;
    Serial.println("DSM501A listo. Primera ventana de 30 segundos.");
  }

  if (dsmListo &&
      uint32_t(micros() - inicioVentanaUs) >= VENTANA_US) {
    uint32_t cantidadPulsos = 0;
    float ocupacionLow = cerrarVentanaDSM(cantidadPulsos);

    float temperatura = dht.readTemperature();
    float humedad = dht.readHumidity();
    float voltajeMQ9 = leerMQ9();

    Serial.printf("\nDSM: %lu pulsos iniciados en la ventana.\n",
                  (unsigned long)cantidadPulsos);

    uint8_t guardadas = 0;
    guardadas += enviarLectura("temperatura", temperatura);
    guardadas += enviarLectura("humedad", humedad);
    guardadas += enviarLectura("mq9_voltaje", voltajeMQ9);
    guardadas += enviarLectura("dsm_low", ocupacionLow);

    Serial.printf("Lecturas guardadas: %u de 4\n", guardadas);
  }

  delay(1);
}
