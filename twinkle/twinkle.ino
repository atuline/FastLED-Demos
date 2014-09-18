
// Twinkle
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// A simple twinkling LED sequence with non-blocking delay.
//
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//
// Note: If you receive compile errors (as I have in the Stino add-on for Sublime Text), set the compiler to 'Full Compilation'.
//

#include <FastLED.h>                                           // FastLED library
 
#define LED_DT 13                                              // Data pin
#define NUM_LEDS 24                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  128                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array


// Initialize global variables for sequences
uint8_t thisdelay = 8;                                             // A delay value for the sequence(s)

// Non-blocking delay timers
volatile long previousMillis = 0;


void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);                // FastLED 2.1 Power management set at 5V, 500mA
} // setup()


void loop () {
 twinkle();                                                   // Improved method of using non-blocking delay
} // loop()


void twinkle() {
    int i = random8();                                         // A random number. Higher number => fewer twinkles. Use random16() for values >255.
    if (i < NUM_LEDS) leds[i] = CHSV(50, 100, 255);            // Only the lowest probability twinkles will do. You could even randomize the hue/saturation. .
    for (int j = 0; j < NUM_LEDS; j++) leds[j].fadeToBlackBy(8);
    show_at_max_brightness_for_power();                        // Power managed display of LED's

//  FastLED.delay(thisdelay*2.5);
  delay_at_max_brightness_for_power(thisdelay*2.5);
//  delay(thisdelay);

} // twinkle()
