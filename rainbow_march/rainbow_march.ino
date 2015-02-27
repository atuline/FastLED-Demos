
/* rainbow_march for FastLED

By: Andrew Tuline

Date: Nov, 2014

Rainbow marching up the strand. Pretty basic, but oh so popular.

*/


#include "FastLED.h"                                          // FastLED library.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 20                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Initialize global variables for sequences
uint8_t thisdelay = 5;                                        // A delay value for the sequence(s)
uint8_t thishue;                                              // Starting hue value.
uint8_t deltahue = 10;                                        // Hue change between pixels.


void setup() {
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // For WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA
} // setup()


void loop () {
  rainbow_march();
  show_at_max_brightness_for_power();                         // Power managed display of LED's.
  delay_at_max_brightness_for_power(2.5*thisdelay);           // Power managed FastLED delay.
} // loop()


void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels
  thishue++;                                                  // Increment the starting hue.
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
} // rainbow_march()
