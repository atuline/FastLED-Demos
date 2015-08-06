/* one_sine_demo

By: Andrew Tuline

Date: Oct, 2014

A demo showing the flexibility of just using a single sine wave. A little code and a lot of variables can go a long way.

*/

#define qsubd(x, b)  ((x>b)?wavebright:0)                   // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                          // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Most of these variables can be mucked around with. Better yet, add some form of variable input or routine to change them on the fly. 1970's here I come. .

// Don't forget to update resetvar() definitions if you change these.
uint8_t wavebright = 128;                                     // You can change the brightness of the waves/bars rolling across the screen. Best to make them not as bright as the sparkles.
uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 0;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
bool thisdir = 0;                                             // You can change direction.
int8_t thisspeed = 4;                                         // You can change the speed, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
int thisdelay = 25;                                           // You can change the delay. Also you can change the allspeed variable above. 
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbri = 32;                                           // Don't go below 16.
// End of resetvar() redefinitions.



void setup() {
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);        // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA
  FastLED.setBrightness(max_bright);
} // setup()



void loop() {
  ChangeMe();                                                 // Muck those variables around.
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    one_sin();
    bgclr++;
  }
  show_at_max_brightness_for_power();
} // loop()



void ChangeMe() {
  uint8_t secondHand = (millis() / 1000) % 60;                // Increase this if you want a longer demo.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  
  // You can change variables, but remember to set them back in the next demo, or they will stay as is.
  if(lastSecond != secondHand) {
    lastSecond = secondHand;
    switch (secondHand) {
      case  0: thisrot = 1; thiscutoff=252; allfreq=8; break; // Both rotating hues
      case  5: thisrot = 0; thisspeed=-4; break;              // Just 1 rotating hue
      case 10: thishue = 255; break;                          // No rotating hues, all red.
      case 15: thisrot = 1; thisspeed=2; break;               // 
      case 20: allfreq = 16; break;                           // Time to make a wider bar.
      case 25: thishue=100; thiscutoff = 96; break;           // Change width of bars.
      case 30: thiscutoff = 96; thisrot = 1; break;           // Make those bars overlap, and rotate a hue
      case 35: thisspeed = 3; break;                          // Change the direction.
      case 40: thiscutoff = 128; wavebright = 64; break;      // Yet more changes
      case 45: wavebright = 128; thisspeed = 3; break;        // Now, we change speeds.
      case 50: thisspeed = -2; break;                         // Opposite directions
      case 55: break;                                         // Getting complicated, let's reset the variables.
    }
  }
} // ChangeMe()



void one_sin() {                                                                // This is the heart of this program. Sure is short.
//  if (thisdir == 0) thisphase+=thisspeed; else thisphase-=thisspeed;          // You can change direction and speed individually.
    thisphase += thisspeed;
    thishue += thisrot;                                                         // Hue rotation is fun for thiswave.
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbri);
    leds[k] += CHSV(thishue, allsat, thisbright);                               // Assigning hues and brightness to the led array.
  }
} // one_sin()


void resetvar() {                                             // Reset the variable back to the beginning.
  wavebright = 128;                                           // You can change the brightness of the waves/bars rolling across the screen. Best to make them not as bright as the sparkles.
  thishue = 0;                                                // You can change the starting hue value for the first wave.
  thisrot = 0;                                                // You can change how quickly the hue rotates for this wave. Currently 0.
  allsat = 255;                                               // I like 'em fully saturated with colour.
  thisdir = 0;                                                // You can change direction.
  thisspeed = 4;                                              // You can change the speed, and use negative values.
  allfreq = 32;                                               // You can change the frequency, thus overall width of bars.
  thisphase = 0;                                              // Phase change value gets calculated.
  thiscutoff = 192;                                           // You can change the cutoff value to display this wave. Lower value = longer wave.
  thisdelay = 25;                                             // You can change the delay. Also you can change the allspeed variable above. 
  bgbri = 16;
} // resetvar()
