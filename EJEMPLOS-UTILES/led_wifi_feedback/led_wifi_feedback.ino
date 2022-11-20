/*******************************************************************************
 *  Titulo: Controlar un led con Wifi
 * Placa: Nodmecu
 *
 * cmpUpldEsp.sh -f led_wifi_feedback.ino
 *
 * Subir Codigo:
 *     arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 led_wifi_feedback.ino
 *     arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:nodemcuv2 led_wifi_feedback.ino
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
#define LOGF(s)           Serial.print(F(s))
#define LOGFN(s)          Serial.println(F(s))
#define PRINTF(s, ...)    Serial.printf(PSTR(s), __VA_ARGS__)
#else
#define LOG(x)
#define LOGN(x)
#define LOGX(x)
#define LOGSN(s, x)
#define LOGS(s, x)
#define LOGSX(s, x)
#define LOGF(s)
#define LOGFN(s)
#define PRINTF(s, ...)
#endif

/************************************************
 *  Librerias Externas
 ***********************************************/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


/************************************************
 *  WiFi Init
 ***********************************************/
#include "wifi_config.h"


/************************************************
 *  GPIO
 ***********************************************/
#define PIN_LED       LED_BUILTIN
bool estado_led = false;
#define ACTIVE_LED    LOW
void setLed(bool nuevo_estado);

/************************************************
 *  Botones
 ***********************************************/

/************************************************
 * @brief Setup
 ***********************************************/
void setup()
{
  ////////////////
  // Serial Init
#if DEBUG == 1
  Serial.begin(9600);
  while (!Serial)
  {
  }
  delay(50);
  LOGN("\n\n---------------\n  EMPEZAMOS");
#endif

  //////////////////////////
  // Setup Wifi and server
  scan_select_wifi();
  setup_wifi();
  setup_otaa();
  setup_server();

  /////////////
  // Led Init
  pinMode(PIN_LED, OUTPUT);
  setLed(false);
}


/************************************************
 * @brief Loop
 ***********************************************/
void loop()
{
  server.handleClient();
  ArduinoOTA.handle();
}


/************************************************
 * @brief Setup WebServer with callbacks
 ***********************************************/
void setup_server()
{
  server.on("/", handleRoot);
  server.on("/led", handleLed);   // stop pista
  server.on("/getInfo", handleGetInfo);
  server.onNotFound(handleNotFound);
  server.begin();
}


/************************************************
 * @brief ServerCB  OnRoot
 ***********************************************/
void handleRoot()
{
  server.send(200, "text/html", html_string);
  LOGN("\n[server] Handle Root");
}


/************************************************
 * @brief ServerCB  OnRoot
 ***********************************************/
void handleNotFound()
{
  String message = "File Not Found\n\n";

  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  // server.send(404, "text/plain", message);
  server.send(200, "text/html", html_string);
  LOGN("\n[server] Request not found");
  LOGN(message);
}


/************************************************
 * @brief ServerCB  On Led Commnad  '/led'
 * Args is state=1/0
 ***********************************************/
void handleLed()
{
  server.send(200, "text/html", html_string);
  String arg_str       = server.arg("state");
  bool   new_led_state = arg_str.toInt();

  LOGS("led \tstr:", arg_str);
  LOGSN("\tnew_led_state:", new_led_state);
  setLed(new_led_state);
}


/************************************************
 * @brief ServerCB  On GetInfo Commnad  '/getInfo'
 * Returns {'estado_led':1}
 ***********************************************/
void handleGetInfo()
{
  String                 output;
  StaticJsonDocument<16> doc;

  doc["estado_led"] = estado_led;
  serializeJson(doc, output);
  server.send(200, "text", output);
}


/************************************************
 *  @brief util setLed
 ***********************************************/
void setLed(bool nuevo_estado)
{
  estado_led = nuevo_estado;
  digitalWrite(PIN_LED, estado_led == ACTIVE_LED);
}
