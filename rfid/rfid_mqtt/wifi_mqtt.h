/*
 *  =====  SETUP ================================
 */


const char *ssid     = "CanCarlitos2";
const char *password = "6162909297";
// const char *ssid     = "junglerace";
// const char *password = "LasMonjasVuelan666";
#define TIMEOUT_WIFI    30000 //ms
const char *mqtt_server   = "192.168.1.51";
const char *mqtt_username = "epiphany2";
const char *mqtt_password = "epiphany";

WiFiClient espClient;
PubSubClient client(espClient);

// ---- TOPCIOS --------------------------------------------------
// #define TIPO                    "puerta"
// #define HABITACION              "jardin"
// #define GRUPO                   "entrada"
// #define NOMBRE                  "entrada"
#define CABECERA                "pruebas"

// Topicos Subscritos
#define N_TOPCIOS_SUBSCRITOS    1
const char *topico_comando = CABECERA "comando";
const char **lista_subscritos[N_TOPCIOS_SUBSCRITOS] =
{
  &topico_comando
};

// Topicos a Publicar
const char *topico_estado = "rfid_tag";
const char *topico_ping   = CABECERA "ping";

// Json doc for input msg
StaticJsonDocument < 200 > jsonDoc;

// == FUNCTIONS ==========================================
void setup_wifi();
void setup_mqtt();
void reconnect_mqtt();
void publicar_estado();
void publicar_ping();

void callback(char *, byte *, unsigned int);

// void comandoCallback(char *);


// ========================================================

/* --------------------------------------------------------
 *  Setup Wifi
 *  -------------------------------------------------------- */
void setup_wifi()
{
  delay(10);

  // We start by connecting to a WiFi network
  LOG("Connecting to ");
  LOGN(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  pinMode(LED_BUILTIN, OUTPUT); // GPIO16 = D0

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

  digitalWrite(LED_BUILTIN, HIGH); // OFF
  LOGN("");
  LOGN("WiFi connected");
  LOGN("IP address: ");
  LOGN(WiFi.localIP());
}


/* --------------------------------------------------------
 *  Setup MQTT
 *  -------------------------------------------------------- */
void setup_mqtt()
{
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect_mqtt();
}


/* --------------------------------------------------------
 *  Reconnect with MQTT SERVER
 *  -------------------------------------------------------- */
void reconnect_mqtt()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    LOGN("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
    {
      LOGN("connected");
      LOG("Publicamos a:\n  - ");
      LOGN(topico_estado);
      publicar_estado();

      // Subscribe to Topics:
      LOGN("Subscritos a: ");
      for (int i = 0; i < N_TOPCIOS_SUBSCRITOS; i++)
      {
        client.subscribe(*lista_subscritos[i]);
        LOG("  - ");
        LOGN(*lista_subscritos[i]);
      }
    }
    else
    {
      LOG("failed, rc=");
      LOG(client.state());
      LOGN(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


/* --------------------------------------------------------
 *  Publicar Estado
 *  -------------------------------------------------------- */
void publicar_estado()
{
  DynamicJsonDocument state_doc(1024);

  char output[128];

  state_doc["valor_rfid"] = rfid_tag_val;
  serializeJson(state_doc, output);

  client.publish(topico_estado, output);
}


/* --------------------------------------------------------
 *  Publicar PING
 *  -------------------------------------------------------- */
void publicar_ping()
{
  client.publish(topico_ping, "1");
}


/* --------------------------------------------------------
 *  MQTT CALLBACK FUNCTION
 *  -------------------------------------------------------- */
void callback(char *topic, byte *payload, unsigned int length)
{
  char *msg = (char *)malloc(sizeof(char) * length);

  for (int i = 0; i < length; i++)
  {
    msg[i] = (char)payload[i];
  }
  LOG("Message arrived on topic: [");
  LOG(topic);
  LOGN("]  Message: ");
  LOGN(msg);

  // if (strcmp(topic, topico_comando) == 0)
  // {
  //   comandoCallback(msg);
  // }

  free(msg);
}


// /* --------------------------------------------------------
//  *  COMANDO CALLBACK
//  *  -------------------------------------------------------- */
// void comandoCallback(char *msg)
// {
//   // Deserialize the JSON document
//   DeserializationError error = deserializeJson(jsonDoc, msg);
//
//   // Test if parsing succeeds.
//   if (error)
//   {
//     LOG(F("deserializeJson() failed: "));
//     LOGN(error.f_str());
//     return;
//   }
//
//   // Valores del Comando
//   const char    *comando  = jsonDoc["comando"];
//   unsigned long t_abierto = jsonDoc["t_abierto"]; // Opcional!
//
//   LOG("Comando:   ");
//   LOGN(comando);
//   LOG("t_abierto: ");
//   LOGN(t_abierto);
//
//   // Guardar Estadp antes de nada
//   abierto_antiguo = abierto;
//
//   // Procesar Comando
//   if (strcmp(comando, "abrir") == 0)
//   {
//     abrir_puerta(true);
//
//     if (t_abierto) // Si t_abierto presente !
//     {
//       timer.in(t_abierto, reset_abierto_anitguo);
//     }
//   }
//
//   else if (strcmp(comando, "cerrar") == 0)
//   {
//     abrir_puerta(false);
//   }
//
//   else if (strcmp(comando, "toggle") == 0)
//   {
//     abrir_puerta(!abierto_antiguo);
//   }
//
//
//   // abrir_puerta();
// }
