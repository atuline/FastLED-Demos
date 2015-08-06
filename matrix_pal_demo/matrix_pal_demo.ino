
/* Matrix with Palettes

Originally By: funkboxing LED

Modified a LOT by: Andrew Tuline

Date: July, 2015

Various coloured characters going up/down the strand, a la 'The Matrix'. This highly modified version uses non-blocking
delays as well as colorPalette lookups and other variables such as direction, speed and background colours.

*/


#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 20                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Palette definitions
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;


// Initialize global variables for sequences
int      thisdelay =  50;                                          // A delay value for the sequence(s)
uint8_t    thishue =  95;
uint8_t    thissat = 255;
int        thisdir =   0;
uint8_t thisbright = 255;
bool        huerot =   0;                                          // Does the hue rotate? 1 = yes
uint8_t      bgclr =   0;
uint8_t      bgbri =   0;


void setup() {
  Serial.begin(57600);
  
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // For WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED power management set at 5V, 500mA

  currentPalette  = CRGBPalette16(CRGB::Black);
  targetPalette   = RainbowColors_p;                            // Used for smooth transitioning.
  currentBlending = LINEARBLEND;  
} // setup()



void loop () {
  ChangeMe();

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLISECONDS(thisdelay) {
    matrix();
  }
  show_at_max_brightness_for_power();
} // loop()


void matrix() {                                               // One line matrix

  if (huerot) thishue++;
  
  if (random16(90) > 80) {
    if (thisdir == 0) leds[0] = ColorFromPalette(currentPalette, thishue, thisbright, currentBlending); else leds[NUM_LEDS-1] = ColorFromPalette( currentPalette, thishue, thisbright, currentBlending);
  }
  else {
    if (thisdir ==0) leds[0] = CHSV(bgclr, thissat, bgbri); else leds[NUM_LEDS-1] = CHSV(bgclr, thissat, bgbri);
  }

  if (thisdir == 0) {
    for (int i = NUM_LEDS-1; i >0 ; i-- ) leds[i] = leds[i-1];
  } else {
    for (int i = 0; i < NUM_LEDS ; i++ ) leds[i] = leds[i+1];
  }
} // matrix()


// Preset palettes: RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.

void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  uint8_t secondHand = (millis() / 1000) % 25;                // Change '25' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: thisdelay=50; thishue=95; bgclr=140; bgbri=16; huerot=0; break;
      case  5: targetPalette = OceanColors_p; thisdir=1; bgbri=0; huerot=1; break;
      case 10: targetPalette = LavaColors_p; thisdelay=30; thishue=0; bgclr=50; bgbri=15; huerot=0; break;
      case 15: thisdelay=80; bgbri = 32; bgclr=96; thishue=random8(); break;
      case 20: thishue=random8(); huerot=1; break;
      case 25: break;
    }
  }
} // ChangeMe()
