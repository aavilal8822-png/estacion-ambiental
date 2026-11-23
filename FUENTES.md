# Estación Ambiental IoT — TEI201

**Taller de Diseño en Ingeniería | Universidad Adolfo Ibáñez | Avance 3 (23 de noviembre de 2026)**

**Integrantes:** Antonia Ávila, Agustín Jiménez y Claudio Parada.

**Repositorio:** https://github.com/aavilal8822-png/estacion-ambiental  
**Dashboard:** https://estacion-ambiental-production.up.railway.app

## 1. Problema y objetivo

Las condiciones ambientales pueden variar entre sectores de Santiago según el tránsito, la vegetación y otras características del entorno. Sin embargo, acceder a información local y comparable no siempre es sencillo.

El objetivo del proyecto es **construir una estación ambiental IoT de bajo costo** que registre variables del entorno, permita consultar su evolución temporal y sirva como base para futuras comparaciones entre ubicaciones. El prototipo es educativo: **no reemplaza una estación ambiental certificada**.

## 2. Solución y arquitectura

El sistema integra tres sensores conectados directamente a una **ESP32-S3 N16R8**, que adquiere las señales y utiliza Wi-Fi para comunicarse con la aplicación web.

```text
DHT11 (temperatura / humedad) ─┐
MQ-9  (señal relativa de gases) ├─> ESP32-S3 ──Wi-Fi──> Backend Node.js en Railway
DSM501A (ocupación de pulsos) ─┘                         │
                                                          ├─> Almacenamiento histórico*
                                                          └─> Dashboard web en Railway
```

*El proveedor y la configuración exacta de la base de datos deben comprobarse en `server.js` y las variables de entorno de despliegue antes de documentarlos por nombre.*

El dashboard se encuentra en: **https://estacion-ambiental-production.up.railway.app**. Durante las pruebas el equipo reportó aproximadamente **500 registros** disponibles para consulta. El detalle y análisis de las mediciones deben respaldarse en los archivos de testing.

## 3. Hardware

| Elemento | Función |
|---|---|
| ESP32-S3 N16R8 | Control, adquisición y comunicación Wi-Fi |
| DHT11 | Temperatura (°C) y humedad relativa (%) |
| MQ-9 | Señal analógica relacionada con presencia de ciertos gases; uso relativo, sin calibración en ppm |
| DSM501A | Salida de pulsos asociada a partículas; ocupación LOW, sin validación como concentración PM2.5 |
| Protoboard, cables y resistencias | Interconexión y adaptación de niveles de tensión |
| Powerbank 10.000 mAh | Alimentación USB de 5 V |
| Caja y tapa impresas en 3D | Protección, ventilación y acceso a componentes |

### Conexiones principales

| Componente | Alimentación | Señal hacia ESP32-S3 |
|---|---|---|
| DHT11 (módulo) | 3V3 y GND | DATA → GPIO4 |
| MQ-9 (módulo) | 5 V y GND | AO → divisor resistivo → GPIO5 |
| DSM501A | 5 V y GND | Vout1 → divisor resistivo → GPIO7 |

Cada divisor indicado utiliza **10 kΩ en la rama superior y 10 kΩ + 4,7 kΩ en serie en la inferior**. La ESP32 y los sensores comparten GND. No conectar señales de 5 V directamente a los GPIO de 3,3 V.

Documentación: [esquemático y conexiones](hardware/) · [BOM y costos](hardware/BOM.md).

## 4. Software y visualización

- **Firmware ESP32-S3:** lectura y preparación de los datos de los sensores. Revisar los archivos de [`firmware/`](firmware/) para consultar la versión definitiva y la frecuencia de muestreo implementada.
- **Backend:** aplicación Node.js en `server.js`, con dependencias definidas en `package.json` y `package-lock.json`.
- **Despliegue web:** Railway, con dashboard público e historial de mediciones.
- **Análisis:** las lecturas permiten observar variaciones en el tiempo; los gases y las partículas **no se presentan como concentraciones certificadas**.

## 5. Cómo utilizar o reproducir el proyecto

### A. Consultar las mediciones

1. Abrir el [dashboard de la estación ambiental](https://estacion-ambiental-production.up.railway.app).
2. Revisar las variables y los registros históricos disponibles.
3. Contrastar la información con las evidencias guardadas en el repositorio.

### B. Preparar el dispositivo

1. Montar los sensores de acuerdo con el [esquemático](hardware/) y las [conexiones](hardware/conexiones.md).
2. Instalar Arduino IDE y el soporte de placas ESP32 de Espressif.
3. Abrir el archivo `.ino` definitivo de la carpeta `firmware/` y seleccionar la placa ESP32-S3 correspondiente.
4. Instalar las librerías que importe ese archivo, según [`FUENTES.md`](FUENTES.md).
5. Configurar **localmente** la red Wi-Fi y la dirección de la API con los nombres de variables definidos en el firmware.
6. Cargar el programa y comprobar las lecturas con el monitor serie y el dashboard.

**Seguridad:** no publicar contraseñas Wi-Fi, tokens, claves de API ni credenciales de base de datos en GitHub.

### C. Ejecutar el servidor localmente

Con Node.js instalado y desde la raíz del repositorio:

```bash
npm ci
node server.js
```

Antes de iniciar, configurar las variables de entorno requeridas por `server.js` (incluidas las de conexión a la base de datos, si corresponde). En Railway se configuran como variables privadas del servicio. El comando mostrado sirve como punto de partida; el despliegue real depende de la configuración del proyecto.

## 6. Diseño 3D y mantenimiento

La carcasa distribuye la **batería a la izquierda**, la **ESP32-S3 al centro**, y los sensores a la derecha: **DSM501A arriba**, **MQ-9 en el alojamiento cuadrado** y **DHT11 en el sector circular**. Se consideran ventilación, paso de cables y acceso para mantenimiento.

En [`diseno-3d/modelo/`](diseno-3d/modelo/) se incluyen los STL de caja y tapa, renders y planos técnicos en PDF. También se desarrolló un ensamble de referencia en formato `.3mf` y el archivo de fusion en formato .f3d

## 7. Pruebas y limitaciones

El [protocolo de pruebas](protocolo_pruebas.md) contempla lectura de cada sensor, funcionamiento simultáneo, transmisión al dashboard y revisión del historial. Se documentaron dificultades de alimentación y niveles de señal, sustitución de un DHT11 defectuoso y la necesidad de validar lecturas atípicas del DSM501A.

Los aproximadamente 500 registros reportados **no demuestran por sí solos precisión ni ausencia de errores**. Para validar el prototipo se deben presentar datos cuantitativos, fechas, registros válidos, errores y análisis de fallas.

## 8. Archivos y documentación

| Recurso | Ubicación |
|---|---|
| Firmware | [`firmware/`](firmware/) |
| Esquemático, conexiones y materiales | [`hardware/`](hardware/) |
| Caja, tapa, renders y planos | [`diseno-3d/`](diseno-3d/) |
| Protocolo de pruebas | [`protocolo_pruebas.md`](protocolo_pruebas.md) |
| Resultados cuantitativos | `testing.md` |
| Presentación | `Estación ecológica.pptx` |
| Fuentes e inteligencia artificial | [`FUENTES.md`](FUENTES.md) |

## 9. Trabajo futuro

- Validar el DSM501A y caracterizar la estabilidad de las lecturas.
- Calibrar sensores con instrumentos de referencia si se requieren concentraciones absolutas.
- Comparar mediciones en distintos sectores de Santiago con procedimientos equivalentes.
- Documentar tolerancias de fabricación y, cuando esté disponible, el archivo nativo de Fusion 360.

---

**Transparencia académica:** las fuentes, librerías, herramientas y contribuciones de IA se documentan en [`FUENTES.md`](FUENTES.md).

