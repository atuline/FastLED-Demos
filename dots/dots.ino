
/* Moving Dots for FastLED 2.1

By: John Burroughs

Modified by: Andrew Tuline

Date: Oct, 2014

This sequence moves LED's at different rates across the strip.

FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1

Note: If you receive compile errors (as I have in the Stino add-on for Sublime Text), set the compiler to 'Full Compilation'.

*/



#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 13                                             // Serial data pin for WS2812B or WS2801
#define COLOR_ORDER GRB                                       // Are they RGB, GRB or what??
#define LED_TYPE WS2811                                       // What kind of strip are you using?
#define NUM_LEDS 24                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 255;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Define variables used by the sequences.
int thisdelay = 4;                                            // A delay value for the sequence(s)
uint8_t count;                                                // Count up to 255 and then reverts to 0
uint8_t fader = 16;                                           // Trail behind the LED's. Higher => faster fade.


void setup() {
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);
} // setup()



void loop () {
  dots();
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(thisdelay*2.5);
  LEDS.countFPS();
} // loop()



void dots() {

  uint8_t middle = 0;
  uint8_t side = 0;
  uint8_t other = 0;

  count++;                                                     // overflowing a byte => 0.
  uint8_t x = quadwave8(count);

  other = map(x, 0, 255, NUM_LEDS/4, NUM_LEDS/4*3);            // 1/4 to 3/4 of strip.
  side = map(x, 0, 255, 0, NUM_LEDS-1);                        // full length of strip.
  middle = map(x, 0, 255, NUM_LEDS/3, NUM_LEDS/3*2);           // 1/3 to 2/3 of strip.

  leds[middle] = CRGB::Purple;
  leds[side] = CRGB::Blue;
  leds[other] = CRGB::Aqua;

  for (uint8_t i = 0; i < NUM_LEDS; i++) leds[i].fadeToBlackBy(fader);  // A nice trailing effect.
} // dots()
