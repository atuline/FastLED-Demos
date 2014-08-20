
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

struct CRGB leds[NUM_LEDS];
int thisdelay = 20;

fastclass twinkle(leds, NUM_LEDS);


void setup() {
	Serial.begin(9600);
	LEDS.addLeds<WS2811, LED_DT, GRB>(leds, NUM_LEDS);
}

void loop () {

twinkle.run(leds, NUM_LEDS);

}
