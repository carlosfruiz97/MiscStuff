# led_wifi_feedback
En esta plantilla encendemos un led desde pagina web, y leemos el estado real.

Usamos la estructura de callbacks de ESP8266WebServer


## Archivos
- led_wifi_feedback.ino   Archivo principal
- wifi_config.h           Funciones de config de wifi y codigo html guardado en PROGMEM

### Html -> NodeMCU
Enviamos una url con argumentos. La estructura del string es :
```
"urlName?argA=1&argB=2&argC=3"
```

### NodeMCU -> HTML
Cada 1seg, mandamos un GET a `/getInfo`, que esta asociado a un Callback en el codigo C.

El callback genera un json con la info importante y lo manda.
