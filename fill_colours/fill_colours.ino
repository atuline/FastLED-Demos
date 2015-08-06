
/* fill_colours

By: Andrew Tuline

Date: July, 2015

This boring example provides ways of filling the strand with colours such as:

- fill_solid for a single colour across the entire strip
- fill_gradient for one or more colours across the section of, or the entire strip
- fill_rainbow for a whole enchalada of colours across sections of, or the entire the strip


In order to keep the code simple, you will only see the last colour definition. Uncomment the one you want to see.


References:

http://fastled.io/docs/3.1/group___colorutils.html
https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list

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

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Colours defined for below
long firstval = 0xff00ff;
CRGB rgbval(50,0,500);
CHSV hsvval(150,255,200);
uint8_t startpos = 0;
int endpos = NUM_LEDS-1;

uint8_t thishue = 0;
uint8_t deltahue = 15;


void setup() {
  delay(1000);                                                // Power-up safety delay.
  Serial.begin(57600);                                        // Initialize serial port for debugging.

  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812
  
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
} // setup()


void loop () {                                                // Several different ways to fill the strand with a solid colour.

// fill_solid section 
  fill_solid(leds,NUM_LEDS, firstval);                        // A 'long' RGB value
  fill_solid(leds, NUM_LEDS, CRGB::HotPink);                  // https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list
  fill_solid(leds, NUM_LEDS, rgbval);                         // 8 bit values inside the colour definition
  fill_solid(leds, NUM_LEDS, hsvval);                         // 8 bit values inside the colour definition

// fill_gradient section
  fill_gradient_RGB(leds, startpos, 0x000011, endpos, 0x110000);   // You can mix and match long values and CRGB values. Remember, endpos goes up to NUM_LEDS-1
  fill_gradient_RGB(leds, NUM_LEDS, CRGB(50,0,200), CRGB(80,200,240));  // up to 4 CRGB (or long) values

//FORWARD_HUES, BACKWARD_HUES, SHORTEST_HUES, LONGEST_HUES
  fill_gradient(leds, startpos, CHSV(50, 255,255) , endpos, CHSV(150,255,255), SHORTEST_HUES);
  fill_gradient(leds, NUM_LEDS, CHSV(50, 255,255), CHSV(100,255,255), LONGEST_HUES);    // up to 4 CHSV values

// fill_rainbow section
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
  fill_solid(leds,NUM_LEDS,0);                                // Clear the strip for. . . 
  fill_rainbow(leds+1, NUM_LEDS-2, thishue, deltahue);        // One pixel border at each end.

  show_at_max_brightness_for_power();                         // Power managed display

} // loop()
