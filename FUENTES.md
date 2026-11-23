# FUENTES.md

## 1. Librerías y dependencias utilizadas

### WiFi.h
- Uso: conexión del ESP32-S3 a una red Wi-Fi.
- Fuente: documentación oficial de Espressif para ESP32.

### DHT Sensor Library
- Uso: lectura de temperatura y humedad mediante el sensor DHT11.
- Fuente: Adafruit DHT Sensor Library.
- Versión: pendiente de verificar en Arduino IDE.

### ThingSpeak
- Uso: envío, almacenamiento y visualización de los datos obtenidos por el ESP32.
- Fuente: documentación oficial de MathWorks ThingSpeak.
- Versión: pendiente de verificar.

## 2. Código y documentación externa utilizada

### Sensor DHT11

Se utilizaron ejemplos de la librería DHT como referencia para realizar la lectura de temperatura y humedad.

El código fue adaptado al ESP32-S3 y al pin utilizado en nuestro circuito.

### Sensor MQ-9

Se utilizó la documentación técnica del sensor MQ-9 y ejemplos de lectura analógica como referencia.

El código fue adaptado para leer la señal del sensor mediante el ESP32.

La señal del MQ-9 se utiliza como una medición relativa. No se presentan concentraciones exactas en ppm debido a que el sensor no ha sido calibrado utilizando una concentración de gas patrón.

### Sensor DSM501A

Se utilizó la documentación técnica del sensor DSM501A como referencia para interpretar la señal del sensor de material particulado.

El código fue adaptado al ESP32-S3 y al montaje realizado por el equipo.


## 3. Uso de Inteligencia Artificial

### ChatGPT

Se utilizó ChatGPT durante el desarrollo del proyecto como herramienta de apoyo.

Principales usos:

- Ayuda para comprender el funcionamiento de los sensores.
- Apoyo para realizar las conexiones entre sensores y ESP32-S3.
- Revisión de niveles de voltaje y divisores resistivos.
- Corrección de códigos de prueba.
- Apoyo conceptual para el diseño del encapsulado 3D.

El código y las recomendaciones generadas mediante inteligencia artificial fueron revisadas, modificadas y adaptadas al hardware real utilizado por el equipo.

## 4. Adaptaciones realizadas por el equipo

El equipo realizó las siguientes adaptaciones:

- Configuración de los GPIO utilizados por cada sensor.
- Adaptación de las conexiones al ESP32-S3.
- Uso de divisores resistivos para proteger las entradas del ESP32.
- Pruebas individuales de cada sensor antes de realizar la integración.
- Integración de las lecturas en un único sistema.
- Ajuste del código según los errores encontrados durante las pruebas.
- Diseño del encapsulado de acuerdo con las dimensiones reales de los componentes.

## 5. Fuentes técnicas principales

- Espressif Systems - Documentación oficial ESP32.
- Adafruit - DHT Sensor Library.
- Datasheet del sensor MQ-9.
- Datasheet del sensor DSM501A.

## 6. Declaración

Las fuentes externas, librerías y herramientas de inteligencia artificial utilizadas durante el desarrollo del proyecto fueron empleadas como apoyo.

El equipo comprende el funcionamiento general del código utilizado y realizó las modificaciones necesarias para adaptarlo al prototipo desarrollado.
