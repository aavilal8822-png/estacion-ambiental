#include <Arduino.h>
#include <DHT.h>

constexpr uint8_t PIN_DHT = 4;
constexpr uint8_t PIN_MQ9 = 5;
constexpr uint8_t PIN_DSM = 7;

DHT dht(PIN_DHT, DHT11);

// Divisor del MQ-9:
// AO -> 10 kohm -> GPIO5 -> 14,7 kohm -> GND
constexpr float FACTOR_MQ9 = (10.0f + 14.7f) / 14.7f;

constexpr uint32_t INTERVALO_LECTURA_MS = 2500;
constexpr uint32_t CALENTAMIENTO_DSM_MS = 60000;
constexpr uint32_t VENTANA_DSM_US = 30000000;

portMUX_TYPE muxDSM = portMUX_INITIALIZER_UNLOCKED;

volatile bool dsmEnLow = false;
volatile uint32_t ultimoCambioUs = 0;
volatile uint32_t tiempoLowUs = 0;
volatile uint32_t pulsosLow = 0;

uint32_t inicioMs = 0;
uint32_t ultimaLecturaMs = 0;
uint32_t inicioVentanaUs = 0;
bool dsmListo = false;

// Registra ambos flancos sin bloquear el programa esperando pulsos.
void ARDUINO_ISR_ATTR cambioDSM() {
  portENTER_CRITICAL_ISR(&muxDSM);

  uint32_t ahora = micros();
  bool nuevoLow = (digitalRead(PIN_DSM) == LOW);

  if (dsmEnLow) {
    tiempoLowUs += ahora - ultimoCambioUs;
  }

  if (nuevoLow && !dsmEnLow) {
    pulsosLow++;
  }

  dsmEnLow = nuevoLow;
  ultimoCambioUs = ahora;

  portEXIT_CRITICAL_ISR(&muxDSM);
}

void iniciarVentanaDSM() {
  portENTER_CRITICAL(&muxDSM);

  uint32_t ahora = micros();
  tiempoLowUs = 0;
  pulsosLow = 0;
  dsmEnLow = (digitalRead(PIN_DSM) == LOW);
  ultimoCambioUs = ahora;
  inicioVentanaUs = ahora;

  portEXIT_CRITICAL(&muxDSM);
}

void mostrarDSM() {
  uint32_t acumulado;
  uint32_t pulsos;
  uint32_t duracion;

  portENTER_CRITICAL(&muxDSM);

  uint32_t ahora = micros();

  // Incluye la parte del pulso que cruza el final de la ventana.
  if (dsmEnLow) {
    tiempoLowUs += ahora - ultimoCambioUs;
  }

  acumulado = tiempoLowUs;
  pulsos = pulsosLow;
  duracion = ahora - inicioVentanaUs;

  tiempoLowUs = 0;
  pulsosLow = 0;
  ultimoCambioUs = ahora;
  inicioVentanaUs = ahora;

  portEXIT_CRITICAL(&muxDSM);

  float porcentajeLow = 100.0f * acumulado / duracion;

  Serial.println("\n--- DSM501A: ventana de medicion ---");
  Serial.printf("Duracion: %.2f s\n", duracion / 1000000.0f);
  Serial.printf("Tiempo en LOW: %.2f ms\n", acumulado / 1000.0f);
  Serial.printf("Ocupacion LOW: %.3f %%\n", porcentajeLow);
  Serial.printf("Pulsos iniciados: %lu\n", (unsigned long)pulsos);

  if (porcentajeLow > 99.0f) {
    Serial.println(
      "Aviso: senal casi siempre LOW; revisar cableado y niveles."
    );
  }

  Serial.println("-----------------------------------\n");
}

void mostrarDHTyMQ9() {
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

  Serial.println("--- Lecturas ambientales ---");

  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println("DHT11: error de lectura.");
  } else {
    Serial.printf("Temperatura: %.1f C\n", temperatura);
    Serial.printf("Humedad: %.1f %%\n", humedad);
  }

  // Promedio de varias lecturas calibradas del ADC.
  uint32_t sumaMv = 0;
  constexpr uint8_t MUESTRAS = 16;

  for (uint8_t i = 0; i < MUESTRAS; i++) {
    sumaMv += analogReadMilliVolts(PIN_MQ9);
    delay(1);
  }

  float voltajeGPIO = sumaMv / (1000.0f * MUESTRAS);
  float voltajeAO = voltajeGPIO * FACTOR_MQ9;

  Serial.printf("MQ-9, voltaje GPIO5: %.3f V\n", voltajeGPIO);
  Serial.printf("MQ-9, voltaje AO estimado: %.3f V\n", voltajeAO);

  if (!dsmListo) {
    Serial.println("DSM501A: estabilizando durante 60 segundos.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();

  analogReadResolution(12);
  analogSetPinAttenuation(PIN_MQ9, ADC_11db);

  // Sin pull-up interno: la senal llega desde el divisor.
  pinMode(PIN_DSM, INPUT);

  iniciarVentanaDSM();

  attachInterrupt(
    digitalPinToInterrupt(PIN_DSM),
    cambioDSM,
    CHANGE
  );

  inicioMs = millis();
  ultimaLecturaMs = inicioMs;

  Serial.println("\nEstacion ambiental ESP32-S3");
  Serial.println("DHT11: GPIO4 | MQ-9: GPIO5 | DSM501A: GPIO7");
  Serial.println("MQ-9: lectura de voltaje, sin conversion a ppm.");
}

void loop() {
  uint32_t ahoraMs = millis();

  if (!dsmListo &&
      ahoraMs - inicioMs >= CALENTAMIENTO_DSM_MS) {
    iniciarVentanaDSM();
    dsmListo = true;
    Serial.println("\nDSM501A listo. Comenzando ventana de 30 s.");
  }

  if (dsmListo &&
      uint32_t(micros() - inicioVentanaUs) >= VENTANA_DSM_US) {
    mostrarDSM();
  }

  if (ahoraMs - ultimaLecturaMs >= INTERVALO_LECTURA_MS) {
    ultimaLecturaMs = ahoraMs;
    mostrarDHTyMQ9();
  }

  delay(1);
}
