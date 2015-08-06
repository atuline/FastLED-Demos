
/* fastcirc_beat

By: Andrew Tuline

Date: Aug, 2015


Single LED moving increasingly faster across the strand using beats instead of counting.

My original fastcirc routine counted LED's and had a few if statements, an else and some for loops. This has none of that.

Why count LED's when you can use sinewaves to do the heavy lifing for you.

Of course, you can modify this routine to add a demo loop, rotate hues, add a background colour and so forth.

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
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];  


// Initialize global variables for sequences
int thiscount = 0;
int thissat = 255;
int thishue = 0;
int thisbright = 255;
int thisdir = 1;


void setup() {
  delay(1000);                                                // Power-up safety delay.
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812
  
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
} // setup()


void loop () {
  uint8_t beatval = beatsin8(10,5,50);
  fast_circ();
  delay(beatval*2);                                           // I tried to use EVERY_N_MILLISECONDS, but it didn't work for some reason.
  show_at_max_brightness_for_power();
} // loop()


void fast_circ() {                                            // Fast Circle
  thiscount = (thiscount + thisdir)%NUM_LEDS;
  leds[thiscount] = CHSV(thishue, thissat, thisbright);
  fadeToBlackBy(leds, NUM_LEDS, 224);
} // fast_circ()

