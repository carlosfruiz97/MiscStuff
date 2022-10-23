# led_wifi con control MQTT

En esta plantilla encendemos un led desde mqtt ademas de actualizar OTAA

- Se conecta al WiFi e inicia el mqtt
- Se reconecta al servidor mqtt (con usuario y cnts) solo
- El WiFi se reconecta solo
- Gestiona la actualizacion del FW por OTAA

## Archivos

- led_mqtt.ino Archivo principal
- wifi_mqtt.h Funciones de config de wifi, OTAA, y las funciones MQTT

**NOTA**: Al inicio de los dos archivos, se deberían declarar las funciones.

## Topicos

### Publicar

- `luz/test/L1/estado` Publica un json cada **KEEP_ALIVE_INTERVAL**, con el estado de nodemcu

### Subscribir

- `luz/test/L1/comando` Se subscribe, recibiendo un json con el comando del led. Finaliza publicando el estado
