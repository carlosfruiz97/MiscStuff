#include <Servo.h>

// OLED test.
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET     4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

// Define the initial angle position of steering gear test.
int pos = 50;
// Define the minimum angle position of steering gear test.
int posMin = 50;
// Define the maximum angle position of steering gear test.
int posMax = 120;

// Uniformly define all pins (test load steering gear).
int pin[] = {9,6,5,3,11,10};
 
// Define the number of test pins.
const int PIN_COUNT = sizeof(pin)/sizeof(pin[0]);
// create servo objects to control servos
Servo s[PIN_COUNT];
// Define control mode: 0, independent control; 1, Full control.
int control = 1;

// Define which steering gear to control from.
int tmpkey = 0;

//It is controlled by the button on the drive board.
const int buttonPin = 4;

// Define flipper(make the steering gear rotate back and forth without returning directly)
int flip = 0;
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  // OLED test.
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);//Sets the font display color
  display.clearDisplay();//cls
}

void loop() {
  int timer=0;
  if(digitalRead(buttonPin)==LOW){
      while(digitalRead(buttonPin)==LOW){
        delay(100);
        timer++;
        if(timer>=50){timer=50;}
      }
      if(timer==50){
        tmpkey=0;
      }else{
          tmpkey++;
          if(tmpkey >= 6) tmpkey = 0;
      }   
  }
  
  if (pos >= posMax) {
    if (control == 0){
      detachOne(); 
    } else {
      tmpkey++;
    }
    if (tmpkey == PIN_COUNT) {
      if (control == 1) detachAll(); 
      tmpkey = 0; 
    }
    flip = 1;
    
    if (control == 0) {
      attachOne(pin[tmpkey]);
      Serial.print("Individually attached: ");
    } else {
      attachAll();
      Serial.print("All attached : ");
    }
    Serial.print(tmpkey);
    Serial.print(" (pin:");
    Serial.print(pin[tmpkey]);
    Serial.print(") control model: ");
    Serial.println(control);
  } else if(pos <= posMin){
    flip = 0;
  }
  moveServo(pos);
  if(flip == 0){
    pos++;
  } else {
    pos--;
  }
  delay(20);

  // OLED test.
  //Set the font size
  display.clearDisplay();
  display.setTextSize(2);
  //Set the display location
  display.setCursor(0,0);
  //String displayed
  display.println(F("OLED OK"));
  display.println(tmpkey);
  //Began to show
  display.display();
}

// Associated single pin
void attachOne(int pin) { 
  s[tmpkey].attach(pin); 
}

// Disassociate a single pin
void detachOne() { 
  s[tmpkey].detach(); 
}

// Associate all pins
void attachAll() { 
  for (int i = 0; i < PIN_COUNT; ++i) { 
    s[i].attach(pin[i]); 
  } 
}

// Disassociate all pins
void detachAll() { 
  for (int i = 0; i < PIN_COUNT; ++i) { 
    s[i].detach(); 
  } 
}

// Drive steering gear
void moveServo(int angle) { 
    s[tmpkey].write(angle); 
}
