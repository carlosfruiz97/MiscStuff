/*
  Codigo muestra para controlar un mp3 serial

  Fuentes:
  - https://github.com/cefaloide/ArduinoSerialMP3Player
  -


  Nombres archivos para que funcione
  |
  |-01
  |  |- 001xxx.mp3
  |  |- 002xxx.mp3
  |-02
  |  |- 001xxx.mp3
  |  |- 002xxx.mp3
  |  |- 005xxx.mp3
*/



/* Si usamos arduino mega */
#define mp3    Serial1

/* Si usamos otro arduino */
//#include <SoftwareSerial.h>
//#define ARDUINO_RX 5  //should connect to TX of the Serial MP3 Player module
//#define ARDUINO_TX 6  //connect to RX of the module
//SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);

#include "serial_mp3.h"


void setup()
{
  Serial.begin(115200);
  mp3.begin(9600);
  delay(500);

  sendCommand(CMD_SEL_DEV, DEV_TF);
  delay(500);
  //  playFromFolder(01, 01);
}


void loop()
{
  char c = ' ';

  // If there a char on Serial call sendMP3Command to sendCommand
  if ( Serial.available() )
  {
    c = Serial.read();
    if (c == 't') {
      //      sendCommand(CMD_PLAY_FOLDER_FILE, 0x0204); // ask for the number of file is playing
      playFromFolder(02, 05);
    } else {
      sendMP3Command(c);
    }
  }

  // Check for the answer.
  if (mp3.available())
  {
    Serial.println(decodeMP3Answer());
  }
  delay(100);
}
