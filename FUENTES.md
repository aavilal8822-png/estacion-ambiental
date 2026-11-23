# FUENTES — Estación Ambiental IoT

**Curso:** TEI201, Taller de Diseño en Ingeniería — Universidad Adolfo Ibáñez  
**Equipo:** Antonia Ávila, Agustín Jiménez y Claudio Parada  
**Fecha de actualización:** 23 de noviembre de 2026

## 1. Propósito y alcance

Este documento declara las fuentes externas, bibliotecas, herramientas y asistencia de inteligencia artificial empleadas para desarrollar y documentar la estación ambiental. Su objetivo es distinguir los recursos consultados, las adaptaciones realizadas y el trabajo de integración y comprobación del equipo.

**Nota de verificación:** la lista definitiva de paquetes, versiones y fragmentos de código debe cotejarse con el archivo `.ino` de `firmware/`, `package.json`, `package-lock.json` y `server.js`. No se atribuyen versiones o fragmentos específicos sin comprobarlos en esos archivos.

## 2. Librerías y dependencias

## 2.1. Firmware de ESP32-S3

Librería	Versión	Uso en el proyecto	Fuente
Arduino core para ESP32	Por verificar	Programación de la ESP32-S3, lectura ADC, GPIO e interrupciones	Espressif
WiFi.h	Incluida en el core ESP32	Conexión a internet y reconexión automática	Espressif
WiFiClientSecure.h	Incluida en el core ESP32	Establecimiento de conexiones HTTPS con el servidor	Espressif
HTTPClient.h	Incluida en el core ESP32	Envío de mediciones al servidor de Railway mediante solicitudes POST	Espressif
ArduinoJson.h	Por verificar	Creación y serialización de datos JSON para enviarlos al servidor	ArduinoJson
DHT.h	Por verificar	Lectura de temperatura y humedad del DHT11	Adafruit DHT Sensor Library

Adaptaciones e implementación en el proyecto:

El firmware firmware/main.ino fue configurado específicamente para utilizar los tres sensores conectados directamente a la ESP32-S3.

* DHT11: conectado al GPIO4 para registrar temperatura y humedad.
* MQ-9: conectado al GPIO5 mediante un divisor de voltaje. Se promedian 16 lecturas analógicas y se estima el voltaje de salida, sin convertirlo directamente a ppm.
* DSM501A: conectado al GPIO7. Se utilizan interrupciones para contabilizar los pulsos y calcular el porcentaje de ocupación LOW en ventanas de 30 segundos.
* Wi-Fi: se implementó conexión a internet y reintentos de reconexión cada 20 segundos cuando se pierde la señal.
* Comunicación con Railway: las mediciones se organizan en JSON y se envían mediante solicitudes HTTPS POST a la API del proyecto.
* Validación básica: se evita enviar valores numéricos no finitos y se comprueba la respuesta HTTP del servidor.

Limitación de seguridad identificada: el código utiliza setInsecure() para las conexiones HTTPS de prueba, por lo que no verifica el certificado del servidor. Para una versión destinada a operar permanentemente se debe implementar la validación del certificado TLS.

Nota de versiones: las versiones exactas del core ESP32, ArduinoJson y la biblioteca DHT deben comprobarse en el entorno Arduino IDE utilizado para compilar el proyecto.

## 2.2. Backend Node.js

Librería	Versión declarada	Uso en el proyecto	Fuente oficial
dotenv	^16.4.5	Carga de variables de entorno para configurar el servidor y las credenciales de conexión.	dotenv
express	^4.21.0	Implementación del servidor HTTP, los endpoints de la API y el acceso al dashboard.	Express
pg	^8.13.0	Comunicación entre el servidor Node.js y la base de datos PostgreSQL.	node-postgres
Node.js	>=18	Entorno de ejecución del servidor alojado en Railway.	Node.js

Adaptación al proyecto: Estas dependencias forman parte del servidor desarrollado para la estación ambiental. Su función es permitir la configuración del backend, la comunicación mediante HTTP y el acceso a PostgreSQL para gestionar las mediciones ambientales.

Las versiones declaradas corresponden al archivo package.json. Las versiones instaladas deben verificarse en package-lock.json.

## 3. Material externo, ejemplos y documentación técnica

| Recurso | Fuente | Uso en el proyecto | Adaptación / aplicación |
|---|---|---|---|
| Documentación de ESP32-S3 y Arduino-ESP32 | https://docs.espressif.com/projects/arduino-esp32/en/latest/ | Referencia para periféricos, GPIO y conectividad | Adaptación de pines y adquisición a la placa ESP32-S3 N16R8 y a tres sensores conectados directamente |
| Guía de clase de despliegue con Railway y base de datos | Material entregado en TEI201 | Referencia de arquitectura IoT y almacenamiento | Se utiliza como orientación para separar captura con ESP32, servicio web y consulta de históricos; comprobar qué fragmentos se reutilizaron literalmente |
| Documentación de Railway | https://docs.railway.com/ | Configuración y despliegue de aplicación web | Aplicación al dashboard de la estación ambiental |
| Documentación de GitHub | https://docs.github.com/es | Organización del repositorio y Markdown | Estructura de carpetas, README y evidencias de entrega |
| Datasheets y documentación de cada sensor | Indicar URL específica del documento efectivamente consultado | Alimentación, pinout y limitaciones de DHT11, MQ-9 y DSM501A | Selección de niveles de tensión, división resistiva e interpretación de mediciones |

### Código externo incorporado

No es posible determinar únicamente desde los documentos de entrega qué bloques de `server.js` o del firmware fueron copiados o adaptados de ejemplos externos. **El equipo debe revisar el código y agregar aquí cada fragmento externo realmente empleado**, siguiendo este formato:

- **Archivo y función:** `[archivo y nombre de función o líneas]`
- **Fuente original:** `[URL del tutorial, repositorio o guía]`
- **Qué se reutilizó:** `[explicación concreta]`
- **Qué se modificó:** `[pines, campos, tiempos de espera, validación, etc.]`
- **Cómo se probó:** `[evidencia o prueba realizada]`

Si un bloque se desarrolló íntegramente por el equipo sin copiar un ejemplo externo, indicarlo expresamente tras verificarlo; no inventar referencias.

## 4. Uso de inteligencia artificial

**Herramienta declarada:** ChatGPT (OpenAI).  
**Período:** desarrollo del proyecto durante 2026, incluidos los materiales preparados en noviembre de 2026.

| Actividad asistida | Consulta o solicitud del equipo | Aporte de IA | Adaptación, comprensión y verificación |
|---|---|---|---|
| Diseño de conexiones de sensores | Consultas sobre pines de DHT11, MQ-9 y DSM501A en ESP32-S3 | Apoyo para explicar cableado, alimentación y divisores de tensión | El equipo debe contrastar el esquema con el montaje; se definieron GPIO4, GPIO5 y GPIO7 para los tres sensores y alimentación compatible |
| Arquitectura del prototipo | Consultas para pasar del montaje Arduino + ESP32 a una ESP32-S3 | Orientaciones de organización del firmware y de lectura directa | Adaptación a la arquitectura actual; la funcionalidad final debe justificarse mediante las pruebas del dispositivo |
| Tratamiento de los datos | Consultas sobre MQ-9, DSM501A y cómo interpretar sus lecturas | Apoyo para describir limitaciones y evitar expresar señales sin calibración como ppm o PM2.5 certificado | El equipo adoptó la interpretación relativa del MQ-9 y documentó la necesidad de validar el DSM501A |
| Modelado y documentación 3D | «me puedes hacer un código fusión en python de ese archivo porfa» y solicitudes posteriores de gemelo 3D | Apoyo con script Python, volúmenes representativos, modelo `.3mf`, renders y planos | Se integraron caja y tapa STL con batería, ESP32 y sensores; el formato `.3mf` no se declara como `.f3d` paramétrico validado |
| Documentación de pruebas | Solicitudes para redactar la Prueba 5 y los resultados del dashboard | Organización y redacción de objetivos, procedimientos, evidencias y criterios de éxito | Los 500 registros son una cifra reportada por el equipo; las estadísticas definitivas deben calcularse desde datos reales, no generarse por IA |
| Documentación GitHub | «hazme dnvo el coso de readme porfa y el de fuentes tb bien hecho y según la rúbrica porfa» | Elaboración de borradores estructurados para `README.md` y `FUENTES.md` | El equipo debe comprobar dependencias, rutas, versiones y funcionamiento real antes de entregar |

### Qué debe poder explicar el equipo

- **Adquisición:** cómo la ESP32-S3 lee GPIO4, GPIO5 y GPIO7, y por qué se protegen las entradas de 3,3 V.
- **Transmisión:** cómo se envían las mediciones por Wi-Fi y qué hace el backend en `server.js`.
- **Persistencia:** dónde quedan guardadas las mediciones y cómo se recupera su historial.
- **Visualización:** qué significa cada variable y qué conclusiones **sí** están respaldadas por datos.
- **Limitaciones:** por qué el MQ-9 se presenta como señal relativa y por qué las lecturas del DSM501A necesitan validación adicional.
- **Diseño 3D:** diferencia entre el modelo `.3mf` y el archivo nativo `.f3d` solicitado por la pauta.

## 5. Responsabilidad del equipo y trazabilidad

Las herramientas de IA se emplearon como apoyo para aprender, revisar, proponer código y preparar documentación. **La integración, ejecución, verificación experimental y presentación de resultados son responsabilidad del equipo.** Ninguna respuesta de IA sustituye las lecturas reales, el análisis de errores ni la validación física.

La evidencia experimental se documenta en [`protocolo_pruebas.md`](protocolo_pruebas.md), en los resultados de testing que se suban y en el dashboard: https://estacion-ambiental-production.up.railway.app.

**Revisión obligatoria antes de la entrega:** comprobar librerías y versiones efectivamente instaladas; identificar todo código externo adaptado; añadir las URL precisas de datasheets utilizados; contrastar las declaraciones anteriores con el código final publicado.
