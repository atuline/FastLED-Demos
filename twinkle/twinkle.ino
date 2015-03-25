/* twinkle

By: Andrew Tuline

Date: Nov, 2014

A quick demo showing fading LED's. It's easy to start a twinkle at maximum brightness and then fade out. It's a whole other ball game to fade in as well.

*/



#include "FastLED.h"                                          // FastLED library.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// These variables can be changed.
int     ranamount =  50;                                      // The higher the number, lowers the chance for a pixel to light up.
uint8_t thisdelay =  50;                                      // Standard delay value in milliseconds.
uint8_t   fadeval = 224;                                      // Fade rate


void setup() {
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);          // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED power management set at 5V, 500mA
  FastLED.setBrightness(max_bright);
} // setup()


void loop() {
  twinkle();                                                  // Simple call to the routine.
  show_at_max_brightness_for_power();                         // Power managed display is better than FastLED.show()
  delay_at_max_brightness_for_power(thisdelay*2.5);           // Power managed delay.
} // loop()


void twinkle() {
  if (ranamount >NUM_LEDS) ranamount = NUM_LEDS;               // Make sure we're at least utilizing ALL the LED's.
  int idex = random16(0, ranamount);
  if (idex < NUM_LEDS) {                                      // Only the lowest probability twinkles will do.
    leds[idex] = random();                                    // The idex LED is set to a random 32 bit value
  }
  for (int i = 0; i <NUM_LEDS; i++) leds[i].nscale8(fadeval); // Go through the array and reduce each RGB value by a percentage.
} // twinkle()
