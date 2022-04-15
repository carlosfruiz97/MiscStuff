// Subir Codigo :
// arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 rfid_mqtt.ino
// arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:nodemcuv2 rfid_mqtt.ino

#define DEBUG    1
#if DEBUG == 1
#define LOGN(x)    Serial.println(x)
#define LOG(x)     Serial.print(x)
#else
#define LOG(x)
#define LOGN(x)
#endif

// ==== LIBRERIAS EXTERNAS =======================================
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <stdio.h>
#include <stdlib.h>
#include <arduino-timer.h>

// ==== INIT FUNCTIONS ===========================================

// ==== STATE VARIABLES ==========================================
// -- Rfid --
uint8_t rfid_raw_current, rfid_raw_previous, // Ulitmo valor leido en rfid
        rfid_tag_val;                        // Valor filtrado


// ==== TIMERS ===================================================
auto timer = timer_create_default();
#define T_LEER_RFID         50
#define T_ENVIAR_PING       1000 // ms intervalo de mandar ping
#define T_ENVIAR_ESTADO     500  // ms intervalo mandar estado

// ==== RFID =====================================================
#define RST_PIN             D3     // Configurable, see typical pin layout above
#define SS_PIN              D4     // Configurable, see typical pin layout above
#define USING_ULTRALIGHT    1
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
#include "MFRC522_interface.h"

// ==== WiFi y MQTT =============================================
/* Lo Definimos ultimo antes del setup?*/
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

  // -- RFID --
  SPI.begin();                  // Init SPI bus
  mfrc522.PCD_Init();           // Init MFRC522 card
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();

  // Timers
  timer.every(T_LEER_RFID, actualizar_rfid);
  timer.every(T_ENVIAR_PING, [](void *) -> bool {
    publicar_ping();
    return true;
  });
  timer.every(T_ENVIAR_ESTADO, [](void *) -> bool {
    publicar_estado();
    return true;
  });
}


// ===============================================================
//            LOOP
// ===============================================================
void loop()
{
  // -- Update Constantly
  if (!client.connected())
  {
    LOGN("Disconected");
    reconnect_mqtt();
  }
  client.loop();
  timer.tick();
}


/* ------------------------------------
 * Actualizar RFID
 * ------------------------------------ */
bool actualizar_rfid(void *)
{
  // Leemos el bloque (4,0) y vemos si es 0xFF (master)
  uint8_t current_reading = 0;

  if (ReadData(4, 0, current_reading))
  {
    rfid_tag_val = current_reading;
  }
  else
  {
    if (rfid_raw_previous != 0) // previous not zero
    {
      rfid_tag_val = rfid_raw_previous;
    }
    else
    {
      rfid_tag_val = 0;
    }
  }
  rfid_raw_previous = current_reading;

  LOG("RFID  current reading: ");
  LOG(current_reading);
  LOG("\t filtered value: ");
  LOGN(rfid_tag_val);


  // publicar_tag(result);

  return true;
}
