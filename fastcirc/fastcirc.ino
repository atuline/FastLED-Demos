
// Fastcirc
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// Single LED going increasingly faster up the strand. I use this as a starting sequence for a series of demos.
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
int thisdelay = 40;                                                 // A delay value for the sequence(s)
int thisstep = 1;
int thissat = 255;
int thishue = 0;
int thisbright = 255;



void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA
} // setup()


void loop () {
  fast_circ();
} // loop()



void fast_circ() {                                    // FAST CIRCLE
  if (thisdelay > 25) {thisstep = -1;}
  if (thisdelay == 0) {thisstep = 1;}
  thisdelay += thisstep;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(thishue, thissat, thisbright);
    // thishue++;
    show_at_max_brightness_for_power();
    leds[i] = CRGB::Black;
    delay(thisdelay);
  }
} // fast_circ()


