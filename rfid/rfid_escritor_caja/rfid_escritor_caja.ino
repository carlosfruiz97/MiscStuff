/* ===============================================================
 *
 * Arduino-cli :
 *  arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old rfid_escritor_caja
 *
 *  arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano:cpu=atmega328old rfid_escritor_caja
 *
 *  sudo putty /dev/ttyUSB0 -serial -sercfg 115200,8,n,1,N
 *
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 * Propiedad de @logicprops
 * ===============================================================
 */
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
#include <Adafruit_Keypad.h>

// ==== KEYPAD ===================================================
const byte ROWS = 4; // rows
const byte COLS = 3; // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 8, 7, 6, 5 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 4, 3, 2 };    //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ==== RFID =====================================================
#define RST_PIN             9     // Configurable, see typical pin layout above
#define SS_PIN              10    // Configurable, see typical pin layout above
#define USING_ULTRALIGHT    1
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
#include "MFRC522_interface.h"

// ==== STATE ====================================================
enum Estado_t
{
  start,
  wait_ID,
  read_select_row, read_select_col, wait_read,
  write_select_row, write_select_col, write_select_val, wait_write,
};
Estado_t estado = start;

// ==== INPUT BUFFER =============================================
enum buffer_return_t
{
  not_done, done, cancel
};
uint8_t string_max_length   = 3;
uint8_t string_count        = 0;
String  input_string_buffer = "000";

uint8_t selected_row, selected_col, selected_val;

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

  // -- RFID --
  setup_mfrc522();

  // -- keypad begin --
  customKeypad.begin();

  print_to_screen("1:Mostrar Id  2:Leer  3:Escribir");
}


// ===============================================================
//            LOOP
// ===============================================================


void loop()
{
  customKeypad.tick();

  switch (estado)
  {
  case start:
    update_start();
    break;

  case wait_ID:
    update_wait_ID();
    break;

  case read_select_row:
    update_read_select_row();
    break;

  case read_select_col:
    update_read_select_col();
    break;

  case wait_read:
    update_wait_read();
    break;


  case write_select_row:
    update_write_select_row();
    break;

  case write_select_col:
    update_write_select_col();
    break;

  case write_select_val:
    update_write_select_val();
    break;

  case wait_write:
    update_wait_write();
    break;

  default:
    LOGN("ERROR unkown state");
    cancel_key();
    break;
  }

  delay(10);
}


// ===== UTILS ===============================
void cancel_key()
{
  estado = start;
  print_to_screen("1:Mostrar Id  2:Leer  3:Escribir");
}


// ===== ESTADOS =============================================================
// --- START --------------
void update_start()
{
  char pressed_key = "";

  if (get_keypad_key(pressed_key))
  {
    switch (pressed_key)
    {
    case '*':
      estado = start;
      cancel_key();
      break;

    case '1':
      print_to_screen("Mostrar ID. Pasar Tarjeta: ");
      estado = wait_ID;
      break;

    case '2':
      print_to_screen("Leyendo. Seleccionar fila y apretar #:");
      estado = read_select_row;
      break;

    case '3':
      print_to_screen("Escribiendo. Seleccionar fila y apretar #:");
      estado = write_select_row;
      break;
    }
  }
}


// --- Get ID --------------
void update_wait_ID()
{
  // See if Cancel
  char pressed_key = "";

  if (get_keypad_key(pressed_key))
  {
    if (pressed_key == '*')
    {
      cancel_key();
    }
  }

  MFRC522::Uid foundUid;
  if (GetUid(foundUid))
  {
    print_to_screen("ID: ");
    printUid(foundUid);
  }
}


// --- Leer val ------------
void update_read_select_row()
{
  buffer_return_t keypadstate = update_keypad_buffer(selected_row);

  switch (keypadstate)
  {
  case cancel:
    cancel_key();
    break;

  case done:
    // print_to_screen(" Done ");
    LOG("Row:  ");
    LOGN(selected_row);
    estado = read_select_col;
    print_to_screen("Leyendo. Seleccionar columna y apretar #:");

    break;
  }
}


void update_read_select_col()
{
  buffer_return_t keypadstate = update_keypad_buffer(selected_col);

  switch (keypadstate)
  {
  case cancel:
    cancel_key();
    break;

  case done:
    // print_to_screen(" Done ");
    LOG("Col:  ");
    LOGN(selected_col);
    estado = wait_read;

    print_to_screen("Leyendo. Pasar Tarjeta");
    break;
  }
}


void update_wait_read()
{
  // See if Cancel
  char pressed_key = "";

  if (get_keypad_key(pressed_key))
  {
    if (pressed_key == '*')
    {
      cancel_key();
    }
  }

  byte result;
  if (ReadData(selected_row, selected_col, result))
  {
    LOG("Read [row,col]=[");
    LOG(selected_row);
    LOG(",");
    LOG("]  value: ");
    LOGN(result);
    delay(500);
  }
}


// --- Escribir val ------------
void update_write_select_row()
{
  buffer_return_t keypadstate = update_keypad_buffer(selected_row);

  switch (keypadstate)
  {
  case cancel:
    cancel_key();
    break;

  case done:
    // print_to_screen(" Done ");
    LOG("Row:  ");
    LOGN(selected_row);
    estado = write_select_col;
    print_to_screen("Escribiendo. Seleccionar columna y apretar #:");

    break;
  }
}


void update_write_select_col()
{
  buffer_return_t keypadstate = update_keypad_buffer(selected_col);

  switch (keypadstate)
  {
  case cancel:
    cancel_key();
    break;

  case done:
    // print_to_screen(" Done ");
    LOG("Col:  ");
    LOGN(selected_col);
    estado = write_select_val;

    print_to_screen("Escribiendo. Seleccionar Valor y apretar #");
    break;
  }
}


void update_write_select_val()
{
  buffer_return_t keypadstate = update_keypad_buffer(selected_val);

  switch (keypadstate)
  {
  case cancel:
    cancel_key();
    break;

  case done:
    // print_to_screen(" Done ");
    LOG("Value:  ");
    LOGN(selected_val);
    estado = wait_write;

    print_to_screen("Escribiendo. Pasar Tarjeta");
    break;
  }
}


void update_wait_write()
{
  // See if Cancel
  char pressed_key = "";

  if (get_keypad_key(pressed_key))
  {
    if (pressed_key == '*')
    {
      cancel_key();
    }
  }

  if (WriteRFID(selected_row, selected_col, selected_val))
  {
    LOG("Write [row,col]=[");
    LOG(selected_row);
    LOG(",");
    LOG("]  value: ");
    LOGN(selected_val);
    delay(500);
  }
}


// ==== NUMPAD =================================================================

buffer_return_t update_keypad_buffer(uint8_t& numeric_value)
{
  buffer_return_t return_val;

  char pressed_key = "";

  if (get_keypad_key(pressed_key))
  {
    if (pressed_key == '#')
    {
      // Confirm
      // LOG("Stored ");
      // LOGN(input_string_buffer);

      if (string_count == 1) // length 1
      {
        numeric_value = input_string_buffer.substring(0, 1).toInt();
      }
      else if (string_count == 2)
      {
        numeric_value = input_string_buffer.substring(0, 2).toInt();
      }
      else if (string_count == 0)
      {
        numeric_value = input_string_buffer.toInt();
      }
      return_val = done;

      // Reset
      string_count        = 0;
      input_string_buffer = "000";
    }
    else if (pressed_key == '*')
    {
      // Cancel
      return_val = cancel;
    }
    else
    {
      // Save to buffer
      input_string_buffer.setCharAt(string_count, pressed_key);
      string_count++;
      if (string_count >= string_max_length)
      {
        string_count = 0;
      }
      return_val = not_done;

      LOG("pressed: ");
      LOGN(pressed_key);
    }
  }

  return return_val;
}


bool get_keypad_key(char& pressed_key)
{
  if (!customKeypad.available())
  {
    return false;
  }

  while (customKeypad.available())
  {
    keypadEvent e = customKeypad.read();
    if (e.bit.EVENT == KEY_JUST_RELEASED)
    {
      pressed_key = (char)e.bit.KEY;
      // LOG("pressed: ");
      // LOGN(pressed_key);
      return true;
    }
  }
  return false;
}


// ===============================================================
//            LOOP
// ===============================================================
void print_to_screen(String msg)
{
  LOG("to Screen: ");
  LOGN(msg);
}
