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

Design Engineering Center (DEC) — Universidad Adolfo Ibáñez

* Fuente institucional: https://dec.uai.cl
* Uso en el proyecto: El equipo acudió al DEC de la Universidad Adolfo Ibáñez para solicitar y obtener los componentes electrónicos necesarios para construir la estación ambiental.
* Consulta técnica: La información sobre las características, funcionamiento, alimentación, conexiones y aplicaciones de los sensores DHT11, MQ-9 y DSM501A fue consultada mediante ChatGPT (OpenAI).
* Aplicación: Con esta información, el equipo seleccionó e integró los sensores a la ESP32-S3, considerando sus funciones dentro del sistema y los requerimientos de alimentación y conexión.
* Verificación: Las recomendaciones obtenidas mediante IA se contrastaron con las pruebas y el montaje físico realizados por el equipo, en la medida documentada en el protocolo de pruebas.
### Código externo incorporado

No es posible determinar únicamente desde los documentos de entrega qué bloques de `server.js` o del firmware fueron copiados o adaptados de ejemplos externos. **El equipo debe revisar el código y agregar aquí cada fragmento externo realmente empleado**, siguiendo este formato:

- **Archivo y función:** `[archivo y nombre de función o líneas]`
- **Fuente original:** `[URL del tutorial, repositorio o guía]`
- **Qué se reutilizó:** `[explicación concreta]`
- **Qué se modificó:** `[pines, campos, tiempos de espera, validación, etc.]`
- **Cómo se probó:** `[evidencia o prueba realizada]`

Si un bloque se desarrolló íntegramente por el equipo sin copiar un ejemplo externo, indicarlo expresamente tras verificarlo; no inventar referencias.

## 3. Material externo, ejemplos y documentación técnica

Durante el desarrollo de la estación ambiental IoT se utilizaron recursos institucionales, documentación técnica, material académico y herramientas de inteligencia artificial para apoyar la selección de componentes, el diseño electrónico y la programación del sistema.

3.1. Design Engineering Center (DEC) — Universidad Adolfo Ibáñez

* Fuente institucional: https://dec.uai.cl
* Uso en el proyecto: El equipo acudió al DEC de la Universidad Adolfo Ibáñez para solicitar y obtener los componentes electrónicos necesarios para construir la estación ambiental.
* Consulta técnica: La información sobre las características, funcionamiento, alimentación, conexiones y aplicaciones de los sensores DHT11, MQ-9 y DSM501A fue consultada mediante ChatGPT (OpenAI).
* Aplicación: A partir de estas consultas, el equipo seleccionó e integró los sensores a la ESP32-S3, considerando sus funciones, requerimientos eléctricos y compatibilidad con el sistema.
* Verificación: Las recomendaciones se contrastaron con el montaje físico y las pruebas realizadas, según lo documentado en el protocolo de pruebas.

3.2. Documentación de ESP32-S3 y Arduino-ESP32

* Fuente: https://docs.espressif.com/projects/arduino-esp32/en/latest/
* Uso en el proyecto: Referencia técnica para la programación de la ESP32-S3, sus entradas y salidas, la lectura analógica y la conectividad Wi-Fi.
* Aplicación: Configuración del DHT11 en GPIO4, MQ-9 en GPIO5 y DSM501A en GPIO7, junto con la lectura de sensores y transmisión de información.
* Adaptación: El firmware fue organizado para utilizar directamente los tres sensores en una sola ESP32-S3, eliminando la arquitectura anterior que también utilizaba Arduino UNO.

3.3. Guía académica de TEI201 — Taller de Diseño en Ingeniería

* Fuente: Material de clases proporcionado por el equipo docente de la Universidad Adolfo Ibáñez.
* Uso en el proyecto: Orientación para el desarrollo del sistema IoT, incluyendo adquisición de datos, comunicación con servicios web, almacenamiento y visualización.
* Aplicación: Se tomó como referencia la arquitectura que conecta la ESP32-S3 con un servidor web y una base de datos.
* Adaptación: La arquitectura se aplicó a los sensores seleccionados y al sistema de monitoreo ambiental desarrollado por el equipo.

3.4. Documentación de Railway

* Fuente: https://docs.railway.com/
* Uso en el proyecto: Referencia para el despliegue y configuración de aplicaciones web.
* Aplicación: Utilización de Railway para alojar el servidor y dashboard de la estación ambiental.
* Resultado: La plataforma web permite consultar las mediciones ambientales registradas por el sistema.
* Dashboard: https://estacion-ambiental-production.up.railway.app

3.5. Documentación de GitHub

* Fuente: https://docs.github.com/es
* Uso en el proyecto: Organización, almacenamiento y documentación de los archivos del proyecto.
* Aplicación: Creación del repositorio con firmware, documentación de hardware, esquemáticos, modelos 3D, pruebas y declaración de fuentes.
* Repositorio: https://github.com/aavilal8822-png/estacion-ambiental

3.6. Código externo y adaptaciones realizadas

Para desarrollar el software se contó con el material académico del curso, documentación de bibliotecas y asistencia de ChatGPT.

Las principales implementaciones documentadas en el proyecto son:

Firmware — firmware/main.ino

* Lectura de temperatura y humedad mediante DHT11.
* Lectura analógica del MQ-9 y aplicación del factor de corrección del divisor de voltaje.
* Medición del porcentaje de ocupación LOW del DSM501A mediante interrupciones.
* Conexión Wi-Fi y mecanismo de reconexión.
* Construcción de mensajes JSON y envío HTTPS al servidor de Railway.

Servidor — server.js

* Implementación del servidor mediante Node.js y Express.
* Comunicación con PostgreSQL mediante la biblioteca pg.
* Recepción de las mediciones enviadas por la ESP32-S3.
* Disponibilidad de los datos para su consulta desde el dashboard.

Adaptaciones al proyecto

Las implementaciones se ajustaron a las características de la estación ambiental, sus tres sensores, los pines seleccionados y la estructura de los datos enviados al servidor.

La asistencia de ChatGPT utilizada en el desarrollo y documentación se declara en la sección 4 de este archivo.

Trazabilidad del código: La guía del curso y las bibliotecas utilizadas se identifican como recursos de apoyo. Para cualquier fragmento copiado o adaptado literalmente desde una fuente externa, corresponde registrar su procedencia exacta y los cambios realizados. La descripción anterior no implica que todas las funciones hayan sido escritas originalmente por el equipo ni que todas provengan de una misma fuente.

3.7. Información técnica de los sensores

Las características generales de los sensores fueron investigadas con apoyo de ChatGPT (OpenAI).

* DHT11: funcionamiento, alimentación y lectura de temperatura y humedad.
* MQ-9: alimentación, salida analógica, protección mediante divisor de voltaje y limitaciones para expresar concentraciones en ppm.
* DSM501A: alimentación, señal de salida, pulsos LOW y cálculo de ocupación durante una ventana de medición.

La información obtenida mediante inteligencia artificial se utilizó como apoyo para integrar los componentes y comprender sus limitaciones.

Las pruebas realizadas con el prototipo se documentan en protocolo_pruebas.md.

La información proporcionada por ChatGPT no reemplaza la consulta de fichas técnicas oficiales ni la calibración experimental necesaria para validar la precisión de las mediciones.

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
