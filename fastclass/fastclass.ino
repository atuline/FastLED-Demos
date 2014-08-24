// Fastclass is a modification of fasttwinkle in my first attempt to create classes
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// This is my first attempt to create a class using the twinkle routine.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//
//
// Here's some other examples:
//
// https://bitbucket.org/ratkins/bikeblinder
// http://arduino.cc/en/Hacking/LibraryTutorial
// http://www.cplusplus.com/doc/tutorial/classes/
//

#include <FastLED.h>        // FastLED library
#include "fastclass.h"
 
#define LED_DT 13
#define NUM_LEDS 24
#define COLOR_ORDER GRB
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  196                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];

// Initialize global variables for sequences
int thisdelay = 20;                                                 // A delay value for the sequence(s)

fastclass twinkle(leds, NUM_LEDS);


void setup() {
	Serial.begin(9600);
	LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
} // setup()

void loop () {
twinkle.run(leds, NUM_LEDS);
} // loop()
