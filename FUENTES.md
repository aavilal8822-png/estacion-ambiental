# FUENTES — Estación Ambiental IoT

**Curso:** TEI201, Taller de Diseño en Ingeniería — Universidad Adolfo Ibáñez  
**Equipo:** Antonia Ávila, Agustín Jiménez y Claudio Parada  
**Fecha de actualización:** 23 de noviembre de 2026

## 1. Propósito y alcance

Este documento declara las fuentes externas, bibliotecas, herramientas y asistencia de inteligencia artificial empleadas para desarrollar y documentar la estación ambiental. Su objetivo es distinguir los recursos consultados, las adaptaciones realizadas y el trabajo de integración y comprobación del equipo.

**Nota de verificación:** la lista definitiva de paquetes, versiones y fragmentos de código debe cotejarse con el archivo `.ino` de `firmware/`, `package.json`, `package-lock.json` y `server.js`. No se atribuyen versiones o fragmentos específicos sin comprobarlos en esos archivos.

## 2. Librerías y dependencias

### 2.1. Firmware de ESP32-S3

| Recurso | Versión | Uso previsto / documentado | Fuente | Confirmación pendiente |
|---|---|---|---|---|
| Arduino core para ESP32 (Espressif) | Revisar instalación de Arduino IDE | Programación de la placa y acceso a GPIO, ADC y Wi-Fi | https://github.com/espressif/arduino-esp32 | Confirmar versión instalada |
| Biblioteca Wi-Fi (`WiFi.h`, si se importa) | Incluida en el core | Conectividad de la ESP32-S3 | https://github.com/espressif/arduino-esp32 | Confirmar importación |
| Cliente HTTP (`HTTPClient.h`, si se importa) | Incluida en el core | Envío de mediciones al servidor | https://github.com/espressif/arduino-esp32 | Confirmar importación |
| Biblioteca DHT (si se utiliza `DHT.h` de Adafruit) | Verificar en Arduino IDE | Lectura de temperatura y humedad del DHT11 | https://github.com/adafruit/DHT-sensor-library | Confirmar nombre y versión |
| Adafruit Unified Sensor (si la biblioteca DHT lo requiere) | Verificar en Arduino IDE | Dependencia de lectura DHT | https://github.com/adafruit/Adafruit_Sensor | Confirmar uso |

### 2.2. Servidor y dashboard

| Dependencia / entorno | Versión | Uso | Fuente / cómo verificar |
|---|---|---|---|
| Node.js | Versión de ejecución por comprobar | Ejecutar `server.js` | https://nodejs.org/ |
| Paquetes npm | Versiones exactas registradas en `package-lock.json` | Servidor, API, acceso a datos y/o visualización según código | Revisar `package.json` y https://www.npmjs.com/ |
| Railway | Servicio de despliegue utilizado | Publicación del backend y dashboard web | https://docs.railway.com/ |
| Proveedor de base de datos | Por confirmar en configuración de despliegue | Persistencia del historial | Revisar `server.js` y variables de entorno (sin publicar credenciales) |

**Antes de entregar:** reemplazar «por comprobar» por las versiones y nombres exactos. Si se utilizan `express`, `pg`, `cors`, `dotenv` u otras bibliotecas, declarar **solo las que aparezcan realmente** en las dependencias y explicar su función.

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
