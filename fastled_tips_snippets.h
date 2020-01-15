
/* My FastLED Tips, Tricks and Traps

By: Andrew Tuline

Date: July, 2015


Note: This is a TEXT file and not a program. It has a .h extension so that it looks like code in an IDE. It's not.


References:

https://github.com/FastLED/FastLED/wiki/Overview
http://fastled.io/docs/3.1/modules.html


*/



/* NOOB NOTES -----------------------------------------------------------------------------------------------------------

1) If you are trying to control LED's with a button or some other device, you will want the loop to run as fast as possible. Therefore:

- Try (really hard) to avoid using nested for loops.
- Avoid using delay statement within any loops, and especially nested for loops.
- Avoid using the delay statement at all. Learn how to use the EVERY_N_MILLISECONDS() function instead. See below:

 EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    mydemo();
    FastLED.show();
  }

2) If you want to ensure you don't overload your battery, you might want to use power managed display. So, instead of just:

    FastLED.show();

  Put the following in setup():

    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // This is used by the power management functionality and is currently set at 5V, 500mA.

 
 

3) You can even use functions like beatsin8() mapped to NUM_LEDS to avoid using delays entirely.

4) Palettes. Learn to use them. Learn how to smoothly transition between them. You will almost never go back to CRGB or CHSV.

5) Use 8 and 16 bit math, and the FastLED math functions where possible. Floating point is s-l-o-w.


Here's an excellent article:

http://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#bouncing_balls_color



6) These folks have some excellent displays to study:

Marc Miller
	https://github.com/marmilicious
	https://gist.github.com/marmilicious


Stefan Petrick
	https://github.com/StefanPetrick


Jason Coon
	https://gist.github.com/jasoncoon
	http://pastebin.com/u/infinitron
	https://www.youtube.com/user/pup05
	https://evilgeniuslabs.org/

Mark Kriegsman
	https://gist.github.com/kriegsman

Andrew Tuline
	https://github.com/atuline/FastLED-Demos
	http://pastebin.com/u/atuline
	https://www.youtube.com/user/atuline/videos


Robert Atkins
	Look around for some demos

Jon Burroughs
	Look around for some demos



*/



/*

Here's my FastLED Snippets, which are just a bunch of notes I've made over time.

*/


// Basic Definition -----------------------------------------------------------------------------------------------------

#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

#if FASTLED_VERSION < 3001000                                 // This guarantees the person will have to use FastLED 3.1
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                   // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                          // Analog Unsigned subtraction macro. if result <0, then => 0

   
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER BGR                                       // Use BGR for APA102 and GRB for WS2812
#define LED_TYPE APA102                                       // Or WS2812. Don't forget to change the FastLED.addLeds line as well.
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly, i.e. with a potentiometer.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

// VARIABLES - In general, use the smallest and most accurate variable definition you can, i.e. Localize them to your routine if you can.
uint8_t myVar = 0;                                            // for a variable that ranges from 0 to 255.






// SETUP -------------------------------------------------------------------------------------------------------


void setup () {

	delay(1000);                                                // If things go bad, you can shutdown before the LED's start drawing power.
	Serial.begin(57600);                                        // Get the serial port running for debugging

	FastLED.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);        // Use this for WS2812B
	FastLED.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
	FastLED.addLeds<LED_TYPE,LED_DT,LED_CK,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip).setDither(max_bright < 255);
	FastLED.addLeds<LED_TYPE,LED_DT,LED_CK,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(0xffb0e0);
	FastLED.setBrightness(max_bright);                          // You can change the overall brightness on the fly, i.e. with a potentiometer.
	FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // This is used by the power management functionality and is currently set at 5V, 500mA.


	// Optional randomization
	random16_set_seed(4832);                                    // Awesome randomizer
	random16_add_entropy(analogRead(2));
	int ranstart = random16();


} // setup()



// LOOP -------------------------------------------------------------------------------------------------------

void loop() {
	static int myvar = 5;                                       // The value is only initialized once. Very cool.
	int yourvar = 6;                                            // The value is initialied every time you call this routine.
	ChangeMe();                                                 // Used to change parameters of your program.

	EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
	  twinkle();
	}

	FastLED.show();                         					// Run the FastLED.show() at full loop speed.
	Serial.println(LEDS.getFPS());                              // Display frames per second on the serial monitor.  
} // loop()





// CHANGEME -----------------------------------------------------------------------------------------------------

void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
	uint8_t secondHand = (millis() / 1000) % 15;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
	static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
	if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
	  lastSecond = secondHand;
	  switch(secondHand) {
	    case 0: thisdelay = 10; break;
	    case 5: thisdelay = 20; break;
	    case 10: thisdelay = 30; break;
	    case 15: break;
	  }
	}
} // ChangeMe()





// FastLED Delay -------------------------------

FastLED.delay(thisdelay);									// I don't use this either. I use EVERY_N_MILLISECONDS(x) instead.


// Power managed display -----------------------------------

FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // This is defined in setup and used by the power management functionality and is currently set at 5V, 500mA.
FastLED.show();                         					// This is used in loop for power managed display of LED's.
  

// Randomizer

	random16_set_seed(4832);                                    // Awesome randomizer
	random16_add_entropy(analogRead(2));
	int ranstart = random16();


// Palettes -----------------------------------------------------------------------------------------


Instructions for using Paletteknife (you need to use Chrome browser):


http://fastled.io/tools/paletteknife/

Put in your toolbar:
https://plus.googleapis.com/112916219338292742137/posts/FvLgYPF52Ma

Get Palettes here:
http://soliton.vm.bytemark.co.uk/pub/cpt-city/index.html


Tutorial

https://learn.adafruit.com/simple-beautiful-color-changing-light-strand/creating-color-palettes

https://www.youtube.com/watch?v=7CDgxgyALWQ\


Find palette and click on above toolbar link.

Copy/paste the link to the top of your routine.



CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;        // NOBLEND or LINEARBLEND


// In setup
// RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p

currentPalette  = CRGBPalette16(CRGB::Black);
targetPalette   = RainbowColors_p;                            // Used for smooth transitioning.
currentBlending = LINEARBLEND;



// Assigning color
leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);


// In loop

EVERY_N_MILLISECONDS(100) {
	uint8_t maxChanges = 24; 
	nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
}

// In loop for testing
EVERY_N_MILLISECONDS(5000) {
  targetPalette = CRGBPalette16(CHSV(random8(), 255, 32), CHSV(random8(), random8(64)+192, 255), CHSV(random8(), 255, 32), CHSV(random8(), 255, 255)); 
}

// Variable EVERY_N_MILLISECONDS_I

EVERY_N_MILLIS_I(thistimer, thisdelay) {								// Sets initial timing only. Changes here don't do anything
strobe_lights(current_mode);
}

thistimer.setPeriod(thisdelay);											// Here is where you change the timing.




// Creating a palette
void SetupRandomPalette() {
  int myHue = random8();
  targetPalette = CRGBPalette16(CHSV(myHue, 255, 32), CHSV(myHue, random8(64)+192, 255), CHSV(myHue, 255, 32), CHSV(myHue, random8(64)+192, 255)); 
}

void SetupRandomPalette() {
  for (int i = 0; i < 16; i++) {
    targetPalette[i] = CHSV(random8(), 255, 255);
  }
}

void SetupRandomPalette() {
  int myHue = random8();
  targetPalette = CRGBPalette16(CRGB::Green); 
}


// Assigning a palette

leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);


// CHSV Palettes

Definitions:

CHSVPalette16 RainbowColorsHSV_p;
CHSVPalette16 currentPalette = RainbowColorsHSV_p; 


Setup:

  for( uint8_t j = 0; j < 16; j++) {
    uint8_t hue = j * 16;
    currentPalette[j] = CHSV( hue, 255, 255);
  }

Loop:

  CHSVcolor = ColorFromPalette(currentPalette, paletteIndex++);
  fill_solid(leds, NumLeds, color);

// Not sure if this works.

  You could, however:

  CRGBcolor = ColorFromPalette(currentPalette, paletteIndex++);
  fill_solid(leds, NumLeds, color);



// Timing ----------------------------------------------------------------------------------------------------------


EVERY_N_MILLISECONDS(10) {run_function();}        // I use this instead of delay statements.

EVERY_N_SECONDS(5) {fun_function();}				// I use this one for major changes.


// Assigning Colors -------------------------------------------------------------------------------------------------

leds[k] += CHSV(thathue, allsat, thatbright);
leds[k] = CRGB::Red;


leds[k].g++;												// Or .r or .b



// colorutils.h is at http://fastled.io/docs/3.1/group___colorutils.html

fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.

fill_solid(leds,NUM_LEDS, 0xff00ff);                        //A long RGB value
fill_solid(leds, NUM_LEDS, CRGB(50,0,200));                 // 8 bit values
fill_solid(leds, NUM_LEDS, CHSV(150,255,200));              // 8 bit values

fill_solid(leds,3,0x00ff00);							// 3 green
fill_solid(leds+3,3,0xff0000);                        // then 3 red


fill_gradient_RGB(leds, startpos, 0x000011, endpos, 0x110000);
fill_gradient_RGB(leds, NUM_LEDS, CRGB(50,0,200), CRGB(80,200,240));    // up to 4 of these

//FORWARD_HUES, BACKWARD_HUES, SHORTEST_HUES, LONGEST_HUES
fill_gradient(leds, startpos, CHSV(50, 255,255) , endpos, CHSV(150,255,255), SHORTEST_HUES);
fill_gradient(leds, NUM_LEDS, CHSV(50, 255,255), CHSV(100,255,255), LONGEST_HUES);    // up to 4 of these



// Fade, Scale
fadeToBlackBy(leds, NUM_LEDS, fadeval);                     // 8 bit, 1 = slow, 255 = fast
nscale8(leds,NUM_LEDS,fadeval);                             // 8 bit, 1 = fast, 255 = slow
leds[i].fadeToBlackBy(fadeval);

fadeToBlackBy(leds+4*3, 3, fadeval);      // 8 bit, 1 = slow, 255 = fast
nscale8(leds+4*3, 3,fadeval);                   // 8 bit, 1 = fast, 255 = slow



// Blend
leds[i] = blend(CRGB::Red, CRGB::Blue, sin8(mysine));


nblend(leds[0], newcolour, 128);				// newcolor is a Colour, either RGB or HSV
nblend(leds[0], CHSV(color, 255,255), 128);				// newcolor is a Colour, either RGB or HSV
nblend(led[0], CRGB(0,50,0),255,255);


// Using random numbers
uint8_t myval = random8(0, 255);
int myval = random16(NUM_LEDS);



// Beats

int beat = beatsin16(BeatsPerMinute,0,NUM_LEDS);
uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);




// Noise http://fastled.io/docs/3.1/group___noise.html
fill_noise16(leds, NUM_LEDS, octaves, x, xscale, hue_octaves, hxy, hue_scale, hue_time);



// Math http://fastled.io/docs/3.1/lib8tion_8h_source.html

// Limiting math
#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.
uint8_t sum = qadd8( 200, 200);                               // --> saturated at 255
sum = qsub8(i, j);                                            // Does not go below 0


// Memory management

memset(leds, 0, NUM_LEDS * 3);                                // Quick clearing of the LED's.
memcpy8(temp, leds, sizeof(leds));                            // Copy values from temp to leds



  if (thisdir == 0) {                                                                 // Now we're copying LED's down the array, again depending on the direction.
//    for (int i = NUM_LEDS-1; i >0 ; i-- ) leds[i] = leds[i-1];                      // Be VERY careful not to go over your define array length, or crashy crashy.
    memmove(leds+1, leds, (NUM_LEDS-1)*3);                                            // Oh look, the FastLED method.
  } else {
//    for (int i = 0; i < NUM_LEDS-1 ; i++ ) leds[i] = leds[i+1];                     // We can go the other way as well.
    memmove(leds, leds+1, (NUM_LEDS-1)*3);
  }




// PRINTF capability for debugging
// If you want to use printf capability, put this in your code:
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

int serial_putchar(char c, FILE* f) {
  if (c == '\n') serial_putchar('\r', f);
  return !Serial.write(c);
} //serial_putchar()
FILE serial_stdout;

void setup() {
Serial.begin(57600);
fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE); // Set up stdout
stdout = &serial_stdout;
} // setup()

void loop() {
printf("My favorite number is %6d!\n", 12);                 // This is just an example
} // loop()


// Other ---------------------------------------------------------------------------------------------------------------------------------

// An add glitter function.
void addGlitter( uint8_t chanceOfGlitter) {
if(random8() < chanceOfGlitter) {
  leds[ random16(NUM_LEDS) ] += CRGB::White;
}
}


// Beats Information ---------------------------------------------------------------------------------------------------------------------


uint8_t wave = beatsin8(
  accum88 beats_per_minute,                                   // I'd use an int or uint8_t
  uint8_t lowest=0,
  uint8_t highest=255,
  uint32_t timebase=0,                                        // Set to millis() to zero out the beat
  uint8_t phase_offset=0)                                     // This is kind of cool

When you want to 'zero' out the beat:

uint8_t wave = beatsin8(60, 0, 255, millis());


// Easing & lerping -------------------------------------------------------------------------------------------------------

easeOutVal = ease8InOutQuad(easeInVal);                       // Start with easeInVal at 0 and then go to 255 for the full easing.
ledNum = lerp8by8(0, NUM_LEDS, easeOutVal);                   // Map it to the number of LED's you have.




// CHSV to CRGB scaling -----------------------------------------------------------

If you want any level of hue rotation, then keep CHSV brightness at 32

Table of CHSV brightness to CRGB value
0 == (0,0,0)
1 == (1,0,0)
16 == (2,0,0)
23 == (3,0,0)
28 == (4,0,0)
32 == (5,0,0)



// Working with CHSV and then displaying CRGB -------------------------------------

CHSV hsvs[NUM_LEDS];
CRGB leds[NUM_LEDS]; 

void setup() {
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  // convert all hsvs into rgbs:
  hsv2rgb_rainbow( hsvs, leds, NUM_LEDS);
  // send the actual rgbs to the physical led strip:
  FastLED.show();
}