
// Random Color Pop
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// A simple twinkling colour LED sequence with standard delay.
//
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//
// Note: If you receive compile errors (as I have in the Stino add-on for Sublime Text), set the compiler to 'Full Compilation'.
//

#include <FastLED.h>                                           // FastLED library
 
#define LED_DT 13                                              // Data pin
#define NUM_LEDS 8                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  128                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array


// Initialize global variables for sequences
uint8_t thisdelay = 20;                                             // A delay value for the sequence(s)
int     thishue = 0;
int     idex = 0;

// Non-blocking delay timers
volatile long previousMillis = 0;


void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);                // FastLED 2.1 Power management set at 5V, 500mA
} // setup()


void loop () {
 random_color_pop();                                                   // Improved method of using non-blocking delay
} // loop()



void random_color_pop() {                                     // RANDOM COLOR POP FADE
  idex = random8();
  thishue = random16(0,255);                                  // For some reason, the second random8() wasn't that random
  if (idex < NUM_LEDS) leds[idex] = CHSV(thishue, 255, 255);  // Only the lowest probability twinkles will do. You could even randomize the hue/saturation. .
  for (int i = 0; i <NUM_LEDS; i++) leds[i].fadeToBlackBy(8); // Go through the array and reduce each RGB value by a percentage.
  show_at_max_brightness_for_power();
  delay(thisdelay);
}


