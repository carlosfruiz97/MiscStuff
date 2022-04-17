/*
 * CONTROLAR MP3 con nodmecu y web
 *
 * //play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)
 * 192.168.1.40/play?pista=2&vol=30
 * 192.168.1.40/stop
 *
 * Compile:
 *  arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 mp3_esp8266
 * Upload:
 *  arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:nodemcuv2 mp3_esp8266
 *
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
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// ==== WIFI  ====================================================
#include "index_html.h"
#define TIMEOUT_WIFI    30000 //ms
const char *ssid     = "CanCarlitos2";
const char *password = "6162909297";

ESP8266WebServer server(80);

// ==== MP3 ======================================================
#define SS_RX    D2
#define SS_TX    D3
SoftwareSerial      mySoftwareSerial(SS_RX, SS_TX); // RX, TX
DFRobotDFPlayerMini myDFPlayer;


// ==== GPIO =====================================================
#define PIN_LEDS    D5
#define LED_ON      HIGH
bool led_state;



// ===============================================================
//            SETUP
// ===============================================================
void setup()
{
  ///////////////////////
  // -- Serial --
#if DEBUG == 1
  Serial.begin(115200);
  while (!Serial)
  {
  }
  delay(50);
  LOGN("\n\n---------------\n  EMPEZAMOS");
#endif

  ///////////
  // Leds
  pinMode(PIN_LEDS, OUTPUT);
  setLed(false);

  ///////////////
  // myDFPlayer
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial))    //Use softwareSerial to communicate with mp3.
  {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  }
  else
  {
    Serial.println(F("DFPlayer Mini online."));
  }

  //////////////
  // -- WiFi --
  setup_wifi();

  //////////////////////////
  // -- Server Callbacks --
  server.on("/", handleRoot);

  server.on("/play", handlePlay);   // play pista y vol
  server.on("/stop", handleStop);   // stop pista

  server.onNotFound(handleNotFound);

  server.begin();
}


void setup_wifi()
{
  LOG("Connecting to ");
  LOGN(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long tstart = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, LOW);  // On
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH); // OFF
    delay(250);

    LOG(".");
    if (millis() - tstart >= TIMEOUT_WIFI)
    {
      LOGN("Restarting");
      ESP.restart();                   // restart
    }
  }

  LOGN("");
  LOGN("WiFi connected");
  LOGN("IP address: ");
  LOGN(WiFi.localIP());
}


// ===============================================================
//            LOOP
// ===============================================================
void loop()
{
  server.handleClient();

  // Read State of myDFPlayer
  if (myDFPlayer.available())
  {
    handleDFPlayer();
  }
}


// === GPIOS =====================================================
void setLed(bool ledVal)
{
  digitalWrite(PIN_LEDS, ledVal == LED_ON);
  led_state = ledVal;
}


// ===============================================================
//            MP3
// ===============================================================
void handleDFPlayer()
{
  // Read type and value
  uint8_t type  = myDFPlayer.readType();
  int     value = myDFPlayer.read();

  // handle cases we care about!
  switch (type)
  {
  // FINISHED PLAYING !!
  case DFPlayerPlayFinished:
    setLed(false);
    break;

  default:
    break;
  }

  // Print State!
#if DEBUG == 1
  printmyDFPlayerDetail(type, value);
#endif
}


///////////////////////////////////////////////////////
// print details of MP3
void printmyDFPlayerDetail(uint8_t type, int value)
{
  Serial.print("myMp3:   ");
  switch (type)
  {
  case TimeOut:
    Serial.println(F("Time Out!"));
    break;

  case WrongStack:
    Serial.println(F("Stack Wrong!"));
    break;

  case DFPlayerCardInserted:
    Serial.println(F("Card Inserted!"));
    break;

  case DFPlayerCardRemoved:
    Serial.println(F("Card Removed!"));
    break;

  case DFPlayerCardOnline:
    Serial.println(F("Card Online!"));
    break;

  case DFPlayerPlayFinished:
    Serial.print(F("Number:"));
    Serial.print(value);
    Serial.println(F(" Play Finished!"));
    break;

  case DFPlayerError:
    Serial.print(F("DFPlayerError:"));
    switch (value)
    {
    case Busy:
      Serial.println(F("Card not found"));
      break;

    case Sleeping:
      Serial.println(F("Sleeping"));
      break;

    case SerialWrongStack:
      Serial.println(F("Get Wrong Stack"));
      break;

    case CheckSumNotMatch:
      Serial.println(F("Check Sum Not Match"));
      break;

    case FileIndexOut:
      Serial.println(F("File Index Out of Bound"));
      break;

    case FileMismatch:
      Serial.println(F("Cannot Find File"));
      break;

    case Advertise:
      Serial.println(F("In Advertise"));
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }
}


// ===============================================================
//            SERVER CALLBACKS
// ===============================================================
// --- on reload ---
void handleRoot()
{
  digitalWrite(led, 1);
  server.send(200, "text/plain", html_string);
  digitalWrite(led, 0);
}


// On play song command handler
void handlePlay()
{
  //   192.168.1.40/play?pista=2&vol=30
  uint8_t vol, pista;

  if (server.arg("pista") == "")
  {
    LOGN("Parametro 'pista' no encontrado");
    return;
  }
  else
  {
    String pista_str = server.arg("pista");
    pista = pista_str.toInt();
  }

  if (server.arg("vol") == "")
  {
    vol = 25;
    LOGN("No hay parametro de volumen!");
  }
  else
  {
    String vol_str = server.arg("vol");
    vol = vol_str.toInt();
  }

  // Poner pista MP3
  myDFPlayer.volume(vol);
  myDFPlayer.playMp3Folder(pista); //play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)

  // Poner Led
  setLed(true);
}


void handleStop()
{
  // Stop!
  setLed(false);
  myDFPlayer.pause();  //pause the mp3
}


// --- on handle not found ---
void handleNotFound()
{
  digitalWrite(led, 1);
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
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
