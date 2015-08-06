
/* one_sine for FastLED 3.1 and above

By: Andrew Tuline

Date: Jan, 2015

This takes my one_sine routine and adds Mark's palette lookup to it.

*/


// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?wavebright:0)                   // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                          // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for WS2812 or WS2801.
#define LED_CK 11                                             // Serial clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // Are they GRB for WS2812 and GBR for APA102
#define LED_TYPE APA102                                       // What kind of strip are you using? WS2812, APA102. . .
#define NUM_LEDS 20                                           // Number of LED's.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.


// Initialize changeable global variables. Play around with these!!!
uint8_t wavebright = 128;                                     // You can change the brightness of the waves/bars rolling across the screen.
uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
int8_t thisspeed = 8;                                         // You can change the speed of the wave, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus distance between bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
int thisdelay = 30;                                           // You can change the delay. Also you can change the allspeed variable above. 
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbright = 10;                                        // Brightness of background colour

// Palette definitions
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;


void setup() {
  Serial.begin(57600);

  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   //WS2801 and APA102
//  LEDS.addLeds<LED_TYPE, LED_DT,COLOR_ORDER>(leds, NUM_LEDS);   // WS2812

  FastLED.setBrightness(max_bright);

  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA. This is OPTIONAL!!!

// Also, RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p
  currentPalette = LavaColors_p;
  currentBlending = LINEARBLEND;
} // setup()


void loop () {

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    one_sine_pal(startIndex);
  }
  show_at_max_brightness_for_power();
} // loop()


void one_sine_pal(uint8_t colorIndex) {                                                              // This is the heart of this program. Sure is short.
  thisphase += thisspeed;                                                     // You can change direction and speed individually.
  thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  for (int k=0; k<NUM_LEDS-1; k++) {                                          // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);    // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbright);                                     // First set a background colour, but fully saturated.
    leds[k] += ColorFromPalette( currentPalette, colorIndex, thisbright, currentBlending);    
    colorIndex +=3;
  }
  bgclr++;                                                                    // You can change the background colour or remove this and leave it fixed.
} // one_sine_pal()
