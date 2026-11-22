# Conexiones del sistema

El sistema utiliza una ESP32-S3 junto a los sensores DHT11, MQ-9 y DSM501A.

## DHT11

Conexiones:

- `+` → 3V3
- `S` → GPIO4
- `-` → GND

## MQ-9

El MQ-9 funciona con 5 V.

Conexiones:

- VCC → 5V
- GND → GND
- AO → GPIO5 usando divisor de voltaje
- DO → sin conectar

Para el divisor de voltaje usamos:

- 2 resistencias de 10 kΩ
- 1 resistencia de 4,7 kΩ

La conexión queda así:

```text
MQ-9 AO
   |
 10 kΩ
   |
   +---- GPIO5
   |
 10 kΩ
   |
 4,7 kΩ
   |
  GND
```

## DSM501A

Usamos los pines 3, 4 y 5 del sensor.

- Pin 1 → sin conectar
- Pin 2 → sin conectar
- Pin 3 → 5V
- Pin 4 → GPIO7 usando divisor de voltaje
- Pin 5 → GND

Para el divisor usamos:

- 2 resistencias de 10 kΩ
- 1 resistencia de 4,7 kΩ

La conexión queda así:

```text
DSM501A Pin 4
      |
    10 kΩ
      |
      +---- GPIO7
      |
    10 kΩ
      |
    4,7 kΩ
      |
     GND
```

## Alimentación

El MQ-9 y el DSM501A se alimentan desde 5V.

El DHT11 se alimenta desde 3V3.

Todos los componentes comparten GND.

## GPIO utilizados

| Componente | GPIO |
|---|---|
| DHT11 | GPIO4 |
| MQ-9 | GPIO5 |
| DSM501A | GPIO7 |

## Resistencias utilizadas

En total usamos:

- 4 resistencias de 10 kΩ
- 2 resistencias de 4,7 kΩ
