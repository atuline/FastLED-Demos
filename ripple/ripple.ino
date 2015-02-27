
/* ripple

Converted and significantly modified by: Andrew Tuline

Date: Oct, 2014

A cool ripple effect for an LED strip that's been re-written from the Neopixel version https://gist.github.com/suhajdab/9716635

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


uint8_t colour;                                               // Ripple colour is randomized.
int center = 0;                                               // Center of the current ripple.
int step = -1;                                                // -1 is the initializing step.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps 16                                           // Case statement wouldn't allow a variable.

uint8_t bgcol = 0;                                            // Background colour rotates.
int thisdelay = 50;                                           // Standard delay value.


 
void setup() {
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);          // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);
} // setup()



void loop () {
  ripple();
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(thisdelay*2.5);
} // loop()



void ripple() {
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(bgcol++, 255, 50);  // Rotate background colour.

  switch (step) {

    case -1:                                                          // Initialize ripple variables.
      center = random(NUM_LEDS);
      colour = random16(0,256);
      step = 0;
      break;

    case 0:
      leds[center] = CHSV(colour, 255, 255);                          // Display the first pixel of the ripple.
      step ++;
      break;

    case maxsteps:                                                    // At the end of the ripples.
      step = -1;
      break;

    default:                                                          // Middle of the ripples.
        leds[wrap(center + step)] += CHSV(colour, 255, myfade/step*2);   // Display the next pixels in the range for one side.
        leds[wrap(center - step)] += CHSV(colour, 255, myfade/step*2);   // Display the next pixels in the range for the other side.
        step ++;                                                      // Next step.
        break;  
  } // switch step
} // ripple()
 
 
 
int wrap(int step) {
  if(step < 0) return NUM_LEDS + step;
  if(step > NUM_LEDS - 1) return step - NUM_LEDS;
  return step;
} // wrap()
