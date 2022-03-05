/*
 * Link tutorial: https://www.ardumotive.com/how-to-use-a-keypad-en.html
 *
 * arduino-cli lib install Keypad
 *
 * arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old teclado
 *
 * arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano:cpu=atmega328old teclado
 */

#define DEBUG    1
#if DEBUG == 1
#define LOGN(x)    Serial.println(x)
#define LOG(x)     Serial.print(x)
#else
#define LOG(x)
#define LOGN(x)
#endif

#include "Adafruit_Keypad.h"

const byte ROWS = 4; // rows
const byte COLS = 3; // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte rowPins[ROWS] = { 5, 4, 3, 2 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 8, 7, 6 };    //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  Serial.begin(115200);
  LOG("Hello");
  customKeypad.begin();
}


void loop()
{
  // put your main code here, to run repeatedly:
  customKeypad.tick();

  while (customKeypad.available())
  {
    keypadEvent e = customKeypad.read();
    Serial.print((char)e.bit.KEY);
    if (e.bit.EVENT == KEY_JUST_PRESSED)
    {
      Serial.println(" pressed");
    }
    else if (e.bit.EVENT == KEY_JUST_RELEASED)
    {
      Serial.println(" released");
    }
  }

  delay(10);
}
