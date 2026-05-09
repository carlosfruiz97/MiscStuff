#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
    Serial.begin(115200);

    SPI.begin();
    rfid.PCD_Init();

    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acerque Tarjeta");

    Serial.println("Sistema listo");
}

#define NB_CHARS 16
char buffer[NB_CHARS + 1]; // +1 para el carácter nulo

void loop() {

    // Esperar a que haya una tarjeta
    if (!rfid.PICC_IsNewCardPresent()) {
        return;
    }

    // Leer la tarjeta
    if (!rfid.PICC_ReadCardSerial()) {
        return;
    }
    // =====================================
    // Generar numero de 5 digitos
    // =====================================

    unsigned long valor = 0;

    for (byte i = 0; i < rfid.uid.size; i++) {

        valor = valor * 256UL;
        valor += rfid.uid.uidByte[i];
    }

    // Limitar a 5 digitos
    uint16_t codigo = valor % 100000;

    // =====================================
    // Mostrar codigo decimal
    // =====================================

    char textoCodigo[6];

    // Siempre 5 digitos
    sprintf(textoCodigo, "%05u", codigo);

    Serial.print("Codigo: ");
    Serial.println(textoCodigo);


    // Detener comunicación con la tarjeta
    rfid.PICC_HaltA();

    // Detener cifrado
    rfid.PCD_StopCrypto1();


    // Volver al mensaje inicial
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(textoCodigo);


    // Mostrar 3 segundos
    delay(3000);

    // Volver al mensaje inicial
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acerque Tarjeta");
}