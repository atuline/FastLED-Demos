
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

#include <FastLED.h>        // FastLED library

 
#define LED_DT 13
#define NUM_LEDS 24

#define qsub(x, b)	((x>b)?x-b:0)							// Unsigned subtraction macro. if result <0, then => 0
#define _sq(x) ((x)*(x))  
#define pi 3.1415926535897932384626433832795

struct CRGB leds[NUM_LEDS];

uint8_t wave1 = 0;
uint8_t wave2 = 0;
uint8_t wave3 = 0;
int thisdelay = 20;


void setup() {
	Serial.begin(9600);
	LEDS.addLeds<WS2811, LED_DT, GRB>(leds, NUM_LEDS);
}


void loop () {

plas3();

}
 

// leds[k].r = qsub(quadwave8(15*k + wave1), 160);
// leds[i].setRGB( cred, cgrn, cblu);
// leds[i] = CHSV(ahue, thissat, abright);


void plas1() {

	wave1 +=1;
	wave2 +=1;
	wave3 +=10;
	for (int k=0; k<NUM_LEDS; k++) {

		leds[k].r = qsub(sin8(15*k + wave1), 160);				// Another fixed frequency, variable phase sine wave with lowered level
		leds[k].g = qsub(cos8(30*k + wave2), 180);				// A fixed frequency, variable phase sine wave with lowered level
		leds[k].b = 0;				// Another fixed frequency, variable phase sine wave with lowered level



//		leds[k].g = qsub(quadwave8(k*k*2 + wave1), 160)*k/NUM_LEDS;		// A variable frequency, variable phase sine wave with lowered level and decaying amplitude
	}
	LEDS.show();
	delay(thisdelay);
}


void plas2() {

	wave1 +=2;
	wave2 +=2;
	wave3 +=2;
	for (int k=0; k<NUM_LEDS; k++) {
		leds[k].r = 200;				// Another fixed frequency, variable phase sine wave with lowered level
		leds[k].g = qsub(cos8(30*k + wave2), 80);				// A fixed frequency, variable phase sine wave with lowered level
		leds[k].b = qsub(sin8(30*k + wave3), 80);				// A fixed frequency, variable phase sine wave with lowered level
	}
	LEDS.show();
	delay(thisdelay);
}


void plas3() {

	wave1 +=2;
	wave2 +=1;
	wave3 -=3;
	for (int k=0; k<NUM_LEDS; k++) {
		leds[k].r = qsub(sin8(20*k + wave1), 80);				// Another fixed frequency, variable phase sine wave with lowered level
		leds[k].g = qsub(cos8(25*k + wave2), 80);				// A fixed frequency, variable phase sine wave with lowered level
		leds[k].b = qsub(sin8(22*k + wave3), 80);				// A fixed frequency, variable phase sine wave with lowered level
	}
	LEDS.show();
	delay(thisdelay);
}








void splash() {
	wave1 +=1;
	wave2 +=5;
	wave3 +=10;
	for (int k=0; k<NUM_LEDS; k++) {

//		Here's several sine/quadwave examples. Uncomment each set separately.

//		leds[k].g = quadwave8(25*k);									// A fixed frequency, fixed phase sine wave

//		leds[k].g = qsub(quadwave8(25*k), 160);					// A fixed frequency, fixed phase sine wave with lowered level

//		leds[k].g = qsub(quadwave8(30*k + wave1), 160);			// A fixed frequency, variable phase sine wave with lowered level

//		uint8_t tempa = qsub(quadwave8(30*k + wave1), 160);	// A fixed frequency, variable phase sine wave with lowered level
//		uint8_t tempb = quadwave8(30*k + wave1);					// The same fixed frequency, variable phase sine wave without lowered level
//		leds[k] = CHSV(tempa, 255, tempb);							// Let's combine them


		leds[k].r = qsub(quadwave8(15*k + wave1), 160);				// Another fixed frequency, variable phase sine wave with lowered level
		leds[k].g = qsub(quadwave8(30*k + wave2), 180);				// A fixed frequency, variable phase sine wave with lowered level
		leds[k].b = qsub(quadwave8(50*k + wave3), 180);				// Another fixed frequency, variable phase sine wave with lowered level



//		leds[k].g = qsub(quadwave8(k*k*2 + wave1), 160)*k/NUM_LEDS;		// A variable frequency, variable phase sine wave with lowered level and decaying amplitude
	}
	LEDS.show();
	delay(thisdelay);
}
