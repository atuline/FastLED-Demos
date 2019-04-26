/* palettecrossfade
 *
 * By: Mark Kriegsman
 * 
 * Modified by: Andrew Tuline
 *
 * Date: October, 2016
 *
 * This example by Mark Kriegsman (and modified by Andrew) shows how to cross-fade 
 * between different color palettes using the function nblendPaletteTowardPalette.
 *
 * The basic idea is that you always have a "current palette" that you're
 * pulling colors from with ColorFromPalette, and you have a "target palette"
 * which is the 'next' palette that you want to get to.
 *
 * After that, implementation is relatively simple: just periodically call:
 * 
 *   nblendPaletteTowardPalette(currentPalette, targetPalette);
 *   
 * If the current palette is not yet equal to the target palette, this 
 * function will make a few small changes to the current palette to make
 * it slightly more like the target.  Over time, the current palette will
 * come to be equal to the target.
 * There's no need to test the current and target for equality; it's safe
 * to keep calling nblendPaletteTowardPalette even after current reaches target.
 * For faster blending, call nblendPaletteTowardPalette twice per loop.
 * 
 */

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 60                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.
uint8_t thisdelay = 20;

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

CRGBPalette16 currentPalette(CRGB::Black);
CRGBPalette16 targetPalette(PartyColors_p );
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND



void setup() {

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
 
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812

  currentBlending = LINEARBLEND;  
  
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
} // setup()


 
void loop() {
  
  ChangePalettePeriodically();
 
  // nblendPaletteTowardPalette() will crossfade current palette slowly toward the target palette.
  //
  // Each time that nblendPaletteTowardPalette is called, small changes
  // are made to currentPalette to bring it closer to matching targetPalette.
  // You can control how many changes are made in each call:
  //   - the default of 24 is a good balance
  //   - meaningful values are 1-48.  1=veeeeeeeery slow, 48=quickest
  //   - "0" means do not change the currentPalette at all; freeze

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }

 EVERY_N_MILLISECONDS(thisdelay) {
  static uint8_t startIndex = 0;
  startIndex += 1;                                 // motion speed
  FillLEDsFromPaletteColors(startIndex);
 }

  FastLED.show();
  
} // loop()



void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex + sin8(i*16), 255);
    colorIndex += 3;
  }

} // FillLEDsFromPaletteColors()


 
void ChangePalettePeriodically() {
  
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if(lastSecond != secondHand) {
    lastSecond = secondHand;
    CRGB p = CHSV(HUE_PURPLE, 255, 255);
    CRGB g = CHSV(HUE_GREEN, 255, 255);
    CRGB b = CRGB::Black;
    CRGB w = CRGB::White;
    if(secondHand ==  0)  { targetPalette = RainbowColors_p; }
    if(secondHand == 10)  { targetPalette = CRGBPalette16(g,g,b,b, p,p,b,b, g,g,b,b, p,p,b,b); }
    if(secondHand == 20)  { targetPalette = CRGBPalette16(b,b,b,w, b,b,b,w, b,b,b,w, b,b,b,w); }
    if(secondHand == 30)  { targetPalette = LavaColors_p; }
    if(secondHand == 40)  { targetPalette = CloudColors_p; }
    if(secondHand == 50)  { targetPalette = PartyColors_p; }
  }
  
} // ChangePalettePeriodically()
