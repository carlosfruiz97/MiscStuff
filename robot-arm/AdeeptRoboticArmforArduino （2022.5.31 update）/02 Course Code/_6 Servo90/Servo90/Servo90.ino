/***********************************************************
File name: AdeeptArmInitializationCode.ino
Description: Power on, the servo rotates 90 to straighten the robotic arm
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2019/04/26
***********************************************************/
#include <Servo.h>
int servopin1 = 9;    //Define servo interface digital interface 9
int servopin2 = 6;    //Define servo interface digital interface 6
int servopin3 = 5;    //Define servo interface digital interface 5
int servopin4 = 3;    //Define servo interface digital interface 3
int servopin5 = 11;   //Define servo interface digital interface 11

int moveServoData;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
int angle = 90;        //Angle of rotation of the servo

void setup() {
  // put your setup code here, to run once:
  pinMode(servopin1,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin2,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin3,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin4,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin5,OUTPUT);//Set the servo interface as the output interface
  Serial.begin(9600);
}
void loop()
{
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
