/* inoise8_pal_demo
 *
 * By: Andrew Tuline
 *
 * Date: August, 2016
 *
 * This short sketch demonstrates some of the functions of FastLED, including:
 *
 * Perlin noise
 * Palettes
 * Palette blending
 * Alternatives to blocking delays
 * Beats (and not the Dr. Dre kind, but rather the sinewave kind)
 *
 * Refer to the FastLED noise.h and lib8tion.h routines for more information on these functions.
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

CRGBPalette16 currentPalette(CRGB::Black);
CRGBPalette16 targetPalette(OceanColors_p);
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND 
 
static int16_t dist;                                          // A random number for our noise generator.
uint16_t xscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint16_t yscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 24;                                      // Value for blending between palettes.



void setup() {

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
 
//  LEDS.addLeds<LED_TYPE,LED_DT,COLOR_ORDER>(leds,NUM_LEDS);
  LEDS.addLeds<LED_TYPE,LED_DT,LED_CK, COLOR_ORDER>(leds,NUM_LEDS);
 
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
  dist = random16(12345);                                     // A semi-random number for our noise generator

} // setup()

 
 
void loop() {
  
  EVERY_N_MILLISECONDS(10) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
    fillnoise8();                                                            // Update the LED array with noise at the new location
  }
 
  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
 
  FastLED.show();
  
} // loop()


 
void fillnoise8() {
  
  for(int i = 0; i < NUM_LEDS; i++) {                                      // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i*xscale, dist+i*yscale) % 255;                // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  
  dist += beatsin8(10,1,4);                                                // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
                                                                           // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
} // fillnoise8()
