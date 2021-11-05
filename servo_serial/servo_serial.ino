/*
  Ejemplo de Como controlar posicion de servo desde el serial
*/

#define DEBUG    1
#if DEBUG == 1
#define LOGN(x)    Serial.println(x)
#define LOG(x)     Serial.print(x)
#else
#define LOG(x)
#define LOGN(x)
#endif


#include <Servo.h>
Servo myservo;

void setup() {
  Serial.begin(115200);
  Serial.println("Send Angle Value 0-180");
  myservo.attach(D2);  // attaches the servo on GIO2 to the servo object
}

void loop()
{
  if (Serial.available())
  {
    int pos = Serial.readStringUntil('\n').toInt();
    LOG("Recived Pos: ");
    LOGN(pos);

    myservo.write(pos);
  }
}
