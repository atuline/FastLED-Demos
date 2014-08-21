/*
 # This shows the differences in the 'Delay' commands. The FastLED version doesn't always work.
 #
*/ 

#include "FastLED.h"


#define NUM_LEDS 24
#define DATA_PIN 13
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

void setup() { 
//      FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  LEDS.addLeds<WS2811, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
	LEDS.setBrightness(128);
  set_max_power_in_volts_and_milliamps( 5, 1000);
}

void loop() { 
	for(int i = 0; i < NUM_LEDS; i++) {
		leds[i] = CRGB::Red;
		show_at_max_brightness_for_power();
		leds[i] = CRGB::Black;
//		delay_at_max_brightness_for_power(20);
		delay(20);
	}
	for(int i = NUM_LEDS-1; i >= 0; i--) {
		leds[i] = CRGB::Red;
		show_at_max_brightness_for_power();
		leds[i] = CRGB::Black;
//		delay_at_max_brightness_for_power(20);
//	  LEDS.delay(20);
		delay(20);
	}
}