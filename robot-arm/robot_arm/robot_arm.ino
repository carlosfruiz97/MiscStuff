/*******************************************************************************
 *  Titulo: Controlar un led con MQTT
 *
 * Placa: Nodmecu
 * Descripcion:
 *
 * Subir Codigo:
 *     arduino-cli compile --fqbn arduino:avr:uno robot_arm.ino
 *     arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno robot_arm.ino
 *
 *     arduino-cli monitor -p /dev/ttyUSB0
 *
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
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/************************************************
 *  Control Button
 ***********************************************/

/************************************************
 *  Control Servos
 ***********************************************/
#define NUM_SERVOS    5
const uint8_t pin_servo[NUM_SERVOS] = { 9, 6, 5, 3, 11 };
const uint8_t pin_pot[NUM_SERVOS]   = { A0, A1, A2, A3, A6 };
Servo         servoArry[NUM_SERVOS];

/************************************************
 *  OLED
 ***********************************************/
#define OLED_RESET       -1
#define SCREEN_WIDTH     128 // OLED display width, in pixels
#define SCREEN_HEIGHT    64  // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/************************************************
 *  Estado
 ***********************************************/
typedef enum
{
  STOP,
  POT_CONTROL,
  PLAY
}
modo_t;

typedef struct estado_s
{
  modo_t   modo = POT_CONTROL;
  uint16_t potVals[NUM_SERVOS];
  uint8_t  x[NUM_SERVOS] = { 0, 0, 0, 0, 0 };
} estado_t;
estado_t estado;

/* FUNCTION DEFINITIONS ********************************************************/
void attach_servos();
void readPots();
void writeServos();
void potentiometer_control();

/************************************************
 *  File Includes
 ***********************************************/
#include "servo_control.h"
#include "secuencia.h"


secuencia_t mySec =
{
  .sec_len   = 3,
  .pasosArry =
  {
    // { { 90,  90,  90, 90,  90 },  100 },
    { { 80, 104, 122, 91, 165 }, 3000 },
    { { 19,  65,  80, 96, 165 }, 1000 }
    // { { 19,  65,  83, 135, 127 }, 1000 },
    // { { 15,  37,  38,  65, 180 },  500 },
    // { { 19,  65,  83, 135, 127 }, 1000 },
    // { { 80, 104, 122,  91, 165 }, 2000 },
  },
  .nombre    = "mySec"
};

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


  /////////////////
  // Atach Servos
  attach_servos();
  delay(1000);
  writeServos();
  delay(1000);

  ////////////////
  // Display OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE); //Sets the font display color
  display.setTextSize(1);
  display.clearDisplay();      //cls

  // LOGSN("MySec: ", mySec.pasosArry[0].dest[0]);
  // // LOGF()
  // start_secuencia(&mySec);
  SeqManag.activar(mySec);
}


/*****************************************************************************
* @brief Loop
*****************************************************************************/
void loop()
{
  switch (estado.modo)
  {
  case STOP:
    break;

  case POT_CONTROL:
    potentiometer_control();
    break;

  case PLAY:
    // SeqManag.update();
    // SeqManag.displayInfo();
    // display.clearDisplay();
    // display.setCursor(0, 0);
    // display.println(F("Modo: PLAY"));
    // display.display();
    // update_display(&mySec);
    break;
  }

  // if (Serial.available())
  // {
  //   Serial.write(Serial.read());
  // }
}


// void cambiarEstado()
// {
//   if (button.fell())
//   {
//     switch (estado.modo)
//     {
//     case STOP:
//       LOGFN("STOP");
//       estado.modo = POT_CONTROL;
//       break;
//
//     case POT_CONTROL:
//       LOGFN("POT_CONTROL");
//       estado.modo = PLAY;
//       break;
//
//     case PLAY:
//       LOGFN("PLAY");
//       estado.modo = STOP;
//       break;
//     }
//   }
// }
