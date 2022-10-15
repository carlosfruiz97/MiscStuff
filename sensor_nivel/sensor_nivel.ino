/*
  Sensor Nivel para la impresora de resina
*/

#define DEBUG    1
#if DEBUG == 1
#define LOGN(x)    Serial.println(x)
#define LOG(x)     Serial.print(x)
#else
#define LOG(x)
#define LOGN(x)
#endif

// ==== LIBRERIAS EXTERNAS =======================================
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ==== GPIO =====================================================
#define SENSOR_PIN A0

uint16_t value;
String str_value;
// ==== WiFi y MQTT =============================================
/* Lo Definimos ultimo antes del setup*/
#include "wifi_mqtt.h"

// ===============================================================
//            SETUP
// ===============================================================
void setup()
{
  // -- Serial --
#if DEBUG == 1
  Serial.begin(115200);
  delay(50);
  LOGN("\n\n----------------------------\n\tEMPEZAMOS\n");
#endif

  // -- WiFi y MQTT --
  setup_wifi();
  setup_mqtt();
}

void loop()
{
  reconectar();
  client.loop();
  if (wifi_conected)
  {

    value = analogRead(SENSOR_PIN); // read the analog value from sensor
    str_value = String( value);
    //    LOGN(str_value);
    client.publish(topico_sensor, str_value.c_str());

    delay(50);
  }
}
