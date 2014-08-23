
// TITLE
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// This is a simple FastLED (2.1 and greater) display sequence template.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//


#include <FastLED.h>                                           // FastLED library


#define LED_DT 13                                              // Data pin
#define NUM_LEDS 24                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  32                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];                                    // Initialize our array

int thisdelay; 
uint8_t thissat;
uint8_t thishue;

// Initialize global variables for sequences

void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA

  thisdelay = 200; thissat = 255; thishue = 95;

} // setup()


void loop () {
  matrix();
} // loop()


void matrix() {
  if (random8(0, 100) > 80) { 
    leds[0] = CHSV(thishue, thissat, 255);
  } else {
    leds[0] = CHSV(thishue, thissat, 0);
  }

  for (int i = NUM_LEDS-1; i >0 ; i-- ) {
    leds[i].r = leds[i-1].r;
    leds[i].g = leds[i-1].g;
    leds[i].b = leds[i-1].b;
  }

  LEDS.show();
  delay(thisdelay);
}
