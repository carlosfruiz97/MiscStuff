/***********************************************************
File name: AdeeptSimulation.ino
Description:Robotic arm imitates short-time motion, such as mobile
            phone page turning, keyboardtapping.
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2020/12/12
***********************************************************/
#include "U8glib.h"
#include <EEPROM.h>
#include <Servo.h>

//The number of simulations is set here
int number = 5;//Record the number of actions

/*I2C*/
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); 
Servo servo1;//create servo object to control a servo
Servo servo2;//create servo object to control a servo
Servo servo3;//create servo object to control a servo
//The following can be modified according to your specific needs
int dataServo1 = 180; // Servo 1 rotation range(dataServo1=0~180)
int dataServo2 = 180; // Servo 2 rotation range(dataServo2=0~180) 
int dataServo3 = 180; // Servo 3 rotation range(dataServo3=0~180)

float dirServo1Offset = 0;    // define a variable for deviation(degree) of the servo
float dirServo2Offset = 0;    // define a variable for deviation(degree) of the servo
float dirServo3Offset = 0;    // define a variable for deviation(degree) of the servo
int val1;//Read potentiometer 1 data
int val2;//Read potentiometer 2 data
int val3;//Read potentiometer 3 data
int oldNumber=0;
char string[4];//OLED display string
char string1[4];//OLED display string
char string2[4];//OLED display string
char string3[4];//OLED display string
const int buttonPin = 4;     // the number of the pushbutton pin
/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int addr = 0;
// start reading from the first byte (address 0) of the EEPROM
int address = 0;
int value1=0;
int value2=0;
int value3=0;

void setup()
{
  //Serial.begin(9600);
  servo1.attach(9);//attachs the servo on pin 9 to servo object
  servo2.attach(6);//attachs the servo on pin 6 to servo object
  servo3.attach(5);//attachs the servo on pin 5 to servo object
  servo1.write(dataServo1+dirServo1Offset);//goes to dataServo1 degrees 
  servo2.write(dataServo2+dirServo2Offset);//goes to dataServo2 degrees 
  servo3.write(dataServo3+dirServo3Offset);//goes to dataServo3 degrees 
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
  oldNumber = number*3;
}
void loop()
{
  int timer=0;
  servoMove();
  if(number!=0){
    val1 = map(analogRead(0), 0, 1023, 0, 180);  
    val2 = map(analogRead(1), 0, 1023, 0, 180);  
    val3 = map(analogRead(2), 0, 1023, 0, 180);  
  }
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
          //number=number-1;
          number--;
        } 
        EEPROM.write(addr++, val1);
        EEPROM.write(addr++, val2);
        EEPROM.write(addr++, val3);
        //addr = addr + 3;  
        //Serial.println(addr);
      }   
  }
  if(number==0){
    value1 = EEPROM.read(address++);
    val1 = value1;
    value2 = EEPROM.read(address++);
    val2 = value2;
    value3 = EEPROM.read(address++);
    val3 = value3;
    //address=address+3;
    if(address==oldNumber){
      address=0;
    }
  }
  itoa(number,string,10);
  itoa(value1,string1,10);
  itoa(value2,string2,10);
  itoa(value3,string3,10);
  u8g.firstPage();  
  do {
    u8g.drawStr(0,0,"Current state:  ");
    if(number==0){
      u8g.drawStr(0,10,"    Working status  "); 
      u8g.drawStr(0,28,"(1) (2)  (3)  "); 
      u8g.drawStr(0,41,string1);
      u8g.drawStr(27,41,string2);
      u8g.drawStr(54,41,string3);
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
  int cntMax=0;
  cntMax = abs(dataServo1-val1);
  if(cntMax<abs(dataServo2-val2)){
    cntMax = abs(dataServo2-val2);  
  }
  if(cntMax<abs(dataServo3-val3)){
    cntMax=abs(dataServo3-val3);
  }
  for(int i=0;i<=cntMax;i++){
    servo1.write(dataServo1+dirServo1Offset);//goes to dataServo1 degrees 
    servo2.write(dataServo2+dirServo2Offset);//goes to dataServo2 degrees 
    servo3.write(dataServo3+dirServo3Offset);//goes to dataServo3 degrees 
   
    if(dataServo1>val1){dataServo1--;  }
    if(dataServo1<val1){dataServo1++;  }
    if(dataServo1>180) {dataServo1=180;}
    if(dataServo1<0)   {dataServo1=0;  }
  
    if(dataServo2>val2){dataServo2--;  }
    if(dataServo2<val2){dataServo2++;  }
    if(dataServo2>180) {dataServo2=180;}
    if(dataServo2<0)   {dataServo2=0;  }
  
    if(dataServo3>val3){dataServo3--;  }
    if(dataServo3<val3){dataServo3++;  }
    if(dataServo3>180) {dataServo3=180;}
    if(dataServo3<0)   {dataServo3=0;  }

    delay(30);//wait for 0.002second  
  }
}
