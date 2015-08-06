/* two_sin_pal_demo

By: Andrew Tuline

Date: July, 2015

Two moving waves of different colours. In this case, the wave values are either '1' or '0', thus really bars.
The cool thing about this routine is that there are lot of configurable items without requiring a lot of complex code. It uses
sine waves to create 'bars' and not lengthy/complex 'marching' code.


With a few potentiometers or other inputs, you could change several values:

- Change the width of each wave
- Change the speed
- Change the direction
- Change the overall frequency
- Change the colour of each wave or even change the hue rotation speed.
- You could even get real funky and have different frequencies and phase changes for the waves. Epilepsy alert!

This would make for an awesome interactive display for youth.


*/


#include "FastLED.h"                                          // FastLED library. Please use the latest development version.

#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0


#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // Are they GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 20                                           // Number of LED's.

CRGB leds[NUM_LEDS];

unsigned long previousMillis;                                 // Store last time the strip was updated.

// Most of these variables can be mucked around with. Better yet, add some form of variable input or routine to change them on the fly. 1970's here I come. . 
//

uint8_t brightness = 128;

uint8_t wavebright = 128;                                     // You can change the brightness of the waves/bars rolling across the screen. Best to make them not as bright as the sparkles.
uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thathue = 140;                                        // You can change the starting hue for other wave.
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t thatrot = 1;                                          // You can change how quickly the hue rotates for the other wave. Currently 0.
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
uint8_t thisdir = 0;
uint8_t thatdir = 0;
uint8_t alldir = 0;                                           // You can change direction.
int8_t thisspeed = 4;                                         // You can change the speed.
int8_t thatspeed = 4;                                         // You can change the speed.
uint8_t allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
int thisphase = 0;                                            // Phase change value gets calculated.
int thatphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
uint8_t thatcutoff = 192;                                     // You can change the cutoff value to display that wave. Lower value = longer wave.
int thisdelay = 20;

uint8_t fadeval = 192;


CRGBPalette16 thisPalette;
CRGBPalette16 thatPalette;
TBlendType    currentBlending;        // NOBLEND or LINEARBLEND


void setup() {
  delay(1000);                                                 // Power-up safety delay or something like that.
  Serial.begin(57600);

  FastLED.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // WS2801 and APA102
//  FastLED.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);       // WS2812

  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED power management set at 5V, 500mA
  FastLED.setBrightness(brightness);

  // Also, RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p
  thisPalette = RainbowColors_p;
  thatPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
} // setup()


void loop() {
  ChangeMe();
  EVERY_N_MILLISECONDS(thisdelay) {
    two_sin();                                                // Routine is still delay based, but at least it's now a non-blocking day.
  }
  show_at_max_brightness_for_power();
} // loop()


void two_sin() {

//  if (alldir == 0) {thisphase+=thisspeed; thatphase+=thatspeed;}        // You can change direction.
//  else {thisphase-=thisspeed; thatphase-=thatspeed;}                    // You can have individual speeds.

    thisdir ? thisphase += beatsin8(thisspeed, 2, 10) : thisphase -= beatsin8(thisspeed, 2, 10);
    thatdir ? thatphase += beatsin8(thisspeed, 2, 10) : thatphase -= beatsin8(thatspeed, 2, 10);
    thatphase += beatsin8(thatspeed, 2, 10);
    thishue += thisrot;                                        // Hue rotation is fun for thiswave.
    thathue += thatrot;                                        // It's also fun for thatwave.
  
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsuba(cubicwave8((k*allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int thatbright = qsuba(cubicwave8((k*allfreq)+128+thatphase), thatcutoff);  // This wave is 180 degrees out of phase (with the value of 128).

    leds[k] = ColorFromPalette(thisPalette, thishue, thisbright, currentBlending);
    leds[k] += ColorFromPalette(thatPalette, thathue, thatbright, currentBlending);
  }
     nscale8(leds,NUM_LEDS,fadeval);
} // sinmov8()


// RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, ForestColors_p, and PartyColors_p.
void ChangeMe() {
  uint8_t secondHand = (millis() / 1000) % 60;                // Increase this if you want a longer demo.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    switch (secondHand) {
      case  0: thisrot = 1; thatrot = 1; thisPalette=PartyColors_p; thatPalette=PartyColors_p; break;
      case  5: thisrot = 0; thatdir = 1; thatspeed = -4; thisPalette=ForestColors_p; thatPalette=OceanColors_p; break;
      case 10: thatrot = 0; thisPalette=PartyColors_p; thatPalette=RainbowColors_p; break;
      case 15: allfreq = 16; thisdir = 1; thathue = 128; break;
      case 20: thiscutoff = 96; thatcutoff = 240; break;
      case 25: thiscutoff = 96; thatdir = 0; thatcutoff = 96; thisrot = 1; break;
      case 30: thisspeed= -4; thisdir = 0; thatspeed= -4; break;
      case 35: thiscutoff = 128; thatcutoff = 128; wavebright = 64; break;
      case 40: wavebright = 128; thisspeed = 3; break;
      case 45: thisspeed = 3; thatspeed = -3; break;
      case 50: thisspeed = 2; thatcutoff = 96; thiscutoff = 224; thatspeed = 3; break;
      case 55: resetvar(); break;
      case 60: break;
    }
  }
} // ChangeMe()


void resetvar() {                   // Reset the variable back to the beginning.
  wavebright = 128;                    // You can change the brightness of the waves/bars rolling across the screen. Best to make them not as bright as the sparkles.
  thishue = 0;                         // You can change the starting hue value for the first wave.
  thathue = 140;                       // You can change the starting hue for other wave.
  thisrot = 0;                         // You can change how quickly the hue rotates for this wave. Currently 0.
  thatrot = 0;                         // You can change how quickly the hue rotates for the other wave. Currently 0.
  allsat = 255;                        // I like 'em fully saturated with colour.
  thisspeed = 4;                       // You can change the speed, and use negative values.
  thatspeed = 4;                       // You can change the speed, and use negative values.
  allfreq = 32;                        // You can change the frequency, thus overall width of bars.
  thisphase = 0;                       // Phase change value gets calculated.
  thatphase = 0;                       // Phase change value gets calculated.
  thiscutoff = 192;                    // You can change the cutoff value to display this wave. Lower value = longer wave.
  thatcutoff = 192;                    // You can change the cutoff value to display that wave. Lower value = longer wave.
  thisdelay = 10;                      // You can change the delay. Also you can change the allspeed variable above. 
  thisdir = 0;
  thatdir = 0;
} // resetvar()