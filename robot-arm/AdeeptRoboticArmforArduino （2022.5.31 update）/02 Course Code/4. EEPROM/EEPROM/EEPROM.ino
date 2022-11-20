/***********************************************************
File name: EEprom.ino
Description:   EEprom holds information
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2020/05/22 
***********************************************************/
#include <EEPROM.h>
int a = 0;
void setup()
{
  Serial.begin(115200);
  
  a = EEPROM.read(5);
  if(a == 2)
  {
    Serial.println("Read Succes");  
  }
  else
  {
    Serial.println("Read Failed");  
    EEPROM.write(5,2);
    
  }
  
} 
void loop() 
{
  
}
