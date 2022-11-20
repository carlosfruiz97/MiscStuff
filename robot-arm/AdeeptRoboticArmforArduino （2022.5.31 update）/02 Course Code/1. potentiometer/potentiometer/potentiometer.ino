 
 /*************************************
File name:potentiometer
Description:Read the value of the potentiometer and convert it to an Angle.
Website:www.addept.com
E-mail:support@addept.com
Author:felix
Date:2020/05/22 
*************************************/
int val1=90;//Read potentiometer 0 data
int val2=90;//Read potentiometer 1 data
int val3=90;//Read potentiometer 2 data
int val4=90;//Read potentiometer 3 data
int val5=90;//Read potentiometer 6 data

void setup() 
{
  Serial.begin(115200);
}
void loop() 
{
    val1 = map(analogRead(0), 0, 1023, 0, 180);  
    val2 = map(analogRead(1), 0, 1023, 0, 180);  
    val3 = map(analogRead(2), 0, 1023, 0, 180);  
    val4 = map(analogRead(3), 0, 1023, 0, 180);
    val5 = map(analogRead(6), 0, 1023, 35, 90);
    Serial.print("A0:");Serial.println(val1);
    Serial.print("A1:");Serial.println(val2);
    Serial.print("A2:");Serial.println(val3);
    Serial.print("A3:");Serial.println(val4);
    Serial.print("A6:");Serial.println(val5);
    Serial.println("************");
    delay(2000);
}
