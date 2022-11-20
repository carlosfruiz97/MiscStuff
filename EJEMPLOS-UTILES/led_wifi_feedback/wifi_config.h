#define TIMEOUT_WIFI    60000 //ms
#define DEBUG_LED_ON    1     // Se enciende el LED_BUILTIN



/************************************************
 *  WiFi Config
 ***********************************************/
typedef struct wifiList_s
{
  const char *ssid;
  const char *password;
} wifiList_t;

#define NUM_WIFIS    2
wifiList_t wifiList[NUM_WIFIS] PROGMEM =
{
  { "CanCarlitos2", "6162909297"         },
  { "X2",           "LasMonjasVuelan666" }
  // { "iPhone de Carlos", "marpalcar"  },
};
int8_t WiFi_indx = -1;

const char *otaaHostName = "LedWifi";

IPAddress local_IP(192, 168, 1, 105);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

ESP8266WebServer server(80);

/* Controlar manualmente la desconexion del wifi */
WiFiEventHandler wifiDisconnectHandler;
WiFiEventHandler wifiConnectHandler;
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
void onWifiConnect(const WiFiEventStationModeGotIP& event);

/*******************************************************************************
 *  @brief  Escanea los wifis que hay y seleciona el que encuentra
 ******************************************************************************/
bool scan_select_wifi()
{
  String  ssid;
  int32_t rssi;
  uint8_t encryptionType;
  uint8_t *bssid;
  int32_t channel;
  bool    hidden;
  int     scanResult;

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  scanResult = WiFi.scanNetworks(/*async=*/ false, /*hidden=*/ true);

  if (scanResult == 0)
  {
    LOGF("No networks found \n");
    return false;
  }
  PRINTF("%d networks found:\n", scanResult);

#if DEBUG == 1
  for (int8_t i = 0; i < scanResult; i++)
  {
    WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

    PRINTF("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %s\n",
           i,
           channel,
           bssid[0], bssid[1], bssid[2],
           bssid[3], bssid[4], bssid[5],
           rssi,
           (encryptionType == ENC_TYPE_NONE) ? ' ' : '*',
           hidden ? 'H' : 'V',
           ssid.c_str());
  }
#endif

  // Buscar entre wifi list para forzar prioridad
  for (uint8_t j = 0; j < NUM_WIFIS; j++)
  {
    // Print unsorted scan results
    for (int8_t i = 0; i < scanResult; i++)
    {
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

      // Buscar si coincide el wifi
      if (strcmp(wifiList[j].ssid, ssid.c_str()) == 0)
      {
        PRINTF("!! WiFi %s found!\n", wifiList[j].ssid);
        WiFi_indx = j;
        return true;
      }
    }

    delay(0);
  }

  // Print scan results
  return false;
}


/************************************************
 *  @brief  Iniciar WiFi
 ***********************************************/
void setup_wifi()
{
  LOG("Connecting to ");
  LOGN(wifiList[WiFi_indx].ssid);

  wifiConnectHandler    = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);


  WiFi.mode(WIFI_STA);
  //  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  //  {
  //    LOGN("STA FAILED TO CONFIGURE");
  //  }
  WiFi.begin(wifiList[WiFi_indx].ssid, wifiList[WiFi_indx].password);

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
      LOGN("Restarting");
      ESP.restart();                   // restart
    }
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  LOGN("");
  LOGN("WiFi connected");
  LOGN("IP address: ");
  LOGN(WiFi.localIP());

  /* Reconectar Automaticamente si falla! */
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}


/************************************************
 *  @brief  Funcion llamada al desconectar reconectar
 ***********************************************/
void onWifiConnect(const WiFiEventStationModeGotIP& event)
{
  LOGN("Reconected!");
}


void onWifiDisconnect(const WiFiEventStationModeDisconnected& event)
{
  LOGN("Disconnected from Wi-Fi, trying to connect...");
  //  WiFi.disconnect();
  //  WiFi.begin(ssid, password);
  //  delay(1000);
  //  LOGN("Retry Conecting...");
  //  setup_wifi();
}


/************************************************
 *  @brief  Iniciar OTAA
 ***********************************************/
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


/************************************************
 *  HTML STRING
 ***********************************************/
const char html_string[] PROGMEM = R"=====(
<html>
  <head>
    <title>Led WiFi</title>
    <style>
    .button {
      border: 3px solid #FFFFFF;
      color: #000000;
      background-color: #666666;
      text-align: center;
      padding: 13px 0px;
      font-size: 23px;
      font-weight: bold;
      margin: 4px 2px;
      cursor: pointer;
      transition-duration: 0.4s;
      width: 220px;
    }
    .button:active {
      box-shadow: 0 5px #666;
      transform: translateY(2px);
    }

    .bOn {
      background-color: LightGreen;
    }
    .bOn:active {
        background-color: Green;
    }

    .bOff {
      background-color: Red;
    }
    .bOff:active {
        background-color: DarkRed;
    }

    .stateRect {
      border: 3px solid white;
      width: 440px;
      /* height: 50px; */
      background-color: black;
      text-align: center;
      margin: 4px 2px;
      font-size: 23px;
      padding: 13px 0px;
    }

    </style>
  </head>

  <script>
    // Send Led State!
    function led(state) {
      var xhr = new XMLHttpRequest();
      var url = "led?state=" + state; // + "&g=" + g + "&b=" + b;
      xhr.open('POST', url, true);
      xhr.send();
      console.log('Sent: led', state);
    }

    // Cambia el color del rectangulo de estado
    function changeStateRect(state)
    {
      if (state) {
        document.getElementById('_estado_led').innerHTML = "Led On";
        document.getElementById('_estado_led').style.backgroundColor = "white";
        document.getElementById('_estado_led').style.color="black";
      }
      else {
        document.getElementById('_estado_led').innerHTML = "Led Off";
        document.getElementById('_estado_led').style.backgroundColor = "black";
        document.getElementById('_estado_led').style.color="white";
      }
    }

    // -------------- AXAJ --------------------------
     var mytimer = setInterval(ajaxGetInfo, 1000);

    // AXAJ REQUEST OBJECT
    var ajaxRequest = null;
    if (window.XMLHttpRequest) {
      ajaxRequest = new XMLHttpRequest();
    } else {
      ajaxRequest = new ActiveXObject("Microsoft.XMLHTTP");
    }

    /************************************************
       @brief Sends GET request on /getInfo URL
     ***********************************************/
    function ajaxGetInfo() {
      if (!ajaxRequest) {
        alert('AJAX is not supported.');
        return;
      }

      // Open Request
      ajaxRequest.open('GET', 'getInfo', true);

      // Callback Function
      ajaxRequest.onreadystatechange = function() {
        if (ajaxRequest.readyState != 4 || ajaxRequest.status != 200) {
          return;
        }

        // Get text and parse
        const ajaxResult = ajaxRequest.responseText;
        const objResult = JSON.parse(ajaxResult);

        console.log(ajaxResult);
        console.log(objResult);

        // Modify HTML fields
        const estado_led = objResult['estado_led'];
        changeStateRect(estado_led);
      }

      // Send GET REQUEST
      ajaxRequest.send();
    }

  </script>

  <body text="#FFFFFF" bgcolor="#000000">
    <!-- Estado Led -->
    <h3>Estado Led:</h3>
    <div class="stateRect" id="_estado_led">???</div>
    <br></br>

    <!--  Botones on y Off -->
    <div>
      <h3>Control Led:</h3>
      <button class="button bOn" onclick="led(1)">ON</button>
      <button class="button bOff" onclick="led(0)">OFF</button>
    </div>

  </body>
</html>
)=====";
