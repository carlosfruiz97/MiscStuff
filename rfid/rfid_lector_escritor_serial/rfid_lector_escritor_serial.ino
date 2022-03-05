/* ===============================================================
 * Titulo:
 * Juego:     Plantilla
 *
 * Cliente:
 *
 * Fecha:
 * Autor:
 *
 * Placa:
 *
 * Descripcion:
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
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

// ==== PARAMETERS ===============================================
#define USING_ULTRALIGHT    1

// ==== RFID =====================================================
#define RST_PIN             9      // Configurable, see typical pin layout above
#define SS_PIN              10     // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// ==== FUNCTIONS ================================================
bool GetUid(MFRC522::Uid& foundUid);
void printID(MFRC522::Uid& foundUid);
bool ReadData(byte blockAddr, byte column, byte& result);
void ResetRFID();
bool WriteRFID(byte blockAddr, byte column, byte val);

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
  SPI.begin();                  // Init SPI bus
  mfrc522.PCD_Init();           // Init MFRC522 card
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();

  LOGN("SERIAL COMMANDS: ");
  LOGN("> ID        \t Returns id of card");
  LOGN("> dumpinfo  \t Dups Info");
  LOGN("> R 4 0     \t Reads data in row 4 col 0");
  LOGN("> W 4 0 255 \t Writes 255 in row 4 column 0\n\n");
}


void loop()
{
  if (Serial.available())
  {
    String recieved = Serial.readStringUntil('\n');
    // to uppder case
    recieved.toUpperCase();

    // --- Uid ------------------------------
    if (recieved.indexOf("ID") != -1)
    {
      LOGN("\n > Requesting Uid. PASS CARD");
      MFRC522::Uid foundUid;
      while (!GetUid(foundUid))
      {
        delay(50);  //wait
      }
      LOGN("Card Found");
      printID(foundUid);
    }

    // --- WRITE DATA-------------------------
    if (recieved.indexOf("W") != -1)
    {
      int first_space  = recieved.indexOf(" ", 0);
      int second_space = recieved.indexOf(" ", first_space + 1);
      int third_space  = recieved.indexOf(" ", second_space + 1);

      byte row = recieved.substring(first_space, second_space).toInt();
      byte col = recieved.substring(second_space, third_space).toInt();
      byte val = recieved.substring(third_space).toInt();

      LOG("\n > Writing to row ");
      LOG(row);
      LOG(", col ");
      LOG(col);
      LOG(", value ");
      LOG(val);
      LOGN("\t PASS CARD");

      while (!WriteRFID(row, col, val))
      {
        delay(10);
      }
      LOGN("\t > SUCESS");
    }

    // --- READ DATA -------------------------
    if (recieved.indexOf("R") != -1)
    {
      int first_space  = recieved.indexOf(" ", 0);
      int second_space = recieved.indexOf(" ", first_space + 1);

      byte row = recieved.substring(first_space, second_space).toInt();
      byte col = recieved.substring(second_space).toInt();
      LOG("\n > Reading row ");
      LOG(row);
      LOG(", col ");
      LOG(col);
      LOGN("\t PASS CARD");

      byte res;
      while (!ReadData(row, col, res))
      {
        delay(10);
      }
      LOG("\t > Result: ");
      LOG(res);
      LOG("\t(0x");
      Serial.print(res, HEX);
      LOGN(")");
    }

    if (recieved.indexOf("DUMPINFO") != -1)
    {
      LOGN("\n DUMPING INFO");
      while (!DumpInfo())
      {
        delay(10);
      }
      LOGN("DONE");
    }
  }
}


// ==== DUMP INFO ==============================================
bool DumpInfo()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return false;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return false;
  }
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  LOGN("PICC TYPE");
  return true;
}


// ==== GET RDIF ID ==============================================
bool GetUid(MFRC522::Uid& foundUid)
{
  //    typedef struct {
  //    byte    size;     // Number of bytes in the UID. 4, 7 or 10.
  //    byte    uidByte[10];
  //    byte    sak;      // The SAK (Select acknowledge) byte returned from the PICC after successful selection.
  //  } Uid;

  // Check if RFID present
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return false;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return false;
  }

  foundUid = mfrc522.uid;

  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
  return true;
}


void printID(MFRC522::Uid& foundUid)
{
  LOGN("\t > Card ID (hex): ");
  for (byte i = 0; i < foundUid.size; i++)
  {
    Serial.print(foundUid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(foundUid.uidByte[i], HEX);
  }
  LOGN("");
}


// ==== READ RFID ================================================
bool ReadData(byte blockAddr, byte column, byte& result)
{
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  //some variables we need
  //  byte len = 18;
  byte                buffer1[18];
  byte                len = sizeof(buffer1);
  MFRC522::StatusCode status;

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  {
    // GET TYPE
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

    if (piccType != MFRC522::PICC_TYPE_MIFARE_UL)
    {
      // 1. AUTHENTICATE
      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK)
      {
        LOG(F("Authentication failed: "));
        LOGN(mfrc522.GetStatusCodeName(status));
        ResetRFID();
        return false;
      }
    }
#
    // 2. READ DATA
    status = mfrc522.MIFARE_Read(blockAddr, buffer1, &len);
    if (status != MFRC522::STATUS_OK)
    {
      LOG(F("Reading failed: "));
      LOGN(mfrc522.GetStatusCodeName(status));
      ResetRFID();
      return false;
    }

    // 3. STORE DATA
    result = buffer1[column];

    // mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return true;
  }

  return false;
}


void ResetRFID()
{
  //mfrc522.PCD_Reset();
  LOG("Reset done de RFID");

  LOG("  Estado RFID?: ");
  LOGN(mfrc522.PCD_PerformSelfTest());

  mfrc522.PCD_Init(); // Iniciamos otra vez
}


// ==== WRITE RFID ===============================================
bool WriteRFID(byte blockAddr, byte column, byte val)
{
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    //    LOG("\t\t Card not present");
    return false;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    //    LOG("\t\t Cant Read");
    return false;
  }


  MFRC522::StatusCode status;

  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  if (piccType != MFRC522::PICC_TYPE_MIFARE_UL)
  {
    // ------------- USING NORMAL TAGS ---------------------
    byte dataBlock[] = {
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00
    };
    dataBlock[column] = val;

    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++)
    {
      key.keyByte[i] = 0xFF;
    }

    // 1. AUTHENTICATE USING KEY A
    Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return false;
    }
    else
    {
      Serial.println(F("PCD_Authenticate() success: "));
    }

    // 2. WRITE BLOCK
    status = mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
    if (status != MFRC522::STATUS_OK)
    {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return false;
    }
  }
  else
  {
    // ------------- USING ULTRALIGHT TAGS ---------------------
    byte dataBlock[4] = {
      0x00, 0x00, 0x00, 0x00
    };
    dataBlock[column] = val;

    status = (MFRC522::StatusCode)mfrc522.MIFARE_Ultralight_Write(blockAddr, dataBlock, 4);
    if (status != MFRC522::STATUS_OK)
    {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return false;
    }
  }


  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  return true;
}
