/* inoise8_fire
 *
 * By: Andrew Tuline
 *
 * Date: January, 2017
 *
 * This super short sketch (just two lines to the algorithm) displays fire thanks to FastLED's Perlin Noise function and Palettes.
 *
 * It needs some tweaking in order to work across a wide range of NUM_LED values, but looks pretty good at 60.
 * 
 */


#include "FastLED.h"
 
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

struct CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
 
uint32_t xscale = 20;                                          // How far apart they are
uint32_t yscale = 3;                                           // How fast they move
uint8_t index = 0;



void setup() {

  Serial.begin(115200);                                         // Initialize serial port for debugging.
  delay(1000);                                                 // Soft startup to ease the flow of electrons.

  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812

  FastLED.setBrightness(max_bright);

  set_max_power_in_volts_and_milliamps(5, 500);                // FastLED Power management set at 5V, 500mA.
 
  currentPalette = CRGBPalette16(
                                   CRGB::Black, CRGB::Black, CRGB::Black, CHSV(0, 255,4),
                                   CHSV(0, 255, 8), CRGB::Red, CRGB::Red, CRGB::Red,                                   
                                   CRGB::DarkOrange,CRGB::Orange, CRGB::Orange, CRGB::Orange,
                                   CRGB::Yellow, CRGB::Yellow, CRGB::Gray, CRGB::Gray);
} // setup()



void loop() {

  inoise8_fire();                                               // I am the god of hell fire and I bring you . . .
  FastLED.show();

} // loop()



void inoise8_fire() {
  
  for(int i = 0; i < NUM_LEDS; i++) {
    index = inoise8(i*xscale,millis()*yscale*NUM_LEDS/255);                                           // X location is constant, but we move along the Y at the rate of millis()
    leds[i] = ColorFromPalette(currentPalette, min(i*(index)>>6, 255), i*255/NUM_LEDS, LINEARBLEND);  // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }                                                                                                   // The higher the value of i => the higher up the palette index (see palette definition).
                                                                                                      // Also, the higher the value of i => the brighter the LED.
} // inoise8_fire()

