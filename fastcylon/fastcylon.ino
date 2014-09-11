
// TITLE
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// Bouncing single LED, from the Cylons in the original Battlestar Galactica.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//

#include "FastLED.h"

#define NUM_LEDS 24
#define DATA_PIN 13
#define COLOR_ORDER GRB
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  196                                        // How bright do we want to go

CRGB leds[NUM_LEDS];

int thisdelay = 20;


void setup() { 
//      FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
	LEDS.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps( 5, 1000);
} // setup()


void loop() { 
	cylon();
}


void cylon() {
	for(int i = 0; i < NUM_LEDS; i++) {
		leds[i] = CRGB::Red;
		show_at_max_brightness_for_power();
    leds[i] = CRGB::Black;
//		delay_at_max_brightness_for_power(20);
		delay(thisdelay);
	}

	for(int i = NUM_LEDS-1; i >= 0; i--) {
    leds[i] = CRGB::Red;
		show_at_max_brightness_for_power();
  	leds[i] = CRGB::Black;
//		delay_at_max_brightness_for_power(20);
		delay(thisdelay);
	}
} // cylon()