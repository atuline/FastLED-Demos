/* soundmems_demo

By: Andrew Tuline

Date: December 2017

Updated: September 2018


Now with FIFTEEN, YES, FIFTEEN 1D sound reactive routines. Some look similar to the others, so I've spread them out.

These sound reactive program does not use FFT or an MSGEQ7 chip, but rather just uses the current sample, the average (of the last 64 samples) and peak detection.


IMPORTANT NOTE:

The Sparkfun MEMS microphone I use SHOULD be powered with 3.3V. If you do so, you need to connect the 3.3V pin on the Arduino to the AREF pin on your Arduino.

Furthermore, you will need to uncommment the following line in setup:

//  analogReference(EXTERNAL);                                                    // Comment out this line for 3.3V Arduino's, ie. Flora, etc.

If you power your microphone from 5V or are using a 3.3V Arduino, such as a Flora, etc, then leave the line commented out.



Features:

- 10^h^h15, count 'em 10^h^h15 different awesome sound reactive display routines.
- Broken out functionality into separate .h files for readability.
- Only ONE delay statement in the setup. That's it. Delay statements have no place in the loop.
- Uses a variable rate EVERY_N_MILLISI() function instead.
- Trying to localize and tighten up the variable definitions as much as possible.
- Uses the 3.3V Sparkfun MEMS microphone with a whopping 67 dba gain.
- Uses a potentiometer to adjust sensitivity.
- Uses FastLED palettes, palette transitioning, beats, fading, fast math as well as perlin noise functionality.
- No floating point math. I use 8 or 16 bit for better performance.
- Shows that you don't need gobs of code to come up with a cool display sequence.


Not features:

- Not object oriented.
- Not passing variables or structs to/from functions.
- Sorry, but I prefer Java style curly brackets and not C style. Maybe that's because I don't use a real IDE and I'm not a real programmer.
- This probably won't scale beyond 255 LED's. I'll leave that job up to you.


Notes:

- This comes preconfigured for 60 APA102 LED's and a 3.3V Sparkfun MEMS microphone. Reconfigure and compile this for your own environment.
- Comment the analogreference(EXTERNAL) line if using a 3.3V Arduino OR a 5V microphone.
- This requires a potentiometer to adjust sensitivity. I use a linear 10K.
- This runs at about 430 loops per second on an Arduino Nano with 60 APA102's. If you add routines, do NOT add delay statements or I will find you, and . . .


*/


// Use qsuba for smooth pixel filtering and qsubd for digital pixel filtering.
#define qsubd(x, b)  ((x>b)?b:0)                                                // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                                              // Analog Unsigned subtraction macro. if result <0, then => 0. By Andrew Tuline.

#include "FastLED.h"                                                            // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed hardware and FastLED definitions cannot change on the fly.
#define LED_DT 12                                                               // Data pin to connect to the strip.
#define LED_CK 11                                                               // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                                         // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                                         // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 60                                                             // Number of LED's.

// Fixed sound hardware definitions cannot change on the fly.
#define MIC_PIN    5                                                            // Microphone on A5.
#define POT_PIN    4                                                            // Potentiometer on A4.

// Global FastLED variables
uint8_t max_bright = 255;                                                       // Maximum brightness, does change in other programs.
struct  CRGB leds[NUM_LEDS];                                                    // Initialize our LED array.


CRGBPalette16 currentPalette(PartyColors_p);
CRGBPalette16 targetPalette(PartyColors_p);
TBlendType    currentBlending = LINEARBLEND;                                    // NOBLEND or LINEARBLEND 


// Global timer value
uint8_t timeval = 20;                                                           // Currently 'delay' value. No, I don't use delays, I use EVERY_N_MILLIS_I instead.
uint16_t loops = 0;                                                             // Our loops per second counter.

// Global sound variables used in other routines.
uint16_t oldsample = 0;                                                         // Previous sample is used for peak detection and for 'on the fly' values.
bool     samplepeak = 0;                                                        // The oldsample is well above the average, and is a 'peak'.
uint16_t sampleavg = 0;                                                         // Average of the last 64 samples.


// Global visual variables used in display and other routines.
bool thisdir = 0;                                                               // Used in a display routine as well as a support routine.



void setup() {

  delay(3000);                                                                  // A delay in case things don't work correctly. I use this for Pro Micro.
  
  analogReference(EXTERNAL);                                                    // Comment out this line for 3.3V Arduino's, ie. Flora, etc.
  
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);  // Use this for WS2801 or APA102
// LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);          // Use this for WS2812

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);                                 // FastLED Power management set at 5V, 500mA.
  
  Serial.begin(115200);                                                          // Serial port speed for debugging.
   
} // setup()



// Include various routines. Makes for an easier to read main program.

#include "soundmems.h"                                                          // Sample the sounds and provide a current sample, average of last 64 samples and boolean peak (for twinkles).

#include "support.h"                                                            // A few extra routines for good luck.

// Main sound reactive routines
#include "pixels.h"         // Long line of colours
#include "fillnoise.h"      // Center to edges with base color and twinkle
#include "jugglep.h"        // Long line of sinewaves
#include "ripple.h"         // Juggle with twinkles
#include "pixel.h"          // Long line of colours
#include "matrix.h"         // Start to end with twinkles
#include "onesine.h"        // Long line of shortlines
#include "noisefire.h"      // Start to end
#include "rainbowbit.h"     // Long line of short lines with twinkles
#include "noisefiretest.h"  // Center to edges
#include "rainbowg.h"       // Long line with flashes
#include "noisewide.h"      // Center to edges
#include "plasma.h"         // Long line of short lines
#include "besin.h"          // center to edges with black
#include "noisepal.h"       // Long line

// Test sound reactive routines



typedef void (*SimplePatternList[])();                                          // List of patterns to cycle through.  Each is defined as a separate function below.

SimplePatternList gPatterns = {pixels, fillnoise, jugglep, ripple, pixel, matrix, onesine, noisefire, rainbowbit, noisefiretest, rainbowg, noisewide, plasma, besin, noisepal};  // HERE IS WHERE YOU ADD YOUR ROUTINE TO THE LIST!!!!

uint8_t gCurrentPatternNumber = 0;                                              // Index number of which pattern is current.



void loop() {                                                                   // The >>>>>>>>>> L-O-O-P <<<<<<<<<<<<<<<<<<<<<<<<<<<<  is buried way down here!!!11!1!

  soundmems();
  
//  showfps();                                                                  // Show the frames per second. It had better not dip too far.

  EVERY_N_MILLISECONDS(20) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);      // Awesome palette blending capability.
  }

  EVERY_N_MILLIS_I(thisTimer,timeval) {
    thisTimer.setPeriod(timeval);                                               // We can change the timeval on the fly with this line, which is pretty sweet.
    gPatterns[gCurrentPatternNumber]();                                         // Call the current pattern function.
  }
  
  EVERY_N_SECONDS(5) {                                                          // Change the target palette to a 'related colours' palette every 5 seconds.
    uint8_t baseclr = random8();                                                // This is the base colour. Other colours are within 16 hues of this. One color is 128 + baseclr.

    targetPalette = CRGBPalette16(  CHSV(baseclr + random8(64), 255, random8(128,255)),
                                    CHSV(baseclr + random8(64), 255, random8(128,255)),
                                    CHSV(baseclr + random8(64), 192, random8(128,255)),
                                    CHSV(baseclr + random8(64), 255, random8(128,255)));    
  }

  FastLED.show();                                                               // Send the 'leds' array out to the actual LED strip.

  EVERY_N_SECONDS(10) {                                                         // Change the current pattern function periodically.
    nextPattern();
  }

} // loop()



#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))                              // Kriegsman came up with this line. I knew what it did at one time. . Something, something make an array with enough room for the function names something. . 



void nextPattern() {                                                            // Add one to the current pattern number, and wrap around at the end.

  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);  // Another Kriegsman piece of magic.
  
} // nextPattern()



