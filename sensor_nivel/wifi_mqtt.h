const char *ssid          = "CanCarlitos2";
const char *password      = "6162909297";
const char *mqtt_server   = "192.168.1.62";

WiFiClient espClient;
PubSubClient client(espClient);

#define TIMEOUT_WIFI    30000            //ms
#define TIMEOUT_MQTT    5000             //ms
bool     RESTART_WIFI_ON_TIMEOUT = false;
uint32_t last_mqtt_reconnect_attempt;
uint32_t last_wifi_reconnect_attempt;
bool     wifi_conected = false;

const char *topico_sensor = "garaje/resina/nivel";


// == FUNCTIONS ==========================================
void reconectar();
void setup_wifi();
void setup_mqtt();
bool reconnect_mqtt();
void callback(char *, byte *, unsigned int);

// ========================================================
void reconectar()
{
  bool current_is_wifi_conected = WiFi.status() == WL_CONNECTED;

#if DEBUG == 1
  if (wifi_conected && !current_is_wifi_conected)
  {
    LOGN(" ---- DESCONECTADO DEL WIFI  ---- ");
    //    digitalWrite(LED_PIN, true);
  }
  else if (!wifi_conected && current_is_wifi_conected)
  {
    LOGN("\n********************");
    LOGN("** WiFi connected **");
    LOGN("********************");
    LOGN("IP address: ");
    LOGN(WiFi.localIP());

    //    digitalWrite(LED_PIN, false);
  }
#endif

  wifi_conected = current_is_wifi_conected;


  if (!wifi_conected)
  {
    if (millis() - last_wifi_reconnect_attempt > TIMEOUT_WIFI)
    {
      LOGN("Wifi No Conectado!");
      last_wifi_reconnect_attempt = millis();
      if (RESTART_WIFI_ON_TIMEOUT)
      {
        ESP.restart();                         // restart
      }
    }
    return;
  }


  if (!client.connected())
  {
    reconnect_mqtt();
  }
}

/* --------------------------------------------------------
    Setup Wifi
    -------------------------------------------------------- */
void setup_wifi()
{
  delay(10);

  // We start by connecting to a WiFi network
  LOG("Connecting to: ");
  LOGN(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  last_wifi_reconnect_attempt = millis();
}

/* --------------------------------------------------------
    Setup MQTT
    -------------------------------------------------------- */
void setup_mqtt()
{
  client.setServer(mqtt_server, 1883);

//  client.setCallback(callback);
  reconnect_mqtt();
}

/* --------------------------------------------------------
    Reconnect with MQTT SERVER
    -------------------------------------------------------- */
bool reconnect_mqtt()
{
  //
  if (!client.connected() &&
      (millis() - last_mqtt_reconnect_attempt > TIMEOUT_MQTT))
  {
    LOGN("Attempting MQTT connection...");
    last_mqtt_reconnect_attempt = millis();
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      LOGN("connected");
      LOGN("Publicamos a:");
      LOG("  - ");
      LOGN(topico_sensor);

      // Subscribe to Topics:
      // LOGN("Subscritos a: ");
      // for (int i = 0; i < N_TOPCIOS_SUBSCRITOS; i++)
      // {
      //   client.subscribe(*lista_subscritos[i]);
      //   LOG("  - ");
      //   LOGN(*lista_subscritos[i]);
      // }
      return true;
    }
    else
    {
      LOG("failed, rc=");
      LOG(client.state());
      LOG(" try again in ");
      LOG(TIMEOUT_MQTT);
      LOGN(" seconds");
    }
  }
  return false;
}
