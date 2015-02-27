
/* Moving Dots

By: John Burroughs

Modified by: Andrew Tuline

Date: Oct, 2014

This sequence moves a few LED's at different distances across the strip.

*/



#include "FastLED.h"                                          // FastLED library.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin
#define LED_CK 11                                             // Serial clock pin for APA102 or WS2801
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (APA102, WS2801 or WS@2812B
#define NUM_LEDS 20                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Define variables used by the sequences.
int thisdelay = 4;                                            // A delay value for the sequence(s)
uint8_t count;                                                // Count up to 255 and then reverts to 0
uint8_t fadeval = 224;                                           // Trail behind the LED's. Lower => faster fade.


void setup() {
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);
} // setup()



void loop () {
  dots();
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(thisdelay*2.5);
  Serial.println(LEDS.getFPS());
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

  nscale8(leds,NUM_LEDS,fadeval);                          // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);
} // dots()
