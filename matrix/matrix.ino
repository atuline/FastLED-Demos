
// Matrix
//
// By: funkboxing LED
//
// Updated by: Andrew Tuline
//
// Date: Aug, 2014
//
// Green dots going up the strand, a la 'The Matrix'.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//


#include <FastLED.h>                                           // FastLED library
 
#define LED_DT 13                                              // Data pin
#define NUM_LEDS 24                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  196                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array


// Initialize global variables for sequences
int thisdelay = 50;                                                 // A delay value for the sequence(s)
int thishue = 95;
int thissat = 255;


void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA
} // setup()


void loop () {
  matrix();
} // loop()


void matrix() {                             // ONE LINE MATRIX
  if (random8(0, 100) > 85) {
    leds[0] = CHSV(thishue, thissat, 255);
  }
  else {leds[0] = CHSV(thishue, thissat, 0);}
  for (int i = NUM_LEDS-1; i >0 ; i-- ) {
    leds[i].r = leds[i-1].r;
    leds[i].g = leds[i-1].g;
    leds[i].b = leds[i-1].b;
  }
  show_at_max_brightness_for_power();
  //  FastLED.delay(thisdelay*2.5);
  delay_at_max_brightness_for_power(thisdelay*2.5);
//  delay(thisdelay);
} // matrix()

