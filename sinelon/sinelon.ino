
/* Sinelon

By: Mark Kriegsman

Modified by: Andrew Tuline

Date: February 2015

This uses the built in beat in FastLED to move a dot back and forth. In this case, it uses two beats added together for more fun.

Please use FastLED 3.1 or greater.

*/


#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif 

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER BGR                                       // Are they RGB, GRB or what??
#define LED_TYPE APA102                                       // Don't forget to change LEDS.addLeds
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 64;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Define variables used by the sequences.
uint8_t   thisbeat =  23;
uint8_t   thatbeat =  28;
uint8_t   thisfade =   2;                                     // How quickly does it fade? Lower = slower fade rate.
uint8_t    thissat = 255;                                     // The saturation, where 255 = brilliant colours.
uint8_t    thisbri = 255;                                     // Brightness of a sequence.
int          myhue =   0;


void setup() {
  delay(1000);                                                 // Power-up safety delay or something like that.
  Serial.begin(57600);
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);          // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
  
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA.
} // setup()


void loop () {
  sinelon();                                                  // Call our sequence.
  show_at_max_brightness_for_power();                         // Power managed display of LED's.
} // loop()


void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, thisfade);
  int pos1 = beatsin16(thisbeat,0,NUM_LEDS);
  int pos2 = beatsin16(thatbeat,0,NUM_LEDS);
    leds[(pos1+pos2)/2] += CHSV( myhue++/64, thissat, thisbri);
}
