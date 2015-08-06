
/* rainbow_beat for FastLED

By: Andrew Tuline

Date: July, 2015

A rainbow moving back and forth using FastLED function calls with no delays or 'for' loops. Very simple, yet effective.

*/


#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B and BGR for APA102
#define LED_TYPE APA102                                       // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 20                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.



void setup() {
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);         // For WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA
} // setup()


void loop () {
    rainbow_beat();
    show_at_max_brightness_for_power();
} // loop()


void rainbow_beat() {
  uint8_t beatA = beatsin8(5, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(15, 4, 20);                        // Delta hue between LED's
  fill_rainbow(leds, NUM_LEDS, beatA, beatB);                 // Use FastLED's fill_rainbow routine.
} // rainbow_beat()
