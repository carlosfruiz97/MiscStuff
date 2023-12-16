
#define DEBUG 1
#if DEBUG == 1
#define LOGN(x) Serial.println(x)
#define LOG(x) Serial.print(x)
#define LOGX(x) Serial.print(x, HEX);
#define LOGSN(s, x)         \
    {                       \
        Serial.print(F(s)); \
        Serial.print(" ");  \
        Serial.println(x);  \
    }
#define LOGS(s, x)          \
    {                       \
        Serial.print(F(s)); \
        Serial.print(" ");  \
        Serial.print(x);    \
    }
#define LOGSX(s, x)           \
    {                         \
        Serial.print(F(s));   \
        Serial.print(" ");    \
        Serial.print(x, HEX); \
    }
#define LOGF(s) Serial.print(F(s))
#define LOGFN(s) Serial.println(F(s))
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
const uint8_t ARDUINO_RX = 4; // connect to TX of MP3 Player module
const uint8_t ARDUINO_TX = 5; // connect to RX of MP3 Player module

SoftwareSerial MP3Stream(ARDUINO_RX, ARDUINO_TX); // MP3 player serial stream for comms
MD_YX5300 mp3(MP3Stream);
const uint8_t PLAY_FOLDER = 1; // tracks are all placed in this folder

/*****************************************************************************
 * @brief Setup
 *****************************************************************************/
void setup()
{
#if DEBUG == 1
    Serial.begin(115200);
    while (!Serial)
    {
    }
    delay(50);
    LOGN("\n\n---------------\n  EMPEZAMOS");
#endif

    MP3Stream.begin(MD_YX5300::SERIAL_BPS);
    mp3.begin();
    mp3.setSynchronous(true);
    //mp3.playFolderRepeat(PLAY_FOLDER);
    mp3.setCallback(cbMp3);
}

void loop()
{
    mp3.check();

    // Serial Comands
  if (Serial.available())
  {
    char cmd = Serial.read();
    switch (cmd)
    {
    case 'p':
      mp3.playSpecific(1, 1);
      break;

    case 'o':
      mp3.playSpecific(1, 2);
      break;

    case 'i':
      mp3.playSpecific(1, 3);
      break;

    case 's':
      mp3.playStop();
      break;
    }
  }

}

void cbMp3(const MD_YX5300::cbData *status)
{
    LOGSX("Code: ", status->code);
    LOGSX("\tData: ", status->data);
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
