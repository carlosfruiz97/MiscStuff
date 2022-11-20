/***********************************************************
File name: WritingAndDrawing.ino
Description:Control the robotic arm to write and draw through the upper 
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

int moveServoData;
int servo1Angle=180;
int servo2Angle=180;
int servo3Angle=180;
Servo servo1;
Servo servo2;
Servo servo3;
int angle = 180;        //Angle of rotation of the servo
boolean autoLock = false;
//boolean key_mouse_lock = false;
boolean closeLock = false;
String comdata = "090045135";
int mark=0;
int starter=1;
int origin=0;
int numdata[3] = {90,135,45};

void setup() {
  // put your setup code here, to run once:
  pinMode(servopin1,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin2,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin3,OUTPUT);//Set the servo interface as the output interface
  servo1.attach(servopin1);
  servo2.attach(servopin2);
  servo3.attach(servopin3);
  servo1.write(180);
  servo2.write(180);
  servo3.write(180); 
  delay(20);
  Serial.begin(115200);
}
void loop() {
   int tim=100;  
   while (Serial.available() > 0){
      comdata += char(Serial.read());
      delay(2);
      mark=1;  
   }
   if(mark==1){
      mark=0;
      numdata[0]=(comdata[0]-'0')*100+(comdata[1]-'0')*10+(comdata[2]-'0');
      numdata[1]=(comdata[3]-'0')*100+(comdata[4]-'0')*10+(comdata[5]-'0');
      numdata[2]=(comdata[6]-'0')*100+(comdata[7]-'0')*10+(comdata[8]-'0');
      delay(10);
      comdata = String("");
    if(numdata[0]==0&&numdata[1]==0&&numdata[2]==0){
      servo1.write(90);
      servo2.write(45);
      servo3.write(135);
      starter=0;
      origin=0;
    }else if(numdata[0]==1&&numdata[1]==1&&numdata[2]==1&&origin==0){
      for(int i=180;i>=90;i--){
        servo1.write(i);
        servo2.write(i);
        servo3.write(i);
        delay(30);
      }
      int j=90;
      for(int i=90;i>=45;i--){
        j++;
        servo2.write(i);
        servo3.write(j);
        delay(30);
      }
      servo1.write(90);
      servo2.write(45);
      servo3.write(135);
      origin=1;
      starter=0;
      }else{
        if(starter==0&&origin==0){
           starter=1;
          //Move the first servo
          if(numdata[0]>=90){
            for(int i=0;i<=numdata[0]-90;i++){      
               servo1.write(90-i);
               delay(20);
            }
          }else{
            for(int i=0;i<90-numdata[0];i++){      
               servo1.write(90+i);
               delay(20);
            }
          }
          //Move the third servo
          if(numdata[1]>=135){
            for(int i=0;i<=numdata[1]-135;i++){      
               servo3.write(135+i);
                delay(20);
            }
          }else{
            for(int i=0;i<135-numdata[1];i++){     
               servo3.write(135-i);
                delay(20);
            }
          }
          //Move the second servo
          if(numdata[2]>=45){
            for(int i=0;i<=numdata[2]-45;i++){      
               servo2.write(45+i);
               delay(20);
            }
          }else{
            for(int i=0;i<45-numdata[2];i++){      
               servo2.write(45-i);
                delay(20);
            }
          }
        }else{
          if(origin==0){
          //starter=1;
          servo1.write(180-numdata[0]);
          servo2.write(numdata[2]);
          servo3.write(numdata[1]);
          delay(2);
          }
        }
      }
    }
}
