# Protocolo de pruebas

## Objetivo

Comprobar que la estación ambiental funciona correctamente y que los sensores entregan datos de forma estable.

## Prueba 1: DHT11

Objetivo: verificar lectura de temperatura y humedad.

Procedimiento:
- Encender el sistema.
- Revisar las lecturas en el monitor serie.
- Registrar varias mediciones.

Criterio de éxito:
- El sensor entrega valores de temperatura y humedad sin errores de lectura.

## Prueba 2: MQ-9

Objetivo: verificar que el sensor responda a cambios en el ambiente.

Procedimiento:
- Registrar el valor inicial del sensor.
- Observar si la lectura cambia frente a una variación de gases cercana al sensor.
- Comparar las lecturas antes y después.

Criterio de éxito:
- El valor entregado por el sensor cambia frente a una variación del ambiente.

Nota:
La lectura del MQ-9 se utilizará como valor relativo y no como una medición exacta en ppm.

## Prueba 3: DSM501A

Objetivo: verificar la lectura del sensor de material particulado.

Procedimiento:
- Dejar estabilizar el sensor.
- Registrar la ocupación LOW durante las ventanas de medición.
- Comparar varias mediciones consecutivas.

Criterio de éxito:
- El sensor entrega pulsos y valores de ocupación LOW sin permanecer constantemente en 0 % o 100 %.

## Prueba 4: funcionamiento conjunto

Objetivo: comprobar que los tres sensores funcionen al mismo tiempo.

Procedimiento:
- Conectar todos los sensores al ESP32-S3.
- Ejecutar el código principal.
- Observar las lecturas durante varios minutos.

Criterio de éxito:
- Los tres sensores entregan datos sin que el sistema se reinicie o deje de responder.


Criterio de éxito:
- Los datos aparecen correctamente en ThingSpeak y quedan registrados con fecha y hora.

## Prueba 5: transmisión y almacenamiento en ThingSpeak

Objetivo: verificar que las mediciones enviadas por la ESP32-S3 se almacenen correctamente en ThingSpeak.

Procedimiento:
- Conectar la ESP32-S3 a una red Wi-Fi con acceso a internet.
- Ejecutar el programa principal.
- Verificar que se envíen mediciones a ThingSpeak.
- Revisar que los registros incluyan fecha y hora.
- Comprobar que los datos anteriores continúen disponibles después de actualizar el dashboard.
- Comparar la cantidad de envíos realizados con los registros recibidos.

Criterio de éxito:
- Los datos aparecen correctamente en ThingSpeak y quedan almacenados con fecha y hora.
- El historial permanece accesible después de actualizar la página.
- Se registra la cantidad de envíos exitosos y fallidos.

Evidencia:
- Captura del dashboard.
- Exportación de datos en CSV.
- Registro de cantidad de envíos y errores.
  
## Evidencias

Se guardarán:

- Capturas del monitor serie.
- Capturas del dashboard.
- Datos exportados desde ThingSpeak.
- Fotografías del prototipo funcionando.

## Fallas encontradas

1. Problemas de alimentación usando solo la ESP32

Al comienzo del proyecto algunos sensores no funcionaban correctamente al conectarlos directamente a la ESP32, principalmente por diferencias de alimentación y niveles de voltaje.

Como solución, primero se probó una arquitectura con Arduino y ESP32 para separar algunas funciones. Después de revisar las conexiones y adaptar correctamente los niveles de voltaje, se logró volver a utilizar únicamente la ESP32-S3.

2. DHT11 defectuoso o inestable

Durante las primeras pruebas uno de los sensores DHT11 no entregaba lecturas correctas o presentaba errores frecuentes.

Se revisaron las conexiones y el código para descartar problemas de configuración. Finalmente, el sensor fue reemplazado y las lecturas de temperatura y humedad comenzaron a funcionar correctamente.

3. Interpretación de los datos del MQ-9

Al principio se esperaba obtener directamente una concentración de gas en ppm a partir del MQ-9.

Durante las pruebas se identificó que el sensor entrega principalmente una señal analógica y que para obtener concentraciones confiables es necesario realizar una calibración adecuada.

Por esta razón, en el prototipo se utiliza la señal del MQ-9 como un valor relativo y no como una medición exacta en ppm.

4. Alimentación y nivel de señal del MQ-9

También se presentaron dudas y problemas relacionados con la alimentación de 5 V del MQ-9 y con el voltaje de su salida analógica.

Para evitar entregar un voltaje demasiado alto a la entrada del ESP32, se incorporó un divisor de voltaje entre la salida AO del MQ-9 y el GPIO5.

Después de ajustar la alimentación y las conexiones, el sensor comenzó a entregar lecturas estables.
