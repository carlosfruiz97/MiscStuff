/***********************************************************
File name: Learning.ino
Description:The robotic arm performs the motion that to save by pressing 
            the button itself. 
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2020/12/12
***********************************************************/
#include "U8glib.h"
#include <EEPROM.h>
/*I2C*/
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); 
#include <Servo.h>
Servo servo1;//create servo object to control a servo
Servo servo2;//create servo object to control a servo
Servo servo3;//create servo object to control a servo
Servo servo4;//create servo object to control a servo
Servo servo5;//create servo object to control a servo
//The following can be modified according to your specific needs
int dataServo1 = 90; // Servo 1 rotation range(dataServo1=0~180)
int dataServo2 = 90; // Servo 2 rotation range(dataServo2=0~180) 
int dataServo3 = 90; // Servo 3 rotation range(dataServo3=0~180)
int dataServo4 = 90; // Servo 4 rotation range(dataServo4=0~180)
int dataServo5 = 90; // Servo 5 rotation range(dataServo5=35~90)

float dirServo1Offset = 0;    // define a variable for deviation(degree) of the servo
float dirServo2Offset = 0;    // define a variable for deviation(degree) of the servo
float dirServo3Offset = 0;    // define a variable for deviation(degree) of the servo
float dirServo4Offset = 0;    // define a variable for deviation(degree) of the servo
float dirServo5Offset = 0;    // define a variable for deviation(degree) of the servo
int val1=90;//Read potentiometer 1 data
int val2=90;//Read potentiometer 2 data
int val3=90;//Read potentiometer 3 data
int val4=90;//Read potentiometer 4 data
int val5=90;//Read potentiometer 4 data
int number = 10;//Record the number of actions
int oldNumber=0;
char string[4];//OLED display string
char string1[4];//OLED display string
char string2[4];//OLED display string
char string3[4];//OLED display string
char string4[4];//OLED display string
char string5[4];//OLED display string
const int buttonPin = 4;     // the number of the pushbutton pin
/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int addr = 0;
// start reading from the first byte (address 0) of the EEPROM
int address = 0;
int value1=90;
int value2=90;
int value3=90;
int value4=90;
int value5=90;

void setup()
{
  servo1.attach(9);//attachs the servo on pin 9 to servo object
  servo2.attach(6);//attachs the servo on pin 6 to servo object
  servo3.attach(5);//attachs the servo on pin 5 to servo object
  servo4.attach(3);//attachs the servo on pin 3 to servo object
  servo5.attach(11);//attachs the servo on pin 11 to servo object
  
  servo1.write(dataServo1+dirServo1Offset);//goes to dataServo1 degrees 
  servo2.write(dataServo2+dirServo2Offset);//goes to dataServo2 degrees 
  servo3.write(dataServo3+dirServo3Offset);//goes to dataServo3 degrees 
  servo4.write(dataServo4+dirServo4Offset);//goes to dataServo4 degrees 
  servo5.write(dataServo5+dirServo5Offset);//goes to dataServo5 degrees 
 
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) 
    u8g.setColorIndex(255);     // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);         // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);         // pixel on
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
  oldNumber = number*5;
  delay(100);
}

void loop()
{
  int timer=0;
  if(number!=0){
    val1 = map(analogRead(0), 0, 1023, 0, 180);  
    val2 = map(analogRead(1), 0, 1023, 0, 180);  
    val3 = map(analogRead(2), 0, 1023, 0, 180);  
    val4 = map(analogRead(3), 0, 1023, 0, 180);
    val5 = map(analogRead(6), 0, 1023, 35, 90);
  }
  servoMove();
  
  if(digitalRead(buttonPin)==LOW){
      while(digitalRead(buttonPin)==LOW){
        delay(100);
        timer++;
        if(timer>=50){timer=50;}
      }
      if(timer==50){
        number=0;
      }else{
        if(number!=0){
          number=number-1;
        } 
        EEPROM.write(addr, val1);
        EEPROM.write(addr+1, val2);
        EEPROM.write(addr+2, val3);
        EEPROM.write(addr+3, val4);
        EEPROM.write(addr+4, val5);
        addr = addr + 5;  
      }   
  }
  if(number==0){
    value1 = EEPROM.read(address);
    val1 = value1;
    value2 = EEPROM.read(address+1);
    val2 = value2;
    value3 = EEPROM.read(address+2);
    val3 = value3;
    value4 = EEPROM.read(address+3);
    val4 = value4;
    value5 = EEPROM.read(address+4);
    val5 = value5;
    address=address+5;
    if(address==oldNumber){
      address=0;
    }
  }
  itoa(number,string,10);
  itoa(value1,string1,10);
  itoa(value2,string2,10);
  itoa(value3,string3,10);
  itoa(value4,string4,10);
  itoa(value5,string5,10);
  u8g.firstPage();  
  do {
    u8g.drawStr(0,0,"Current state:  ");
    if(number==0){
      u8g.drawStr(0,10,"    Working status  "); 
      u8g.drawStr(0,28,"(1) (2)  (3)  (4) (5)"); 
      u8g.drawStr(0,41,string1);
      u8g.drawStr(27,41,string2);
      u8g.drawStr(54,41,string3);
      u8g.drawStr(81,41,string4);
      u8g.drawStr(109,41,string5);
    }else{
      u8g.drawStr(0,10,"   Learning Status  "); 
      u8g.drawStr(0,30,"remaining steps:     ");
      u8g.drawStr(110,30,string);
    }

    u8g.drawStr(25,54,"www.adeept.com ");  
  } while( u8g.nextPage() );
  delay(5);
}
void servoMove(){
  for(int i=0;i<=180;i++){
    servo1.write(dataServo1+dirServo1Offset);//goes to dataServo1 degrees 
    servo2.write(dataServo2+dirServo2Offset);//goes to dataServo2 degrees 
    servo3.write(dataServo3+dirServo3Offset);//goes to dataServo3 degrees 
    servo4.write(dataServo4+dirServo4Offset);//goes to dataServo4 degrees  
    servo5.write(dataServo5+dirServo5Offset);//goes to dataServo5 degrees  
   
    dataServo1 = val1;
    dataServo2 = val2;
    dataServo3 = val3;
    dataServo4 = val4;
    dataServo5 = val5;

    delay(10);//wait for 0.01second  
  }
}
