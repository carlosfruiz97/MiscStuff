// https://naylampmechatronics.com/blog/35_tutorial-lcd-con-i2c-controla-un-lcd-con-solo-dos-pines.html

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();


  lcd.setCursor(0, 0);
  lcd.print("1:Id 2:Leer");
  lcd.setCursor(0, 1);
  lcd.print("3:Escribir");
}

void loop() {
  //lcd.  scrollDisplayLeft()  ;
  delay(500);
}


/*
  LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows)
  init()
  clear()
  setCursor(col, row)
  print()
  scrollDisplayLeft()
  scrollDisplayRight()
  backlight();
  noBacklight();
  createChar (num, datos)
*/
