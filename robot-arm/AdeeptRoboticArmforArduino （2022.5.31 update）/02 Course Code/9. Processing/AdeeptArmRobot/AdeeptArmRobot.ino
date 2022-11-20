/***********************************************************
File name: AdeeptArmRobot.ino
Description:Control the robotic arm to grab objects through the upper
            computer interface written by Processing.
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2020/12/12
***********************************************************/
#include <Servo.h>
int servopin1 = 9;    //Define servo interface digital interface 9
int servopin2 = 6;    //Define servo interface digital interface 6
int servopin3 = 5;    //Define servo interface digital interface 5
int servopin4 = 3;    //Define servo interface digital interface 3
int servopin5 = 11;   //Define servo interface digital interface 11

int moveServoData;
int servo1Angle=90;
int servo2Angle=90;
int servo3Angle=90;
int servo4Angle=90;
int servo5Angle=90;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
int angle = 90;        //Angle of rotation of the servo

boolean autoLock = false;
//boolean key_mouse_lock = false;
boolean closeLock = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(servopin1,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin2,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin3,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin4,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin5,OUTPUT);//Set the servo interface as the output interface
  Serial.begin(9600);
  servo1.attach(servopin1);
  servo2.attach(servopin2);
  servo3.attach(servopin3);
  servo4.attach(servopin4);
  servo5.attach(servopin5);
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  servo5.write(90); 
  delay(20);
  //servo1Pulse(90);
}
void loop() {
  delay(20);
  do{
    moveServoData = Serial.read();
    if (moveServoData == 111) {//o
      autoLock = false;
      closeLock=false;
      servo1Angle++;
      if(servo1Angle>=180){servo1Angle=180;}
      servo1.write(servo1Angle);
    }
    if (moveServoData == 112){//p
      autoLock = false;
      closeLock=false;
      servo1Angle--;
      if(servo1Angle<=0){servo1Angle=0;}
      servo1.write(servo1Angle);
    }
    if (moveServoData == 117) {//u
      autoLock = false;
      closeLock=false;
      servo2Angle++;
      if(servo2Angle>=180){servo2Angle=180;}
      servo2.write(servo2Angle);
    }
    if (moveServoData == 105) {//i
      autoLock = false;
      closeLock=false;
      servo2Angle--;
      if(servo2Angle<=0){servo2Angle=0;}
      servo2.write(servo2Angle);
    }
    if (moveServoData == 116) {//t
      autoLock = false;
      closeLock=false;
      servo3Angle++;
      if(servo3Angle>=180){servo3Angle=180;}
      servo3.write(servo3Angle);
    }
    if (moveServoData == 121) {//y
      autoLock = false;
      closeLock=false;
      servo3Angle--;
      if(servo3Angle<=0){servo3Angle=0;}
      servo3.write(servo3Angle);
    }



    
    if (moveServoData == 101) {//e
      autoLock = false;
      closeLock=false;
      servo4Angle++;
      if(servo4Angle>=180){servo4Angle=180;}
      servo4.write(servo4Angle);
    }
    if (moveServoData == 114) {//r
      autoLock = false;
      closeLock=false;
      servo4Angle--;
      if(servo4Angle<=0){servo4Angle=0;}
      servo4.write(servo4Angle);
    }
    if(moveServoData == 113) {//q 
      autoLock = false;
      closeLock=false;
      servo5Angle++;
      if(servo5Angle>=90){servo5Angle=90;}
      servo5.write(servo5Angle);
    }
    if(moveServoData == 119) {//w
      autoLock = false;
      closeLock=false;
      servo5Angle--;
      if(servo5Angle<=35){servo5Angle=35;}
      servo5.write(servo5Angle);
    } 
    if(moveServoData == 110){//n
      autoLock = true;
//      key_mouse_lock = false;
      closeLock=false;
    }
  }
  while(Serial.available()>0);

}
