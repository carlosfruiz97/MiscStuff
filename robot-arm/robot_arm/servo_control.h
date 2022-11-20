void attach_servos()
{
  for (uint8_t i = 0; i < NUM_SERVOS; i++)
  {
    servoArry[i].attach(pin_servo[i]);
    servoArry[i].write(estado.x[i]);
  }
}


void readPots()
{
  for (uint8_t i = 0; i < NUM_SERVOS; i++)
  {
    estado.potVals[i] = analogRead(pin_pot[i]);
  }
}


void writeServos()
{
  for (uint8_t i = 0; i < NUM_SERVOS; i++)
  {
    servoArry[i].write(estado.x[i]);
  }
}


void potentiometer_control()
{
  readPots();

  // Map pot value
  for (uint8_t i = 0; i < NUM_SERVOS; i++)
  {
    estado.x[i] = map(estado.potVals[i], 0, 1023, 0, 180);
  }

  writeServos();

  // Update OLED
  display.clearDisplay();//cls

  display.setCursor(0, 0);
  display.println(F("Modo: POT CONTROL"));

  for (uint8_t i = 0; i < NUM_SERVOS; i++)
  {
    display.print(F("- x["));
    display.print(i);
    display.print(F("]: "));
    display.println(estado.x[i]);
  }

  display.display();
}
