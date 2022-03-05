/*
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Esp8266
 *             Reader/PCD   Uno/101       Mega      Nano v3    Nodemcu
 * Signal      Pin          Pin           Pin       Pin        Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         D3
 * SPI SS      SDA(SS)      10            53        D10        D4
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        D7
 * SPI MISO    MISO         12 / ICSP-1   50        D12        D6
 * SPI SCK     SCK          13 / ICSP-3   52        D13        D5
 * Propiedad de @logicprops
 *
 *
 * // Iniciar asi :
 * // ==== RFID =====================================================
 * #define RST_PIN             D3     // Configurable, see typical pin layout above
 * #define SS_PIN              D4     // Configurable, see typical pin layout above
 * #define USING_ULTRALIGHT    1
 * MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
 * #include "MFRC522_interface.h"
 *
 * void setup()
 * {
 * // -- RFID --
 * SPI.begin();                  // Init SPI bus
 * mfrc522.PCD_Init();           // Init MFRC522 card
 * delay(4);
 * mfrc522.PCD_DumpVersionToSerial();
 * }
 */

// ==== CONSTANTS ================================================
#define TARJETA_MASTER    0xFF

// ==== FUNCTIONS ================================================

/*  ----- ResetRFID -----------------------------------------
 * Setup function
 */
void setup_mfrc522();


/*  ----- ResetRFID -----------------------------------------
 * Resets the RFID reader
 */
void ResetRFID();

/*  ----- Read RFID Data -----------------------------------------
 * input:
 * - byte blockAddr     row in RFID data adress
 * - byte column        column in RFID data adress
 * - byte & result      Result is stored here
 * outputs:
 * -  bool               successful read?
 */
bool ReadData(byte blockAddr, byte column, byte& result);

/*  ----- WriteRFID -----------------------------------------
 * input:
 * - byte blockAddr     row in RFID data adress
 * - byte column        column in RFID data adress
 * - byte val           Value to be stored
 * outputs:
 * -  bool               successful write?
 */
bool WriteRFID(byte blockAddr, byte column, byte val);

/*  ----- GetUid -----------------------------------------
 * input:
 * - MFRC522::Uid & foundUid    Results of the UID read, is modified inside
 * outputs:
 * -  bool               successful read?
 *
 * Example:
 * MFRC522::Uid foundUid;
 * if (GetUid(foundUid))
 * {
 *  // something ...
 *  printUid(foundUid);
 * }
 */
bool GetUid(MFRC522::Uid& foundUid);

/*  ----- printUid -----------------------------------------
 * input:
 * - MFRC522::Uid & foundUid    Prints Uid found to serial
 *
 * Example:
 * MFRC522::Uid foundUid;
 * if (GetUid(foundUid))
 * {
 *  // something ...
 *  printUid(foundUid);
 * }
 */
void printUid(MFRC522::Uid& foundUid);

/*  ----- DumpInfo -----------------------------------------
 * Dumpts info of card found
 */
bool DumpPiccInfo();



// ===============================================================
//            RFID
// ===============================================================

/*  ----- ResetRFID -----------------------------------------
 * Setup function
 */
void setup_mfrc522()
{
  SPI.begin();                  // Init SPI bus
  mfrc522.PCD_Init();           // Init MFRC522 card
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
}


/*  ----- Read RFID Data -----------------------------------------
 * input:
 * - byte blockAddr     row in RFID data adress
 * - byte column        column in RFID data adress
 * - byte & result      Result is stored here
 * outputs:
 * -  bool               successful read?
 */
bool ReadData(byte blockAddr, byte column, byte& result)
{
  // Check If New Card is present
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
  {
    return false;
  }


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


/*  ----- ResetRFID -----------------------------------------
 * Resets the RFID reader
 */
void ResetRFID()
{
  //mfrc522.PCD_Reset();
  LOG("Reset done de RFID");

  LOG("  Estado RFID?: ");
  LOGN(mfrc522.PCD_PerformSelfTest());

  mfrc522.PCD_Init(); // Iniciamos otra vez
}


/*  ----- GetUid -----------------------------------------
 * input:
 * - MFRC522::Uid & foundUid    Results of the UID read, is modified inside
 * outputs:
 * -  bool               successful read?
 *
 * Example:
 * MFRC522::Uid foundUid;
 * if (GetUid(foundUid))
 * {
 *  // something ...
 *  printUid(foundUid);
 * }
 */
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


/*  ----- printUid -----------------------------------------
 * input:
 * - MFRC522::Uid & foundUid    Prints Uid found to serial
 *
 * Example:
 * MFRC522::Uid foundUid;
 * if (GetUid(foundUid))
 * {
 *  // something ...
 *  printUid(foundUid);
 * }
 */
void printUid(MFRC522::Uid& foundUid)
{
  Serial.print("> Card ID (hex):\t ");
  for (byte i = 0; i < foundUid.size; i++)
  {
    Serial.print(foundUid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(foundUid.uidByte[i], HEX);
  }
  Serial.println("");
}


/*  ----- DumpInfo -----------------------------------------
 * Dumpts info of card found
 */
bool DumpPiccInfo()
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

  Serial.println("PICC TYPE");
  return true;
}


/*  ----- WriteRFID -----------------------------------------
 * input:
 * - byte blockAddr     row in RFID data adress
 * - byte column        column in RFID data adress
 * - byte val           Value to be stored
 * outputs:
 * -  bool               successful write?
 */
bool WriteRFID(byte blockAddr, byte column, byte val)
{
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
  {
    //    LOG("\t\t Card not present");
    return false;
  }

  MFRC522::StatusCode status;
  MFRC522::PICC_Type  piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  if (piccType != MFRC522::PICC_TYPE_MIFARE_UL)
  {
    // ------------- USING NORMAL TAGS ---------------------
    byte dataBlock[] =
    {
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
    byte dataBlock[4] =
    {
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
