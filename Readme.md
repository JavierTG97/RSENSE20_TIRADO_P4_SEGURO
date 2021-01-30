# Práctica 4: Comunicaciones Wi-Fi y Stack IP

*Clonación del anterior repositorio de la P4, pero sin contraseñas.*

### *Redes de sensores Electrónicos*

**Apartados de la práctica** (cada uno tiene un folder asociado, salvo los apartados 1 y 3 (añadidos en el cuaderno de laboratorio)):

1. Conéctate a la red wifi del laboratorio o a una creada por el móvil como punto de acceso. Extrae tu IP. Comprueba la conectividad con Google mediante un ping
2. Pon en hora el módulo mediante un servidor NTP
3. Monta un chat con una aplicación software PC ([sockettest](http://sockettest.sourceforge.net/))
4. Después sustituye uno de los extremos por el módulo hardware siendo cliente y envía cada segundo tu hora local
5. Añadir una capa de control de tal modo que cuando se le mande “start” empiece a mandar la hora hasta que se le mande “stop”.
6. Monta un servidor WEB que muestre la hora y tenga un botón para resetear la hora a las 0:00
7. Basándote en el estándar SENML crea un fichero json (https://arduinojson.org/) que se genere cada 10 segundos, que contenga datos de temperatura inventados, las unidades y la marca temporal. Súbelos a un servidor ftp con un nombre que sea grupoXX_ddmmss.json
8. Usa la librería de Adafruit IO para subir/ publicar datos (y suscribirse/ recibir datos) al feed usando MQTT.

Los códigos añadidos en el apartado 8, son prácticamente en su totalidad, los ejemplos del [repositorio de Github de Adafruit](https://github.com/adafruit/Adafruit_IO_Arduino/tree/master/examples). Concretamente los ejemplos [*adafruitio\_00\_publish*](https://github.com/adafruit/Adafruit_IO_Arduino/tree/master/examples/adafruitio_00_publish) y [*adafruitio\_01\_subscribe*](https://github.com/adafruit/Adafruit_IO_Arduino/tree/master/examples/adafruitio_01_subscribe).