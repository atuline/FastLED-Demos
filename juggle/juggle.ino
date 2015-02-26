/* Juggle

By: Originally by Mark Kriegsman

Modified By: Andrew Tuline

Date: February 2015

Juggle just moves some balls back and forth.

Note: If you receive compile errors (as I have in the Stino add-on for Sublime Text), set the compiler to 'Full Compilation'.

*/


#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for the strip.
#define LED_TYPE    APA102
#define COLOR_ORDER BGR
#define NUM_LEDS    20


// Initialize changeable global variables.
uint8_t max_bright = 64;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

uint8_t hue = 0;
uint8_t thisdelay = 10;

void setup() {
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(57600);

  //  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);    // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED power management set at 5V, 500mA.
} // setup()

  
void loop() {
  juggle();
  show_at_max_brightness_for_power();                         // Power managed display of LED's.
  FastLED.delay(thisdelay);
} // loop()


void juggle() {                                               // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 192);
  byte dothue = 0;
  for( int i = 0; i < 7; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] += CHSV(dothue, 200, 255);   //beat16 is a FastLED 3.1 function
    dothue += 32;
  }
} // juggle()

