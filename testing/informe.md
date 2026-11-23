# Análisis de registros — Estación Ambiental IoT

**Fecha de los registros:** 23 de noviembre de 2026  
**Origen:** API pública del dashboard de la estación ambiental  
**Dashboard:** https://estacion-ambiental-production.up.railway.app  
**Muestra analizada:** 50 registros proporcionados por el equipo, identificadores 483–532  
**Datos:** [`datos/mediciones_estacion_ambiental_50_registros.csv`](datos/mediciones_estacion_ambiental_50_registros.csv)

## 1. Objetivo

Documentar las lecturas recibidas y almacenadas por el servidor, resumir el comportamiento de cada variable e identificar anomalías que requieran verificación.

## 2. Resumen cuantitativo

| Variable | Registros | Promedio | Mínimo | Máximo |
|---|---:|---:|---:|---:|
| Temperatura (°C) | 12 | 23,97 | 23,8 | 24,2 |
| Humedad relativa (%) | 12 | 56,08 | 55 | 57 |
| MQ-9: voltaje registrado (V) | 13 | 0 | 0 | 0 |
| DSM501A: ocupación LOW (%) | 13 | 100 | 100 | 100 |
| **Total** | **50** | — | — | — |

Los datos se registraron entre `2026-11-23T16:59:01.052Z` y `2026-11-23T17:05:02.505Z` (horarios UTC), según el campo `creado_en` de la API.

## 3. Interpretación y limitaciones

- La temperatura osciló entre 23,8 °C y 24,2 °C y la humedad relativa entre 55 % y 57 % durante la muestra observada.
- Las 13 lecturas del **MQ-9 son exactamente 0 V**. Porque no fue posible la calibración.
- Las 13 lecturas del **DSM501A son exactamente 100 % de ocupación LOW**.
- La existencia de registros con marcas de tiempo evidencia almacenamiento en la plataforma.
- **Esta tabla contiene 50 registros de un conjunto mayor reportado por el equipo; no se analizaron los 500 registros completos.**

## 4. Próximas verificaciones

1. Exportar el conjunto completo de registros y repetir el análisis sobre toda la muestra.
2. Comprobar el MQ-9 con una señal analógica no nula y verificar su lectura en el monitor serie.
3. Revisar las señales de salida del DSM501A y registrar pulsos durante varias ventanas de 30 segundos.
4. Contrastar las lecturas de DHT11 con un instrumento de referencia, si está disponible.

## 5. Conclusión

La muestra permite demostrar que se almacenaron registros con identificador, sensor, valor y fecha. Las lecturas de temperatura y humedad presentaron variaciones pequeñas; en contraste, los valores constantes del MQ-9 y DSM501A identifican posibles problemas de adquisición o interpretación que deben resolverse para obtener mediciones ambientales confiables. La validación de diferencias entre sectores de Santiago requerirá mediciones en múltiples ubicaciones y condiciones comparables.
