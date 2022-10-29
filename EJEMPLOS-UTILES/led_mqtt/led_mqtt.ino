/*******************************************************************************
 *  Titulo: Controlar un led con MQTT
 *
 * Placa: Nodmecu
 * Descripcion:
 *
 * Subir Codigo:
 *     arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 led_mqtt.ino
 *     arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:nodemcuv2 led_mqtt.ino
 *
 *     arduino-cli monitor -p /dev/ttyUSB0
 *
 ******************************************************************************/

#define DEBUG    1
#if DEBUG == 1
#define LOGN(x)           Serial.println(x)
#define LOG(x)            Serial.print(x)
#define LOGX(x)           Serial.print(x, HEX);
#define LOGSN(s, x)       { Serial.print(F(s)); Serial.print(" "); Serial.println(x); }
#define LOGS(s, x)        { Serial.print(F(s)); Serial.print(" "); Serial.print(x); }
#define LOGSX(s, x)       { Serial.print(F(s)); Serial.print(" "); Serial.print(x, HEX); }
#define PRINTF(s, ...)    { Serial.printf(s, __VA_ARGS__); }
#define LOGF(s)           Serial.print(F(s))
#define LOGFN(s)          Serial.println(F(s))
#define PRINTF(s, ...)    { Serial.printf(s, __VA_ARGS__); }
#else
#define LOG(x)
#define LOGN(x)
#define LOGX(x)
#define LOGSN(s, x)
#define LOGS(s, x)
#define LOGSX(s, x)
#deinfe PRINTF(s, ...)
#define LOGF(s)
#define LOGFN(s)
#endif

/************************************************
 *  Librerias Externas
 ***********************************************/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <arduino-timer.h>

/************************************************
 *  GPIO
 ***********************************************/
#define PIN_LED       LED_BUILTIN
#define ACTIVE_LED    LOW

/************************************************
 *  Estado
 ***********************************************/
typedef struct estado_s
{
  uint8_t led;
} estado_t;
estado_t estado;


/************************* FUNCTION HEADERS *************************/
void setLed(bool nuevo_estado);


/************************************************
 *  WiFi and Mqtt setup
 ***********************************************/
#include "wifi_mqtt.h"

/*****************************************************************************
* @brief Setup
*****************************************************************************/
void setup()
{
  ////////////////////////////
  // Serial Init
#if DEBUG == 1
  Serial.begin(9600);
  while (!Serial)
  {
  }
  delay(50);
  LOGN("\n\n---------------\n  EMPEZAMOS");
#endif

  ////////////////////////////
  // Led Init
  pinMode(PIN_LED, OUTPUT);
  setLed(false);

  ////////////////////////////
  // Setup WiFi
  setup_wifi();
}


/*****************************************************************************
* @brief Loop
*****************************************************************************/
void loop()
{
  ArduinoOTA.handle();
  if (!client.connected())
  {
    reconnect_mqtt();
  }
  client.loop();
  timerKeepAlive.tick();
}


/************************************************
 *  @brief util setLed
 ***********************************************/
void setLed(bool nuevo_estado)
{
  estado.led = nuevo_estado;
  digitalWrite(PIN_LED, nuevo_estado == ACTIVE_LED);
}
