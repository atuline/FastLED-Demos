
// Splash
//
// Using FastLED's sin8() and quadwave8() math
//
//  By Andrew Tuline
//
// This demonstrates the cool capabilities of using basic trig functions. They're fast AND and easy.
// 
// For instance, you can create fully anti-aliased moving bars using this function with just a few simple
// lines of code.
//
// Here's some plasma/demo stuff
//
// - http://lodev.org/cgtutor/plasma.html
// - http://www.flipcode.com/archives/The_Art_of_Demomaking-Issue_04_Per_Pixel_Control.shtml
// - http://www.pouet.net/topic.php?which=4650
// - http://www.mennovanslooten.nl/blog/post/72
// - http://www.bidouille.org/prog/plasma  (see fastplasma)
// - https://github.com/johncarl81/neopixelplasma
// - http://codegolf.stackexchange.com/questions/35569/tweetable-mathematical-art (see fasttweetable)

#ifndef twinkle_h
#define twinkle_h

#include <FastLED.h>        // FastLED library
 
#define LED_DT 13
#define NUM_LEDS 24

struct CRGB leds[NUM_LEDS];

void setup() {
	Serial.begin(9600);
	LEDS.addLeds<WS2811, LED_DT, GRB>(leds, NUM_LEDS);
}

void loop () {
	twinkle();
}

void twinkle() {
	int i = random8();													// A random number. Higher number => fewer twinkles. Use random16() for values >255.
	if (i < NUM_LEDS) leds[i] = CHSV(50, 100, 255);				// Only the lowest probability twinkles will do. You could even randomize the hue/saturation. .
	for (int j = 0; j < NUM_LEDS; j++) leds[j].fadeToBlackBy(8);
	LEDS.show();
	LEDS.delay(8);																// Higher number => slower twinkles.
}

#endif