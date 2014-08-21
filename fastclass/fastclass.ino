
// Fastclass is a modification of fasttwinkle
//
//
//  By Andrew Tuline
//


/*
 # Clean up your code Andrew with this example:
 #
 # https://bitbucket.org/ratkins/bikeblinder
 # http://arduino.cc/en/Hacking/LibraryTutorial
 # http://www.cplusplus.com/doc/tutorial/classes/
*/

#include <FastLED.h>        // FastLED library
#include "fastclass.h"
 
#define LED_DT 13
#define NUM_LEDS 24
#define COLOR_ORDER GRB
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  196                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];

// Initialize global variables for sequences
int thisdelay;                                                 // A delay value for the sequence(s)

fastclass twinkle(leds, NUM_LEDS);


void setup() {
	Serial.begin(9600);
	LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

// Initializing code/variables for the sequence
  thisdelay = 20;

}

void loop () {

twinkle.run(leds, NUM_LEDS);

}
