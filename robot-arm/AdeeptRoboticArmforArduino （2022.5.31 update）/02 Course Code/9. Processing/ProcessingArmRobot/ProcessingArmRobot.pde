/***********************************************************
 File name: AdeeptProcessingArmRobot.pde
 Description: Control the robotic arm to grab objects through the
 upper computer interface written by Processing.
 Website: www.adeept.com
 E-mail: support@adeept.com
 Author: Tom
 Date: 2020/12/12
 ***********************************************************/
import controlP5.*;
import processing.serial.*;

ControlP5 cp5;

controlP5.Button b1;
controlP5.Button b2;
controlP5.Button m1;
controlP5.Button m2;
controlP5.Button m3;
controlP5.Button m4;
controlP5.Button m5;
controlP5.Button m6;
controlP5.Button m7;
controlP5.Button m8;
controlP5.Button m9;
controlP5.Button m10;
controlP5.Button k1;
controlP5.Button k2;
controlP5.Button k3;
controlP5.Button k4;
controlP5.Button k5;
controlP5.Button k6;
controlP5.Button k7;
controlP5.Button k8;
controlP5.Button k9;
controlP5.Button k10;

Textlabel label1;
Textlabel label2;
Textlabel label3;
Textlabel label4;
Textlabel label5;
Textlabel label6;
Textlabel label7;
Textlabel label8;

int buttonValue = 1;

char letter;

boolean isOpen=true;

boolean b1Open, b2Open, Aopen, Sopen, btMute;

boolean toggleValue = false;

float b1x=-300, b1y=190, b2x=-300, b2y=190, m1x=-200, m1y=330, m2x=1400, m2y=330, m3x=-200, m3y=390;
float m4x=1400, m4y=390, m5x=-200, m5y=450, m6x=1400, m6y=450, m7x=-200, m7y=510, m8x=1400, m8y=510, m9x=1400, m9y=570;
float m10x=1400, m10y=570, k1x=-200, k1y=430, k2x=1400, k2y=430, k3x=-200, k3y=430, k4x=1400, k4y=430, k5x=-200, k5y=430;
float k6x=1400, k6y=430, k7x=-200, k7y=430, k8x=1400, k8y=430, k9x=1400, k9y=430, k10x=1400, k10y=430;
float label1x=-600, label1y=600, label2x=-600, label2y=600, label3x=-600, label3y=600, label4x=-600, label4y=380;
float label5x=-600, label5y=380, label6x=1300, label6y=380, label7x=1300, label7y=380, label8x=1300, label8y=380;
Serial port1;

void setup() {

  size(1200, 700);
  smooth();
  // List all the available serial ports:
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  port1 = new Serial(this, Serial.list()[1], 9600);

  // Or you can use the port number directly:
  // port1 = new Serial(this,"COM9", 9600);

  // starts the serial communication
  port1.bufferUntil('\n');

  cp5 = new ControlP5(this);

  b1 = cp5.addButton("b1")
    .setValue(4)
    .setPosition(b1x, b1y)
    .setSize(250, 100);

  b2 = cp5.addButton("b2")
    .setValue(4)
    .setPosition(-300, 190)
    .setSize(200, 100);

  m1 = cp5.addButton("m1")
    .setValue(4)
    .setPosition(-200, 330)
    .setSize(90, 50);

  m2 = cp5.addButton("m2")
    .setValue(4)
    .setPosition(1400, 330)
    .setSize(90, 50);

  m3 = cp5.addButton("m3")
    .setValue(4)
    .setPosition(-200, 390)
    .setSize(90, 50);

  m4 = cp5.addButton("m4")
    .setValue(4)
    .setPosition(1400, 390)
    .setSize(90, 50);

  m5 = cp5.addButton("m5")
    .setValue(4)
    .setPosition(-200, 450)
    .setSize(90, 50);

  m6 = cp5.addButton("m6")
    .setValue(4)
    .setPosition(1400, 450)
    .setSize(90, 50);

  m7 = cp5.addButton("m7")
    .setValue(4)
    .setPosition(-200, 510)
    .setSize(90, 50);

  m8 = cp5.addButton("m8")
    .setValue(4)
    .setPosition(1400, 510)
    .setSize(90, 50);

  m9 = cp5.addButton("m9")
    .setValue(4)
    .setPosition(-200, 570)
    .setSize(90, 50);

  m10 = cp5.addButton("m10")
    .setValue(4)
    .setPosition(1400, 570)
    .setSize(90, 50);

  k1 = cp5.addButton("q")//q
    .setValue(4)
    .setPosition(-200, 430)
    .setSize(90, 90);

  k2 = cp5.addButton("w")//w
    .setValue(4)
    .setPosition(1400, 430)
    .setSize(90, 90);

  k3 = cp5.addButton("e")//e
    .setValue(4)
    .setPosition(-200, 430)
    .setSize(90, 90);

  k4 = cp5.addButton("r")//r
    .setValue(4)
    .setPosition(1400, 430)
    .setSize(90, 90);

  k5 = cp5.addButton("t")//t
    .setValue(4)
    .setPosition(-200, 430)
    .setSize(90, 90);

  k6 = cp5.addButton("y")//y
    .setValue(4)
    .setPosition(1400, 430)
    .setSize(90, 90);

  k7 = cp5.addButton("u")//u
    .setValue(4)
    .setPosition(-200, 430)
    .setSize(90, 90);

  k8 = cp5.addButton("i")//i
    .setValue(4)
    .setPosition(1400, 430)
    .setSize(90, 90);

  k9 = cp5.addButton("o")
    .setValue(4)
    .setPosition(-200, 430)
    .setSize(90, 90);

  k10 = cp5.addButton("p")
    .setValue(4)
    .setPosition(1400, 430)
    .setSize(90, 90);

  PFont pfont = createFont("Arial", 20, true);
  ControlFont font = new ControlFont(pfont, 241);

  b1.getCaptionLabel()//captionLabel
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" keyboard");

  b2.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" mouse");

  m1.getCaptionLabel()
    .setFont(font)
    .setSize(20)
    .toUpperCase(false)
    .setText("Gripper+");

  m2.getCaptionLabel()
    .setFont(font)
    .setSize(20)
    .toUpperCase(false)
    .setText("Gripper-");

  m3.getCaptionLabel()
    .setFont(font)
    .setSize(20)
    .toUpperCase(false)
    .setText("Rotate+");

  m4.getCaptionLabel()
    .setFont(font)
    .setSize(20)
    .toUpperCase(false)
    .setText("Rotate-");

  m5.getCaptionLabel()
    .setFont(font)
    .setSize(20)
    .toUpperCase(false)
    .setText("Elbow+");

  m6.getCaptionLabel()
    .setFont(font)
    .setSize(20)
    .toUpperCase(false)
    .setText("Elbow-");

  m7.getCaptionLabel()
    .setFont(font)
    .setSize(20)
    .toUpperCase(false)
    .setText("Shoulder+");

  m8.getCaptionLabel()
    .setFont(font)
    .setSize(20)
    .toUpperCase(false)
    .setText("Shoulder-");

  m9.getCaptionLabel()
    .setFont(font)
    .setSize(20)
    .toUpperCase(false)
    .setText("Base+");

  m10.getCaptionLabel()
    .setFont(font)
    .setSize(20)
    .toUpperCase(false)
    .setText("Base-");


  k1.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" Q");//A

  k2.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" W");//S

  k3.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" E");//D

  k4.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" R");//F

  k5.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" T");//G

  k6.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" Y");//H

  k7.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" U");//J

  k8.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" I");//K

  k9.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" O");

  k10.getCaptionLabel()
    .setFont(font)
    .setSize(50)
    .toUpperCase(false)
    .setText(" P");

  label1 = cp5.addTextlabel("label1")
    .setText("Keyboard Activated.")
    .setPosition(-600, 600)
    .setColorValue(0xff888888)
    .setFont(createFont("Arial", 50));

  label2 = cp5.addTextlabel("label2")
    .setText("Mouse Activated.")
    .setPosition(-600, 600)
    .setColorValue(0xff888888)
    .setFont(createFont("Arial", 50));

  label4 = cp5.addTextlabel("label4")
    .setText("Gripper")
    .setPosition(-600, 380)
    .setColorValue(0xff888888)
    .setFont(createFont("Arial", 40));

  label5 = cp5.addTextlabel("label5")
    .setText("Rotate")
    .setPosition(-600, 380)
    .setColorValue(0xff888888)
    .setFont(createFont("Arial", 40));

  label6 = cp5.addTextlabel("label6")
    .setText("Elbow")
    .setPosition(1300, 380)
    .setColorValue(0xff888888)
    .setFont(createFont("Arial", 40)); 

  label7 = cp5.addTextlabel("label7")
    .setText("Shoulder")
    .setPosition(1300, 380)
    .setColorValue(0xff888888)
    .setFont(createFont("Arial", 40)); 

  label8 = cp5.addTextlabel("label8")
    .setText("Base")
    .setPosition(1300, 380)
    .setColorValue(0xff888888)
    .setFont(createFont("Arial", 40));
}

void stop()
{
  super.stop() ;
}

void draw() {
  background(0, 0, 0);

  textSize(70);

  fill(169, 169, 169);
  text("Adeept Robotic Arm", 250, 100);
  textSize(32);
  text("www.adeept.com", 850, 650);
  if (isOpen==true) {
    b1x = b1x+(250-b1x)*0.1; 
    b1.setPosition(b1x, b1y);
    b2x = b2x+(750-b2x)*0.1; 
    b2.setPosition(b2x, b2y);
  }
  if (isOpen==true && b2Open==true) {
    m1x = m1x+(500-m1x)*0.1; 
    m1.setPosition(m1x, m1y);
    m2x = m2x+(610-m2x)*0.1; 
    m2.setPosition(m2x, m2y);
    m3x = m3x+(500-m3x)*0.1; 
    m3.setPosition(m3x, m3y);
    m4x = m4x+(610-m4x)*0.1; 
    m4.setPosition(m4x, m4y);
    m5x = m5x+(500-m5x)*0.1; 
    m5.setPosition(m5x, m5y);
    m6x = m6x+(610-m6x)*0.1; 
    m6.setPosition(m6x, m6y);
    m7x = m7x+(500-m7x)*0.1; 
    m7.setPosition(m7x, m7y);
    m8x = m8x+(610-m8x)*0.1; 
    m8.setPosition(m8x, m8y);
    m9x = m9x+(500-m9x)*0.1; 
    m9.setPosition(m9x, m9y);
    m10x = m10x+(610-m10x)*0.1; 
    m10.setPosition(m10x, m10y);
  }
  if (!(isOpen==true && b2Open==true)) {
    m1x = m1x+(-100-m1x)*0.1; 
    m1.setPosition(m1x, m1y);
    m2x = m2x+(1300-m2x)*0.1; 
    m2.setPosition(m2x, m2y);
    m3x = m3x+(-100-m3x)*0.1; 
    m3.setPosition(m3x, m3y);
    m4x = m4x+(1300-m4x)*0.1; 
    m4.setPosition(m4x, m4y);
    m5x = m5x+(-100-m5x)*0.1; 
    m5.setPosition(m5x, m5y);
    m6x = m6x+(1300-m6x)*0.1; 
    m6.setPosition(m6x, m6y);
    m7x = m7x+(-100-m7x)*0.1; 
    m7.setPosition(m7x, m7y);
    m8x = m8x+(1300-m8x)*0.1; 
    m8.setPosition(m8x, m8y);
    m9x = m9x+(-100-m9x)*0.1; 
    m9.setPosition(m9x, m9y);
    m10x = m10x+(1300-m10x)*0.1; 
    m10.setPosition(m10x, m10y);
  }
  if (isOpen==true && b1Open==true) {
    k1x = k1x+(100-k1x)*0.1; 
    k1.setPosition(k1x, k1y);
    k2x = k2x+(200-k2x)*0.1; 
    k2.setPosition(k2x, k2y);
    k3x = k3x+(300-k3x)*0.1; 
    k3.setPosition(k3x, k3y);
    k4x = k4x+(400-k4x)*0.1; 
    k4.setPosition(k4x, k4y);
    k5x = k5x+(500-k5x)*0.1; 
    k5.setPosition(k5x, k5y);
    k6x = k6x+(600-k6x)*0.1; 
    k6.setPosition(k6x, k6y);
    k7x = k7x+(700-k7x)*0.1; 
    k7.setPosition(k7x, k7y);
    k8x = k8x+(800-k8x)*0.1; 
    k8.setPosition(k8x, k8y);
    k9x = k9x+(900-k9x)*0.1; 
    k9.setPosition(k9x, k9y);
    k10x = k10x+(1000-k10x)*0.1; 
    k10.setPosition(k10x, k10y);
    label1x = label1x+(100-label1x)*0.1; 
    label1.setPosition(label1x, label1y);
    label4x = label4x+(140-label4x)*0.1; 
    label4.setPosition(label4x, label4y);
    label5x = label5x+(340-label5x)*0.1; 
    label5.setPosition(label5x, label5y);
    label6x = label6x+(550-label6x)*0.1; 
    label6.setPosition(label6x, label6y);
    label7x = label7x+(730-label7x)*0.1; 
    label7.setPosition(label7x, label7y);
    label8x = label8x+(960-label8x)*0.1; 
    label8.setPosition(label8x, label8y);
  }
  if (!(isOpen==true && b1Open==true)) {
    k1x = k1x+(-200-k1x)*0.1; 
    k1.setPosition(k1x, k1y);
    k2x = k2x+(1300-k2x)*0.1; 
    k2.setPosition(k2x, k2y);
    k3x = k3x+(-100-k3x)*0.1; 
    k3.setPosition(k3x, k3y);
    k4x = k4x+(1300-k4x)*0.1; 
    k4.setPosition(k4x, k4y);
    k5x = k5x+(-100-k5x)*0.1; 
    k5.setPosition(k5x, k5y);
    k6x = k6x+(1300-k6x)*0.1; 
    k6.setPosition(k6x, k6y);
    k7x = k7x+(-100-k7x)*0.1; 
    k7.setPosition(k7x, k7y);
    k8x = k8x+(1300-k8x)*0.1; 
    k8.setPosition(k8x, k8y);
    k9x = k9x+(-100-k9x)*0.1; 
    k9.setPosition(k9x, k9y);
    k10x = k10x+(1300-k10x)*0.1; 
    k10.setPosition(k10x, k10y);
    label1x = label1x+(-600-label1x)*0.1; 
    label1.setPosition(label1x, label1y);
    label4x = label4x+(-600-label4x)*0.1; 
    label4.setPosition(label4x, label4y);
    label5x = label5x+(-600-label5x)*0.1; 
    label5.setPosition(label5x, label5y);
    label6x = label6x+(1300-label6x)*0.1; 
    label6.setPosition(label6x, label6y);
    label7x = label7x+(1300-label7x)*0.1; 
    label7.setPosition(label7x, label7y);
    label8x = label8x+(1300-label8x)*0.1; 
    label8.setPosition(label8x, label8y);
  }
  if (isOpen==true && b2Open==true) {
    label2x = label2x+(100-label2x)*0.1; 
    label2.setPosition(label2x, label2y);
  }
  if (!(isOpen==true && b2Open==true)) {
    label2x = label2x+(-600-label2x)*0.1; 
    label2.setPosition(label2x, label2y);
  } 

  if (b1.isPressed()) {
    b1Open = true;
    b2Open = false;
    port1.write("b");
  }
  if (b2.isPressed()) {
    b1Open = false;
    b2Open = true;
    port1.write("m");
  }

  if (isOpen==true) {
    buttonColor();
    mouseCheck();
  }
  if (isOpen==false) {
    b1Open=false;
    b2Open=true;
    port1.write("z");
  }
}

void keyPressed() {
  if (b1Open == true) {
    if (key >= 'A' && key <= 'z') {
      port1.write(key);
      println(key);
    }
  }
}

void mouseCheck() {
  if (b2Open == true) {
    if (m1.isPressed()) {
      port1.write("q");
      println("Gripper+");
    }
    if (m2.isPressed()) {
      port1.write("w");
      println("Gripper-");
    }
    if (m3.isPressed()) {
      port1.write("e");
      println("Rotate +");
      delay(10);
    }
    if (m4.isPressed()) {
      port1.write("r");   
      println("Rotate -");
      delay(10);
    }
    if (m5.isPressed()) {
      port1.write("t");   
      println("elbow up");
      delay(10);
    }
    if (m6.isPressed()) {
      port1.write("y");   
      println("elbow down");
      delay(10);
    }
    if (m7.isPressed()) {
      port1.write("u");   
      println("sholder up");
      delay(10);
    }
    if (m8.isPressed()) {
      port1.write("i");
      println("shoulder down");
      delay(10);
    }
    if (m9.isPressed()) {
      port1.write("o");
      println("base right");
      delay(10);
    }
    if (m10.isPressed()) {
      port1.write("p");
      println("base left");
      delay(10);
    }
  }
}

void buttonColor() {
  if (b1Open==true) {
    if (keyPressed && key=='q') {//a
      k1.setColorBackground(color(255, 0, 0) );
    } else {
      k1.setColorBackground(color(2, 72, 90));
    }
    if (keyPressed && key=='w') {//s
      k2.setColorBackground(color(255, 0, 0));
    } else {
      k2.setColorBackground(color(2, 72, 90));
    }
    if (keyPressed && key=='e') {//d
      k3.setColorBackground(color(255, 0, 0));
    } else {
      k3.setColorBackground(color(2, 72, 90));
    }
    if (keyPressed && key=='r') {//f
      k4.setColorBackground(color(255, 0, 0));
    } else {
      k4.setColorBackground(color(2, 72, 90));
    }
    if (keyPressed && key=='t') {//g
      k5.setColorBackground(color(255, 0, 0));
    } else {
      k5.setColorBackground(color(2, 72, 90));
    }
    if (keyPressed && key=='y') {//h
      k6.setColorBackground(color(255, 0, 0));
    } else {
      k6.setColorBackground(color(2, 72, 90));
    }
    if (keyPressed && key=='u') {//j
      k7.setColorBackground(color(255, 0, 0));
    } else {
      k7.setColorBackground(color(2, 72, 90));
    }
    if (keyPressed && key=='i') {//k
      k8.setColorBackground(color(255, 0, 0 ));
    } else {
      k8.setColorBackground(color(2, 72, 90));
    }
    if (keyPressed && key=='o') {
      k9.setColorBackground(color(255, 0, 0 ));
    } else {
      k9.setColorBackground(color(2, 72, 90));
    }
    if (keyPressed && key=='p') {
      k10.setColorBackground(color(255, 0, 0 ));
    } else {
      k10.setColorBackground(color(2, 72, 90));
    }
  }
}
