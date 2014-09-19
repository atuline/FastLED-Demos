
#include "FastLED.h"
#include "Arduino.h"
#include "fastclass.h"

fastclass::fastclass(CRGB *leds, int pin) { }

	void fastclass::run (CRGB *leds, int numLeds) {

    int thisdelay = 8;                                         // Still learning classes, so this is duplicated.
		int i = random8();															           // A random number. Higher number => fewer twinkles. Use random16() for values >255.
		if (i < numLeds) leds[i] = CHSV(50, 100, 255);						 // Only the lowest probability twinkles will do. You could even randomize the hue/saturation. .
		for (int j = 0; j < numLeds; j++) leds[j].fadeToBlackBy(8); // Use FastLED fadeToBlackBy function
		LEDS.show();

  FastLED.delay(thisdelay*2.5);
//  delay_at_max_brightness_for_power(thisdelay*2.5);
//  delay(thisdelay);

	}
