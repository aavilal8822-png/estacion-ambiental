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

Las dependencias del servidor fueron verificadas mediante los archivos package.json y package-lock.json del repositorio.

Librería	Versión verificada	Uso en el proyecto	Fuente oficial
dotenv	16.6.1	Carga de variables de entorno para configurar el servidor y las credenciales de conexión.	dotenv
express	4.22.3	Implementación del servidor HTTP, los endpoints de la API y el acceso al dashboard.	Express
pg	8.23.1	Comunicación entre el servidor Node.js y la base de datos PostgreSQL.	node-postgres
Node.js	>=18 (requisito declarado)	Entorno de ejecución del servidor desplegado en Railway.	Node.js

Adaptación al proyecto:

Estas dependencias se utilizaron para desarrollar el backend de la estación ambiental IoT, permitiendo recibir las mediciones enviadas por la ESP32-S3, gestionar su almacenamiento en PostgreSQL y poner los datos a disposición del dashboard web.

Verificación de versiones:

Las versiones de dotenv, express y pg corresponden a las registradas en package-lock.json. Este archivo permite mantener versiones reproducibles de las dependencias del servidor.

El proyecto declara Node.js 18 o superior como requisito de ejecución, pero el archivo no permite identificar la versión exacta utilizada en Railway.


## 3. Material externo, ejemplos y documentación técnica

Durante el desarrollo de la estación ambiental IoT se utilizaron recursos institucionales, documentación técnica, material académico y herramientas de inteligencia artificial para apoyar la selección de componentes, el diseño electrónico y la programación del sistema.

### 3.1. Design Engineering Center (DEC) — Universidad Adolfo Ibáñez

* Fuente institucional: https://dec.uai.cl
* Uso en el proyecto: El equipo acudió al DEC de la Universidad Adolfo Ibáñez para solicitar y obtener los componentes electrónicos necesarios para construir la estación ambiental.
* Consulta técnica: La información sobre las características, funcionamiento, alimentación, conexiones y aplicaciones de los sensores DHT11, MQ-9 y DSM501A fue consultada mediante ChatGPT (OpenAI).
* Aplicación: A partir de estas consultas, el equipo seleccionó e integró los sensores a la ESP32-S3, considerando sus funciones, requerimientos eléctricos y compatibilidad con el sistema.
* Verificación: Las recomendaciones se contrastaron con el montaje físico y las pruebas realizadas, según lo documentado en el protocolo de pruebas.

### 3.2. Documentación de ESP32-S3 y Arduino-ESP32

* Fuente: https://docs.espressif.com/projects/arduino-esp32/en/latest/
* Uso en el proyecto: Referencia técnica para la programación de la ESP32-S3, sus entradas y salidas, la lectura analógica y la conectividad Wi-Fi.
* Aplicación: Configuración del DHT11 en GPIO4, MQ-9 en GPIO5 y DSM501A en GPIO7, junto con la lectura de sensores y transmisión de información.
* Adaptación: El firmware fue organizado para utilizar directamente los tres sensores en una sola ESP32-S3, eliminando la arquitectura anterior que también utilizaba Arduino UNO.

### 3.3. Guía académica de TEI201 — Taller de Diseño en Ingeniería

* Fuente: Material de clases proporcionado por el equipo docente de la Universidad Adolfo Ibáñez.
* Uso en el proyecto: Orientación para el desarrollo del sistema IoT, incluyendo adquisición de datos, comunicación con servicios web, almacenamiento y visualización.
* Aplicación: Se tomó como referencia la arquitectura que conecta la ESP32-S3 con un servidor web y una base de datos.
* Adaptación: La arquitectura se aplicó a los sensores seleccionados y al sistema de monitoreo ambiental desarrollado por el equipo.

### 3.4. Documentación de Railway

* Fuente: https://docs.railway.com/
* Uso en el proyecto: Referencia para el despliegue y configuración de aplicaciones web.
* Aplicación: Utilización de Railway para alojar el servidor y dashboard de la estación ambiental.
* Resultado: La plataforma web permite consultar las mediciones ambientales registradas por el sistema.
* Dashboard: https://estacion-ambiental-production.up.railway.app

### 3.5. Documentación de GitHub

* Fuente: https://docs.github.com/es
* Uso en el proyecto: Organización, almacenamiento y documentación de los archivos del proyecto.
* Aplicación: Creación del repositorio con firmware, documentación de hardware, esquemáticos, modelos 3D, pruebas y declaración de fuentes.
* Repositorio: https://github.com/aavilal8822-png/estacion-ambiental

### 3.6. Código externo y adaptaciones realizadas

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

### 3.7. Información técnica de los sensores

Las características generales de los sensores fueron investigadas con apoyo de ChatGPT (OpenAI).

* DHT11: funcionamiento, alimentación y lectura de temperatura y humedad.
* MQ-9: alimentación, salida analógica, protección mediante divisor de voltaje y limitaciones para expresar concentraciones en ppm.
* DSM501A: alimentación, señal de salida, pulsos LOW y cálculo de ocupación durante una ventana de medición.

La información obtenida mediante inteligencia artificial se utilizó como apoyo para integrar los componentes y comprender sus limitaciones.

Las pruebas realizadas con el prototipo se documentan en protocolo_pruebas.md.

La información proporcionada por ChatGPT no reemplaza la consulta de fichas técnicas oficiales ni la calibración experimental necesaria para validar la precisión de las mediciones.

## 4. Uso de inteligencia artificial

Durante el desarrollo de la estación ambiental IoT se utilizó ChatGPT, desarrollado por OpenAI, como herramienta de apoyo para la investigación técnica, programación, resolución de problemas y elaboración de documentación.

Su utilización tuvo como objetivo complementar los conocimientos del equipo, facilitar la comprensión de los componentes electrónicos y apoyar la implementación de las distintas etapas del proyecto.

Herramienta utilizada: ChatGPT (OpenAI).
Período de utilización: 2026, durante el desarrollo del proyecto.
Áreas de aplicación: Electrónica, programación, diseño 3D, análisis de datos y documentación técnica.

### 4.1. Investigación y selección de componentes electrónicos

Uso de IA: Se realizaron consultas sobre el funcionamiento, las características técnicas y los requerimientos eléctricos de los sensores DHT11, MQ-9 y DSM501A, además de la placa ESP32-S3.

Aporte de la herramienta: ChatGPT proporcionó explicaciones sobre los principios de funcionamiento de los sensores, sus conexiones, alimentación y posibles aplicaciones en un sistema de monitoreo ambiental.

Aplicación al proyecto: La información se utilizó para orientar la selección de componentes, definir sus funciones y organizar la arquitectura electrónica de la estación.

Verificación: Las recomendaciones fueron contrastadas con las conexiones y pruebas prácticas realizadas durante la construcción del prototipo. Las especificaciones que requieren calibración o validación adicional se identificaron como limitaciones del sistema.

### 4.2. Programación y configuración del sistema

Uso de IA: Se utilizó ChatGPT como apoyo para comprender, desarrollar, revisar y adaptar el código de adquisición de datos de los sensores y su integración con la ESP32-S3.

Aporte de la herramienta: Se recibieron orientaciones relacionadas con la lectura de entradas analógicas y digitales, el procesamiento de señales, la conectividad Wi-Fi y la organización del firmware.

Adaptaciones realizadas:

* Configuración del DHT11 en GPIO4 para registrar temperatura y humedad.
* Configuración del MQ-9 en GPIO5, incorporando un divisor de voltaje para proteger la entrada analógica.
* Configuración del DSM501A en GPIO7, utilizando interrupciones para registrar los pulsos y calcular el porcentaje de ocupación LOW.
* Integración de los tres sensores directamente a la ESP32-S3, reemplazando la arquitectura inicial que también utilizaba Arduino UNO.
* Organización de las mediciones para su transmisión mediante Wi-Fi.

Comprensión del equipo: Se estudiaron las funciones principales del código, especialmente la adquisición de datos, el procesamiento de las señales y la comunicación del dispositivo.

Verificación: El funcionamiento debe respaldarse mediante los registros y resultados experimentales incluidos en la documentación de pruebas.

### 4.3. Comunicación, almacenamiento y visualización de datos

Uso de IA: Se realizaron consultas sobre alternativas de comunicación y almacenamiento para implementar un sistema IoT con acceso remoto a la información.

Aporte de la herramienta: Se proporcionaron explicaciones sobre la arquitectura de comunicación entre la ESP32-S3, un servidor web y una base de datos, así como sobre el uso de solicitudes HTTP y estructuras JSON.

Aplicación al proyecto: Se implementó una arquitectura que permite enviar las mediciones a un servidor desplegado en Railway y consultar la información mediante un dashboard web.

Adaptación: La estructura de comunicación se ajustó a las variables ambientales registradas por los tres sensores del prototipo.

Verificación: La plataforma dispone de un historial de aproximadamente 500 registros reportados por el equipo. La integridad, continuidad y precisión de las mediciones deben evaluarse mediante los datos y evidencias experimentales.

Dashboard: https://estacion-ambiental-production.up.railway.app

### 4.4. Diseño tridimensional y documentación del encapsulado

Uso de IA: Se utilizó ChatGPT como apoyo para la representación tridimensional del prototipo, la organización de sus componentes internos y la elaboración de documentación técnica.

Aporte de la herramienta: Se generaron propuestas de distribución, modelos tridimensionales simplificados, representaciones visuales y planos técnicos a partir de los archivos STL del encapsulado.

Adaptaciones realizadas:

* Ubicación de la batería portátil en el compartimiento izquierdo.
* Distribución de la ESP32-S3 en el compartimiento central.
* Ubicación del DSM501A en el sector superior derecho.
* Distribución del MQ-9 y DHT11 en los compartimientos correspondientes.
* Incorporación de volúmenes representativos de los componentes electrónicos en el modelo 3D.
* Elaboración de vistas y documentación dimensional de la caja y su tapa.

Verificación y limitaciones: El modelo ensamblado se visualizó en formato 3MF. Este formato no equivale a un archivo paramétrico nativo de Fusion 360 (.f3d), y la compatibilidad física final de los componentes requiere comprobación dimensional.

### 4.5. Pruebas experimentales y análisis de resultados

Uso de IA: ChatGPT se utilizó para estructurar el protocolo de pruebas, definir criterios de evaluación y apoyar la interpretación de los resultados obtenidos.

Aporte de la herramienta: Se propusieron procedimientos para evaluar individualmente los sensores, verificar su funcionamiento simultáneo y comprobar la transmisión y almacenamiento de información.

Aplicación al proyecto:

* Organización de las pruebas del DHT11, MQ-9 y DSM501A.
* Formulación de objetivos, procedimientos y criterios de éxito.
* Documentación de problemas de alimentación, conexiones y lectura de sensores.
* Identificación de limitaciones relacionadas con la calibración del MQ-9 y la interpretación de las mediciones del DSM501A.
* Preparación de la documentación de resultados y evidencias.

Verificación: Las mediciones utilizadas para evaluar el prototipo corresponden a registros experimentales. ChatGPT no se utilizó como fuente de datos simulados para sustituir resultados reales.

Los resultados y procedimientos se documentan en protocolo_pruebas.md y en los archivos de la carpeta testing/.

### 4.6. Elaboración y revisión de documentación técnica

Uso de IA: Se utilizó ChatGPT como herramienta de apoyo para organizar, redactar y revisar los documentos técnicos del proyecto, considerando los requisitos de evaluación establecidos para el Avance 3.

Documentos asistidos:

* README.md: descripción del proyecto, componentes, arquitectura e instrucciones generales.
* FUENTES.md: declaración de bibliotecas, documentación externa y uso de inteligencia artificial.
* protocolo_pruebas.md: estructuración de objetivos, procedimientos y criterios de éxito.
* Documentación de resultados experimentales.
* Planos técnicos y material de apoyo para la presentación.

Adaptación: Los documentos se organizaron de acuerdo con las características del prototipo y las exigencias académicas del curso TEI201.

Responsabilidad del equipo: Corresponde a los integrantes verificar la exactitud de las especificaciones, los resultados y las declaraciones incluidas en la documentación antes de su entrega.

### 4.7. Consideraciones sobre el uso responsable de inteligencia artificial

La inteligencia artificial se utilizó como una herramienta complementaria durante el desarrollo del proyecto y no como sustituto de la experimentación ni de la comprensión técnica del sistema.

El equipo reconoce que:

* Las respuestas generadas por IA pueden contener errores y requieren revisión.
* Los datos experimentales deben obtenerse directamente del prototipo.
* Las decisiones de implementación deben justificarse según los componentes y las condiciones reales de funcionamiento.
* Las propuestas de código deben comprenderse y verificarse antes de considerarse funcionales.
* Las especificaciones de los sensores y las mediciones que requieren calibración no deben presentarse como resultados certificados.
* El uso de herramientas externas debe declararse de manera transparente.

La integración del prototipo, la comprobación de su funcionamiento y la presentación de los resultados corresponden a la responsabilidad del equipo.

En conclusión, ChatGPT contribuyó principalmente como herramienta de consulta, asistencia técnica y apoyo documental, mientras que el proyecto se desarrolló mediante la integración de componentes electrónicos, programación, diseño y pruebas experimentales realizadas por el equipo.

## 5. Responsabilidad del equipo y trazabilidad

Las herramientas de IA se emplearon como apoyo para aprender, revisar, proponer código y preparar documentación. **La integración, ejecución, verificación experimental y presentación de resultados son responsabilidad del equipo.** Ninguna respuesta de IA sustituye las lecturas reales, el análisis de errores ni la validación física.

La evidencia experimental se documenta en [`protocolo_pruebas.md`](protocolo_pruebas.md), en los resultados de testing que se suban y en el dashboard: https://estacion-ambiental-production.up.railway.app.

**Revisión obligatoria antes de la entrega:** comprobar librerías y versiones efectivamente instaladas; identificar todo código externo adaptado; añadir las URL precisas de datasheets utilizados; contrastar las declaraciones anteriores con el código final publicado.
