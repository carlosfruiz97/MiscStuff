#define TIMEOUT_WIFI          60000 //ms
#define RESTART_ON_TIMEOUT    1     // Restart on timeout while connecting?
#define DEBUG_LED_ON          1     // Se enciende el LED_BUILTIN


/*****************************************************************************
*   WiFi Config
*****************************************************************************/
//const char *ssid     = "ELLEBANNA";
//const char *password = "666LasMonjas";
const char *ssid     = "CanCarlitos2";
const char *password = "6162909297";

const char *mqtt_server   = "192.168.1.33";
const char *mqtt_username = "kasbeel";
const char *mqtt_password = "kasbeel";

/* OTAA */
const char *otaaHostName = "LedMqtt"; // Tambien sirve de mqtt name

WiFiClient espClient;
PubSubClient client(espClient);

/* Controlar manualmente la desconexion del wifi */
WiFiEventHandler wifiDisconnectHandler, wifiConnectHandler;


/*****************************************************************************
*   MQTT Topicos
*****************************************************************************/
#define TIPO        "luz"
#define GRUPO       "test"
#define NOMBRE      "L1"
#define CABECERA    TIPO "/" GRUPO "/" NOMBRE "/"

/* Topicos a Publicar */
const char *topico_estado = CABECERA "estado";

/* Topicos Subscritos */
#define N_TOPCIOS_SUBSCRITOS    1
const char *topico_comando = CABECERA "comando";
const char **lista_subscritos[N_TOPCIOS_SUBSCRITOS] =
{
  &topico_comando
};

/************************* FUNCTION HEADERS *************************/
void setup_wifi();
void setup_otaa();
void reconnect_mqtt();
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
void onWifiConnect(const WiFiEventStationModeGotIP& event);
void publicar_estado();
void ProcessComando(uint8_t *payload, uint8_t length);


void callback(char *topic, uint8_t *payload, unsigned int length);

/*****************************************************************************
*  @brief  Iniciar WiFi
*****************************************************************************/
void setup_wifi()
{
  LOGSN("Connecting to ", ssid);

  wifiConnectHandler    = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

#if DEBUG_LED_ON == 1
  pinMode(LED_BUILTIN, OUTPUT);
#endif

  uint32_t tstart = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
#if DEBUG_LED_ON == 1
    digitalWrite(LED_BUILTIN, LOW);  // On
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH); // OFF
    delay(250);
#endif

    LOG(".");
    if (millis() - tstart >= TIMEOUT_WIFI)
    {
      LOGFN("Restarting");
#if RESTART_ON_TIMEOUT == 1
      ESP.restart();                   // restart
#else
      return;
#endif
    }
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);



  /* Reconectar Automaticamente si falla! */
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  /* Setup OTAA */
  setup_otaa();
}


/*****************************************************************************
*  @brief  Funcion llamada al reconectar
*****************************************************************************/
void onWifiConnect(const WiFiEventStationModeGotIP& event)
{
  LOGFN("Reconected!");
  LOGSN("\nWiFiConnected. Ip Address:  ", WiFi.localIP());

  /* Setup MQTT */
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


/*****************************************************************************
*  @brief  Funcion llamada al desconectar
*****************************************************************************/
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event)
{
  LOGFN("Disconnected from Wi-Fi, trying to connect...");
  //  WiFi.disconnect();
  //  WiFi.begin(ssid, password);
  //  delay(1000);
  //  LOGN("Retry Conecting...");
  //  setup_wifi();
}


/*****************************************************************************
*  @brief  Iniciar OTAA
*****************************************************************************/
void setup_otaa()
{
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(otaaHostName);

  ArduinoOTA.onStart([] () {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else     // U_FS
    {
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    LOGN("Start updating " + type);
  });

  ArduinoOTA.onEnd([] () {
    LOGN("\nEnd");
  });

  ArduinoOTA.onProgress([] (unsigned int progress, unsigned int total) {
    //    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    PRINTF("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([] (ota_error_t error) {
    PRINTF("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      LOGN("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      LOGN("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      LOGN("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      LOGN("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      LOGN("End Failed");
    }
  });

  ArduinoOTA.begin();
}


/*****************************************************************************
*  @brief  Reconectar Al MQTT
*****************************************************************************/
void reconnect_mqtt()
{
  while (!client.connected())
  {
    LOGFN("Conectando al servidor MQTT...");

    // Create a random client ID
    String clientId = "Esp8266"; //otaaHostName;
    clientId += String(random(0xffff), HEX);


    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
    {
      LOGSN("Conectado a mqtt!\nPublicamos a \n - ", topico_estado);
      publicar_estado();

      // Subscribe to topics
      LOGFN("Subscritos a: ");
      for (int i = 0; i < N_TOPCIOS_SUBSCRITOS; i++)
      {
        client.subscribe(*lista_subscritos[i]);
        LOGSN("  - ", *lista_subscritos[i]);
      }
    }
    else
    {
      LOGS("failed, rc=", client.state());
      LOGFN(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000); // TODO no me gusta el delay este...
    }
  }
}


/*****************************************************************************
*  @brief  Timer de Keepalive
*****************************************************************************/
#define KEEP_ALIVE_INTERVAL    15 * 1000
Timer < 1 > timerKeepAlive;

/*****************************************************************************
*  @brief  Publicar Estado
*****************************************************************************/
char output[32];
bool _compilarEnviarEstado(void *)
{
  LOGFN("Publicando Estado");

  StaticJsonDocument < 16 > doc;

  doc["led"] = estado.led;
  serializeJson(doc, output);

  client.publish(topico_estado, output);
  return true;
}


void publicar_estado()
{
  // Cancel Timer
  timerKeepAlive.cancel();

  // Publicar Estado
  _compilarEnviarEstado(NULL);

  // Reiniciar Timer
  timerKeepAlive.every(KEEP_ALIVE_INTERVAL, _compilarEnviarEstado);
}


/*****************************************************************************
*  @brief  MQTT CallBack Function
*****************************************************************************/
void callback(char *topic, uint8_t *payload, unsigned int length)
{
  /* LOG Stuff */
#if DEBUG == 1
  PRINTF("\nMessage arrived on topic: [%s] \n\t Msg len: %d. Msg: [", topic, length);
  for (uint8_t i = 0; i < length; i++)
  {
    LOG((char)payload[i]);
  }
  LOGF("]\n");
#endif

  /* Procesar comando */
  if (strcmp(topic, topico_comando) == 0)
  {
    LOGFN("TOPICO COMANDO");
    ProcessComando(payload, length);
  }
}


void ProcessComando(uint8_t *payload, uint8_t length)
{
  /* Deserialize */
  StaticJsonDocument < 16 > doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error)
  {
    LOGF("deserializeJson() failed: ");
    LOGN(error.f_str());
    return;
  }

  /* Extraer valores de JSON */
  uint8_t led = doc["led"];

  PRINTF("Comando: Led [%d] \n", led);

  /* Ejecutar comando  */
  setLed(led);

  /* Publicamos estado actualizado */
  publicar_estado();
}
