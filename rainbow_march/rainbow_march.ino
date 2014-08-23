
// Rainbow March
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// This is a simple FastLED (2.1 and greater) display sequence.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//


#include <FastLED.h>                                           // FastLED library
 
#define LED_DT 13                                              // Data pin
#define NUM_LEDS 24                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  196                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array


// Initialize global variables for sequences
int thisdelay = 8;                                             // A delay value for the sequence(s)
int thishue = 0;


void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);                // FastLED 2.1 Power management set at 5V, 500mA
} // setup()


void loop () {
  rainbow_march();
} // loop()


void rainbow_march() {                                         // RAINBOW MARCH, DOESN'T SUPPORT FADE
  thishue -= 1;
  fill_rainbow( leds, NUM_LEDS, thishue );
 show_at_max_brightness_for_power();
 delay(thisdelay);
}

