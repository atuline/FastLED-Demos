/* ColorPal_Beat

By: Andrew Tuline

Date: July, 2015

This routine uses FastLED beats and colour palettes with gradual palette cross fading. It doesn't require timers/delays except to change/fade between palettes on occasion.

*/


#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif


#define LED_DT     12
#define LED_CK     11
#define NUM_LEDS   64
#define LED_TYPE   APA102
#define COLOR_ORDER BGR

CRGB leds[NUM_LEDS];

uint8_t brightness = 128;

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
uint8_t maxChanges = 24; 
TBlendType    currentBlending;


void setup() {
  delay(3000);                                                // power-up safety delay

//  FastLED.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);             // WS2812B
  FastLED.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // WS2801 or APA102

  FastLED.setBrightness(brightness);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

  currentPalette = RainbowColors_p;                           // RainbowColors_p; CloudColors_p; PartyColors_p; LavaColors_p; HeatColors_p;
  targetPalette = RainbowColors_p;                           // RainbowColors_p; CloudColors_p; PartyColors_p; LavaColors_p; HeatColors_p;
  currentBlending = LINEARBLEND;   
} //setup()


void loop() {
  uint8_t beatA = beatsin8(5, 0, 255);                        // Starting hue
  FillLEDsFromPaletteColors(beatA);
  show_at_max_brightness_for_power();                         // Power managed display.

  EVERY_N_MILLISECONDS(100) {                                // FastLED based timer to update/display the sequence every 5 seconds.
    nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);
  }

  EVERY_N_MILLISECONDS(5000) {                                // FastLED based timer to update/display the sequence every 5 seconds.
    SetupRandomPalette();
  }
} //loop()


void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  uint8_t beatB = beatsin8(15, 10, 20);                       // Delta hue between LED's
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, 255, currentBlending);
    colorIndex += beatB;
  }
} //FillLEDsFromPaletteColors()


void SetupRandomPalette() {
  targetPalette = CRGBPalette16(CHSV(random8(), 255, 32), CHSV(random8(), random8(64)+192, 255), CHSV(random8(), 255, 32), CHSV(random8(), 255, 255)); 
} // SetupRandomPalette()