# Resultados de pruebas — Estación Ambiental IoT

## 1\. Objetivo

Evaluar el funcionamiento de la estación ambiental mediante el registro y almacenamiento de las mediciones obtenidas por los sensores DHT11, MQ\-9 y DSM501A, conectados a la ESP32\-S3\.

## 2\. Plataforma de almacenamiento y visualización

Para visualizar y consultar los datos se utiliza un dashboard web desplegado en Railway\.

**Enlace al dashboard:**
https://estacion\-ambiental\-production\.up\.railway\.app

La plataforma permite consultar las mediciones registradas por el sistema y analizar su comportamiento a lo largo del tiempo\.

## 3\. Datos obtenidos

Durante las pruebas del prototipo se reunieron aproximadamente 500 registros, según el conjunto de datos disponible en el dashboard\.

|Parámetro                  |Descripción                            |
|---------------------------|---------------------------------------|
|Registros disponibles      |Aproximadamente 500                    |
|Microcontrolador           |ESP32-S3                               |
|Sensor DHT11               |Temperatura y humedad                  |
|Sensor MQ-9                |Señal analógica relativa a gases       |
|Sensor DSM501A             |Señal asociada a partículas suspendidas|
|Plataforma de visualización|Railway                                |
|Fecha de revisión          |23 de noviembre de 2026                |

Los registros constituyen evidencia del proceso de adquisición y visualización de información ambiental realizado durante el desarrollo del prototipo\.

## 4\. Análisis de resultados

La disponibilidad de aproximadamente 500 registros permite disponer de un conjunto de datos para estudiar el comportamiento de las variables monitoreadas\.

Las mediciones de temperatura y humedad permiten observar cambios en las condiciones ambientales durante el período registrado\.

En el caso del MQ\-9, los valores se interpretan como señales relativas, debido a que no se realizó una calibración con concentraciones conocidas de gases\.

En el caso del DSM501A, las mediciones requieren validación adicional antes de interpretarlas como concentraciones exactas de material particulado\.

La existencia de registros históricos permite avanzar desde la visualización de valores individuales hacia el análisis de tendencias ambientales\.

## 5\. Limitaciones

- La cantidad de registros almacenados no demuestra por sí sola que todas las mediciones sean correctas\.
- No se ha establecido una precisión experimental mediante comparación con instrumentos certificados\.
- Las señales del MQ\-9 y del DSM501A no deben interpretarse directamente como concentraciones ambientales certificadas\.
- Se requiere analizar los registros para identificar posibles valores anómalos, interrupciones o errores\.

## 6\. Relación con el problema original

El proyecto busca desarrollar una alternativa accesible para monitorear condiciones ambientales locales en distintos sectores de Santiago\.

El registro de mediciones permite evaluar la capacidad del prototipo para recopilar información ambiental que podría utilizarse posteriormente para comparar diferentes ubicaciones y horarios\.

Para validar diferencias reales entre comunas será necesario realizar campañas de medición en distintos lugares y bajo condiciones comparables\.

## 7\. Evidencias

- [Dashboard web de la estación ambiental](https://estacion-ambiental-production.up.railway.app)
- Captura del dashboard con las mediciones\.
- Captura de registro histórico de datos\.

## 8\. Conclusión

El registro de aproximadamente 500 mediciones constituye un avance en la implementación del sistema de monitoreo ambiental\.

El prototipo integra sensores, adquisición de datos y visualización web, proporcionando una base para estudiar cambios en las variables monitoreadas\.

Como mejoras futuras se propone fortalecer la validación de los sensores, cuantificar la estabilidad del sistema y realizar mediciones en diferentes sectores de Santiago\.
