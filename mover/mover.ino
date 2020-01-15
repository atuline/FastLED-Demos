/* aanimations
 *
 * By: Can't recall where I found this.
 * 
 * Modified by: Andrew Tuline
 *
 * Date: January, 2017
 *
 * This sketch demonstrates how to blend between two animations running at the same time.
 * 
 */

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 60                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

// have 3 independent CRGBs - 2 for the sources and one for the output
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];


// Palette definitions
CRGBPalette16 currentPalette = RainbowColors_p;
CRGBPalette16 targetPalette = ForestColors_p;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND


void setup() {


  

  delay(1000);                                                // Power-up safety delay.
  Serial.begin(115200);                                        // Initialize serial port for debugging.

  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812

  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

  currentBlending = LINEARBLEND;  
  
} // setup()



void loop() {

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }



  animationA();                                               // render the first animation into leds2   
  FastLED.show();

} // loop()



void animationA() {                                             // running red stripe.
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t red = (millis() / 5) + (i * 12);                    // speed, length
    if (red > 128) red = 0;
  leds[i] = ColorFromPalette(currentPalette, red, red, currentBlending);
  }
} // animationA()



/* void animationA() {                                             // running red stripe.
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t red = (millis() / 10) + (i * 12);                    // speed, length
    if (red > 128) red = 0;
    leds2[i] = CRGB(red, 0, 0);
  }
} // animationA()
*/

