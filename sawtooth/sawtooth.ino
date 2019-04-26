/* Sawtooth
 *
 * By: Andrew Tuline
 *
 * Date: February, 2018
 *
 * Creating a sawtooth pattern using millis and bpm. It runs at about 440 fps with the configuration below on an Arduino Nano.
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
uint8_t max_bright = 128;                                     // Overall brightness.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Palette definitions
CRGBPalette16 currentPalette = PartyColors_p;
CRGBPalette16 targetPalette = PartyColors_p;
TBlendType    currentBlending = LINEARBLEND;                  // NOBLEND or LINEARBLEND



void setup() {

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Delayed startup in case there's an issue on some platforms.

  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
  //  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

} // setup()




void loop () {
  
  Serial.println(LEDS.getFPS());
  sawtooth();
  FastLED.show();

} // loop()



void sawtooth() {

  int bpm = 60;
  int ms_per_beat = 60000/bpm;                                // 500ms per beat, where 60,000 = 60 seconds * 1000 ms 
  int ms_per_led = 60000/bpm/NUM_LEDS;

  int cur_led = ((millis() % ms_per_beat) / ms_per_led)%(NUM_LEDS);     // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

  if (cur_led == 0)
   fill_solid(leds, NUM_LEDS, CRGB::Black);
  else
    leds[cur_led] = ColorFromPalette(currentPalette, 0, 255, currentBlending);    // I prefer to use palettes instead of CHSV or CRGB assignments.

} // sawtooth()

