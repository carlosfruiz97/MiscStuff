/*
   Adafruit neopixel:
   - https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
*/

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 189

#define TIEMPO_TOTAL  30.0 // minutos
#define TIEMPO_MS     TIEMPO_TOTAL*60.0*1000.0 // ms
#define DLY_POR_LED   TIEMPO_MS / LED_COUNT

// Declare our NeoPixel strip object:
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

uint32_t red = pixels.Color(255, 0, 0, 150);
void setup() {
  Serial.begin(115200);
  Serial.print("Tiempo Total [ms]: "); Serial.println(TIEMPO_MS);
  Serial.print("Tiempo delay [ms]: "); Serial.println(DLY_POR_LED);
  
  pixels.begin();
  delay(1000);
  pixels.clear();
  pixels.fill(red);
  pixels.show(); // Initialize all pixels to 'off'


}


void loop() {
  //  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for (int i = 0; i < LED_COUNT; i++) { // For each pixel...

    delay(DLY_POR_LED); // Pause before next pass through loop

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, 0, 0, 0, 0);

    pixels.show();   // Send the updated pixel colors to the hardware. 
  }
}
