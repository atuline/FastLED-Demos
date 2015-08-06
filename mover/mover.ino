
/* mover

By: Andrew Tuline

Date: February 2015

This is a simple pixel moving routine for those of you that just like to count pixels on a strand.
I prefer to use sine waves as they provide a LOT of flexibility with less code for moving pixels around.

You should be able to add more variables, such as hues, hue rotation, direction and so forth.

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
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Define variables used by the sequence.

uint8_t thisdelay = 100;                                      // A delay value for the sequence(s).
uint8_t  thisfade = 192;                                      // How quickly does it fade? Lower = slower fade rate.


void setup() {
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);        // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED power management set at 5V, 500mA.
} // setup()


void loop () {
  ChangeMe();                                                 // Check the demo loop for changes to the variables.
  mover();                                                    // Call our sequence.
} // loop()


void mover() { 
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] += CHSV(hue, 255, 255);
    leds[(i+5) % NUM_LEDS] += CHSV(hue+85, 255, 255);         // We use modulus so that the location is between 0 and NUM_LEDS
    leds[(i+10) % NUM_LEDS] += CHSV(hue+170, 255, 255);       // Same here.
    show_at_max_brightness_for_power();
    fadeToBlackBy(leds, NUM_LEDS, thisfade);                  // Low values = slower fade.
    delay(thisdelay);                                         // UGH!!!! A blocking delay. If you want to add controls, they may not work reliably.
  }
} // mover()


void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  uint8_t secondHand = (millis() / 1000) % 15;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: thisdelay=20; thisfade=240; break;             // You can change values here, one at a time , or altogether.
      case  5: thisdelay=50; thisfade=128; break;
      case 10: thisdelay=100; thisfade=64; break;             // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 15: break;
    }
  }
} // ChangeMe()