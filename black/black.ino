
/* black

By: Andrew Tuline

Date: July, 2015

Please, just turn off the damn colours while I do some programming.

*/


#include "FastLED.h"                                          // FastLED library. Please use the latest development version.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 20                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 0;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

void setup() {
  delay(1000);                                                // Power-up safety delay.

  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812
  
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
} // setup()


void loop () {                                                // Several different ways to fill the strand with a solid colour.
  fill_solid(leds, NUM_LEDS, CRGB::Black);                    // Now, that's black!
  show_at_max_brightness_for_power();                         // Power managed display
} // loop()
