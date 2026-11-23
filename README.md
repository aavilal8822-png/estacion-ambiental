# Estación Ambiental IoT

Proyecto desarrollado para el curso TEI201 - Taller de Diseño en Ingeniería.

## Descripción

Este proyecto consiste en una estación ambiental IoT diseñada para medir variables ambientales en distintos sectores urbanos de Santiago y comparar lugares con mayor y menor presencia de vegetación.

El sistema utiliza un ESP32 para recopilar datos de distintos sensores y enviarlos mediante Wi-Fi a una plataforma de almacenamiento y visualización.

## Integrantes

- Antonia Ávila
- Agustín Jiménez
- Claudio Parada

## Variables medidas

- Temperatura
- Humedad
- Señal del sensor MQ-9
- Material particulado mediante DSM501A

## Componentes principales

- ESP32-S3
- Sensor DHT11
- Sensor MQ-9
- Sensor DSM501A
- Protoboard
- Resistencias
- Cables Dupont
- Powerbank
- Carcasa impresa en 3D

## Funcionamiento

Los sensores recopilan información ambiental y el ESP32 procesa las mediciones.

Posteriormente, los datos son enviados mediante Wi-Fi a ThingSpeak, donde pueden almacenarse y visualizarse mediante gráficos.

## Dashboard

El dashboard del proyecto será implementado utilizando ThingSpeak.

Link al dashboard:

Pendiente de agregar.

## Estructura del repositorio

- `firmware/`: código del ESP32.
- `hardware/`: esquemático, BOM y documentación del circuito.
- `diseño-3d/`: archivos de Fusion 360, planos y renders.
- `testing/`: protocolo de pruebas, datos y evidencias.
- `docs/`: documentación final del proyecto.
- `FUENTES.md`: fuentes, librerías y herramientas de IA utilizadas.

## Estado del proyecto

El prototipo se encuentra funcionando correctamente. Los sensores realizan lecturas y el sistema está operativo.
