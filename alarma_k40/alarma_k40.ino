#define DEBUG    1
#if DEBUG == 1
#define LOGN(x)    Serial.println(x)
#define LOG(x)     Serial.print(x)
#else
#define LOG(x)
#define LOGN(x)
#endif

// LIBRERIAS
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <arduino-timer.h>

// ==== WIFI ===================================================================
#define SSID_NAME       "CanCarlitos2"
#define PASSWORD        "6162909297"

#define TIMEOUT_WIFI    10000
IPAddress local_IP(192, 168, 1, 101);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

ESP8266WebServer server(80);

// ==== LEDS ===================================================================
#define LED_PIN       D6
#define LED_ACTIVO    HIGH
bool led_status = false;
void led(bool on);
void toggle_led();

#define BUZZER_PIN    D5
bool buzzer_status = false;
void buzzer(bool on);
void toggle_buzzer();

// ==== TIMERS =================================================================
Timer<1> timer;
#define T_PITIDO    250

// =============================================================================
// SETUP
// =============================================================================
void setup()
{
  // -- Serial --
#if DEBUG == 1
  Serial.begin(115200);
  delay(500);
  LOGN("\n\n---------------\n  EMPEZAMOS");
#endif

  // -- LED y buzzer --
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  led(false);
  buzzer(false);

  // -- Conect to wifi --
  connect2Wifi();

  // -- Start Server --
  server.on("/", handleRoot);
  server.on("/aviso", handle_aviso);
  server.on("/stop", handle_stop);
  server.onNotFound(handle_NotFound);

  server.begin();
  LOGN("HTTP server started");
}


void connect2Wifi()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  LOG("Conecting to: ");
  LOGN(SSID_NAME);

  // Config Static Ip
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    LOGN("STA FAILED TO CONFIGURE");
  }
  WiFi.begin(SSID_NAME, PASSWORD);

  unsigned long tstart = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    toggle_led();
    LOG(".");

    if (millis() - tstart >= TIMEOUT_WIFI)
    {
      // restart
      LOGN("RESTARTING");
      ESP.restart();
    }
  }
  LOG("Conected to: ");
  LOGN(WiFi.localIP());

  digitalWrite(LED_BUILTIN, LOW);
  led(false);
}


// ===============================================================
//            LOOP
// ===============================================================
void loop()
{
  server.handleClient();
  timer.tick();
}


// ===== TIMER ===================================================

bool toggle_all(void *arg)
{
  toggle_led();
  toggle_buzzer();
  return true;
}


// ==== WIFI =====================================================
void handleRoot()
{
  digitalWrite(LED_BUILTIN, HIGH);
  server.send(200, "text/plain", "hello from esp8266!\r\n");
  digitalWrite(LED_BUILTIN, LOW);
}


void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}


void handle_aviso()
{
  LOGN("AVISO");
  timer.every(T_PITIDO, toggle_all);
  server.send(200, "text/plain", "Aviso!!");
}


void handle_stop()
{
  LOGN("PARAMOS");
  timer.cancel();
  led(false);
  buzzer(false);
  server.send(200, "text/plain", "Stopped");
}


// ==== LED Y BUZZER ============================================
void led(bool on)
{
  digitalWrite(LED_PIN, on == LED_ACTIVO);
  led_status = on;
}


void toggle_led()
{
  led_status = !led_status;
  led(led_status);
}


void buzzer(bool on)
{
  digitalWrite(BUZZER_PIN, on);
  buzzer_status = on;
}


void toggle_buzzer()
{
  buzzer_status = !buzzer_status;
  digitalWrite(BUZZER_PIN, buzzer_status);
}
