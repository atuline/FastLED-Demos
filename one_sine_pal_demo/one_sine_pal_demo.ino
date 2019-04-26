/* one_sine_pal_demo
 *
 * By: Andrew Tuline
 *
 * Date: January, 2017
 * 
 * This takes my one_sine_pal program and demonstrates some of the different variables in a demo loop.
 * 
 * Forget using delay() statements or counting pixels up and down the strand of LED's. This example demonstrates just a bit of what can be done using basic high school math
 * by using a simple sinewave.
 * 
 * You can use a sine wave to go back and forth up a strand of LED's. You can shift the phase of a sine wave to move along a strand. You can clip the top of that sine wave
 * to just display a certain value (or greater). You can add palettes to that sine wave so that you aren't stuck with CHSV values and so on.
 * 
 * It shows that you don't neeed a pile of for loops, delays or counting pixels in order to come up with decent animations.
 * 
 */

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for WS2812 or WS2801.
#define LED_CK 11                                             // Serial clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // Are they GRB for WS2812 and BGR for APA102
#define LED_TYPE APA102                                       // What kind of strip are you using? WS2812, APA102. . .
#define NUM_LEDS 60                                           // Number of LED's.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

uint8_t max_bright = 192;                                     // Overall brightness definition. It can be changed on the fly.


// Initialize changeable global variables. Play around with these!!!
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave.
int8_t thisspeed = 8;                                         // You can change the speed of the wave, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus distance between bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 128;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
uint8_t thisdelay = 30;                                           // You can change the delay. Also you can change the allspeed variable above. 
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbright = 0;                                        // Brightness of background colour
uint8_t bgclrinc = 0;
int startIndex = 0;

// Palette definitions
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;



void setup() {

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.

  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   //WS2801 and APA102
//  LEDS.addLeds<LED_TYPE, LED_DT,COLOR_ORDER>(leds, NUM_LEDS);   // WS2812

  FastLED.setBrightness(max_bright);

  set_max_power_in_volts_and_milliamps(5, 1000);               // FastLED Power management set at 5V, 500mA. This is OPTIONAL!!!

} // setup()



void loop () {

  ChangeMe();

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    startIndex += thisrot;                                    // Motion speed
    one_sine_pal(startIndex);
  }

  FastLED.show();

} // loop()



void one_sine_pal(uint8_t colorIndex) {                                                         // This is the heart of this program. Sure is short.
  
  thisphase += thisspeed;                                                     // You can change direction and speed individually.
  
   for (int k=0; k<NUM_LEDS; k++) {                                          // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int thisbright = qsuba(cubicwave8((k*allfreq)+thisphase), thiscutoff);    // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbright);                                     // First set a background colour, but fully saturated.
    leds[k] += ColorFromPalette(currentPalette, colorIndex + k, thisbright, currentBlending);
    colorIndex += thisrot;
  }

  bgclr += bgclrinc;                                                                    // You can change the background colour or remove this and leave it fixed.

} // one_sine_pal()



void ChangeMe() {
  
  uint8_t secondHand = (millis() / 1000) % 60;                // Increase this if you want a longer demo.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    switch (secondHand) {
      case  0: break;
      case  5: targetPalette=RainbowColors_p; bgclr=0; bgbright=32; bgclrinc=1; thisrot=1; thisdelay=10; thisspeed=-4; allfreq=16; thiscutoff=128; currentBlending=NOBLEND; break;
      case 10: targetPalette=ForestColors_p; thisspeed = 12; thisrot = 0; thisspeed = 12; thisdelay = 10; thiscutoff=128; allfreq=64; bgclr = 50; bgbright=15; currentBlending=LINEARBLEND; break;
      case 15: SetupRandomColorPalette(); thisrot = 2; thisspeed = 16; break;
      case 20: targetPalette=LavaColors_p; thisspeed = 4; break; allfreq = 16; bgclr=50; break;
      case 25: thiscutoff = 96; thisspeed = -4; thiscutoff = 224; bgclr=20; bgbright=8; break;
      case 30: SetupRandomColorPalette(); thiscutoff = 96; thisrot = 1; break;
      case 35: targetPalette = OceanColors_p; break;
      case 40: SetupSimilarColorPalette(); thiscutoff = 128; break;
      case 45: SetupSimilarColorPalette(); thisspeed = 3; break;
      case 50: targetPalette = PartyColors_p; break;
      case 55: break;
      case 60: break;
    }
  }

} // ChangeMe()



void SetupRandomColorPalette() {
  
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));

} // SetupRandomColorPalette()



void SetupSimilarColorPalette() {
  
    static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line, i.e. baseC+random8(0,5) for the hue.
    targetPalette = CRGBPalette16(CHSV(baseC+random8(0,10), 255, random8(128,255)), CHSV(baseC+random8(0,10), 255, random8(128,255)), CHSV(baseC+random8(0,10), 192, random8(128,255)), CHSV(baseC+random8(0,10), 255, random8(128,255)));

} // SetupSimilarColorPalette()
