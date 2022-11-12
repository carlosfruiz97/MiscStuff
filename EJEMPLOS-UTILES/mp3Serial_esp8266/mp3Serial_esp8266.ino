/*******************************************************************************
 *  Titulo: Ejemplo Software Serial MP3 player con nodemcu
 *
 * Placa: Nodmecu
 * Descripcion:
 *
 * Subir Codigo:
 *     arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 mp3Serial_esp8266.ino
 *     arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:nodemcuv2 mp3Serial_esp8266.ino
 *
 *     arduino-cli monitor -p /dev/ttyUSB0
 *
 * cmpUpldEsp.sh -f mp3Serial_esp8266.ino
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
#define PRINTF(s, ...)
#define LOGF(s)
#define LOGFN(s)
#endif

/************************************************
 *  Librerias Externas
 ***********************************************/
#include <MD_YX5300.h>
#include <SoftwareSerial.h>

/************************************************
 *  Init MP3
 ***********************************************/
#define N_SALAS      3
#define  pin_Rx_0    D1
#define  pin_Tx_0    D2

#define  pin_Rx_1    D7
#define  pin_Tx_1    D3

#define  pin_Rx_2    D5
#define  pin_Tx_2    D6

SoftwareSerial SS0(pin_Rx_0, pin_Tx_0);
SoftwareSerial SS1(pin_Rx_1, pin_Tx_1);
SoftwareSerial SS2(pin_Rx_2, pin_Tx_2);
SoftwareSerial *SS_arry[N_SALAS] = { &SS0, &SS1, &SS2 };

MD_YX5300 mp3_0(SS0);
MD_YX5300 mp3_1(SS1);
MD_YX5300 mp3_2(SS2);
MD_YX5300 *mp3Arry[N_SALAS] = { &mp3_0, &mp3_1, &mp3_2 };

void cbResp0(const MD_YX5300::cbData *status);
void cbResp1(const MD_YX5300::cbData *status);
void cbResp2(const MD_YX5300::cbData *status);

void (*cbRespArry[N_SALAS])(const MD_YX5300::cbData *) = { &cbResp0, &cbResp1, &cbResp2 };

#define FOR_LOOP_SALAS    for (uint8_t ns = 0; ns < N_SALAS; ns++)

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

  ///////////////
  // mp3_player
  delay(50);
  FOR_LOOP_SALAS
  {
    SS_arry[ns]->begin(MD_YX5300::SERIAL_BPS);
    mp3Arry[ns]->begin();
    mp3Arry[ns]->equalizer(2);
    mp3Arry[ns]->setSynchronous(true);
    mp3Arry[ns]->setCallback(cbRespArry[ns]);
  }
}


/*****************************************************************************
* @brief Loop
*****************************************************************************/
void loop()
{
  FOR_LOOP_SALAS
  {
    mp3Arry[ns]->check();
  }

  // Serial Comands
  if (Serial.available())
  {
    char cmd = Serial.read();
    switch (cmd)
    {
    case 'p':
      mp3Arry[0]->playSpecific(1, 1);
      break;

    case 'o':
      mp3Arry[1]->playSpecific(1, 2);
      break;

    case 'i':
      mp3Arry[2]->playSpecific(1, 3);
      break;

    case 'k':
      mp3Arry[0]->playSpecific(1, 1);
      mp3Arry[1]->playSpecific(1, 2);
      mp3Arry[2]->playSpecific(1, 3);
      break;

    case 's':
      mp3Arry[0]->playStop();
      mp3Arry[1]->playStop();
      mp3Arry[2]->playStop();
      break;
    }
  }
}


/*****************************************************************************
* @brief Callback
*****************************************************************************/
void cbResp0(const MD_YX5300::cbData *status)
{
  cbResponse(status, 0);
}


void cbResp1(const MD_YX5300::cbData *status)
{
  cbResponse(status, 1);
}


void cbResp2(const MD_YX5300::cbData *status)
{
  cbResponse(status, 2);
}


void cbResponse(const MD_YX5300::cbData *status, uint8_t id)
{
  PRINTF("Callback[%d]: Code: 0x%X     Data: 0x%X\n\t", id, status->code, status->data);
  switch (status->code)
  {
  case MD_YX5300::STS_OK:
    LOGF("STS_OK");
    break;

  case MD_YX5300::STS_TIMEOUT:
    LOGF("STS_TIMEOUT");
    break;

  case MD_YX5300::STS_VERSION:
    LOGF("STS_VERSION");
    break;

  case MD_YX5300::STS_CHECKSUM:
    LOGF("STS_CHECKSUM");
    break;

  case MD_YX5300::STS_TF_INSERT:
    LOGF("STS_TF_INSERT");
    break;

  case MD_YX5300::STS_TF_REMOVE:
    LOGF("STS_TF_REMOVE");
    break;

  case MD_YX5300::STS_ERR_FILE:
    LOGF("STS_ERR_FILE");
    break;

  case MD_YX5300::STS_ACK_OK:
    LOGF("STS_ACK_OK");
    break;

  case MD_YX5300::STS_FILE_END:
    LOGF("STS_FILE_END");
    break;

  case MD_YX5300::STS_INIT:
    LOGF("STS_INIT");
    break;

  case MD_YX5300::STS_STATUS:
    LOGF("STS_STATUS");
    break;

  case MD_YX5300::STS_EQUALIZER:
    LOGF("STS_EQUALIZER");
    break;

  case MD_YX5300::STS_VOLUME:
    LOGF("STS_VOLUME");
    break;

  case MD_YX5300::STS_TOT_FILES:
    LOGF("STS_TOT_FILES");
    break;

  case MD_YX5300::STS_PLAYING:
    LOGF("STS_PLAYING");
    break;

  case MD_YX5300::STS_FLDR_FILES:
    LOGF("STS_FLDR_FILES");
    break;

  case MD_YX5300::STS_TOT_FLDR:
    LOGF("STS_TOT_FLDR");
    break;

  default:
    // LOGF("STS_??? 0x");
    // Console.print(status->code, HEX);
    break;
  }

  LOGF("\n\n");
}
