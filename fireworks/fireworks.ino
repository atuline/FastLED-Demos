
/* Fireworks for FastLED 3.0 or greater

By: Andrew Tuline

Date: Oct, 2014

Non-bouncing one dimensional fireworks demonstration.

The variables are tuned to a pretty short LED strip. You'll need to play around for a longer strip.

FastLED is available at https://github.com/FastLED/FastLED

*/


#include "FastLED.h"                                          // FastLED library.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for WS2812B or WS2801
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER GRB                                       // Are they RGB, GRB or what??
#define LED_TYPE APA102                                       // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 20                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.



// Initialize global variables for sequences
int thisdelay = 20;                                           // A delay value for the sequence(s)                        ********* CHANGE ME **************

int thissat = 255;                                            // Standard fully saturated LED
int thisbright = 255;                                         // Standard fully bright LED

// GRAVBALL VARIABLES
int gravity = -8;                                             // Gravity                                                 ********* CHANGE ME **************
int drag = 0;                                                 // Not required due to losses in 16 bit math. Not used.
int timeinc = 2;                                              // A time increment.
int maxcount = 100;                                           // Maximum number of explosion frames.
int mycount = 0;                                              // Repeat the explosion counter.


// Inital speed variables can be changed to make it go HIGHER!!!
int streamervelocity = 500;                                   // Velocity of the initial streamer that goes into the air. ********* CHANGE ME **************
int explosionvelocity = 500;                                  // Maximum velocity of the explosion.                       ********* CHANGE ME **************


uint8_t thisstatus = 0;                                       // Used to determine which state our finite state machine is in.

#define numgravs 6                                            // How many gravs we are using. The first one is our streamer.  ********* CHANGE ME **************

typedef struct {                                              // Define a structure for the gravs.
      long distold;                                           // I defined this as 'long' so that it'll work with longer strips.
      long distance;                                          // Ditto.
      int velold;
      int velocity;
      int thishue;
      int thissat;
      int thisbright;
  }  gravs;

gravs mygravs[numgravs];



void setup() {
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // For WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801

  FastLED.setBrightness(max_bright);                          // Too bright chews up a lot of current and is overkill at nighttime.
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA.
} // setup()



void loop () {
  fire();                                                     // It's really a finite state machine, so no huge 'for' loops.
  show_at_max_brightness_for_power();                         // Display the LED's
  delay_at_max_brightness_for_power(thisdelay*2.5);           // And delay
  Serial.println(LEDS.getFPS());
} // loop()



void fire() {                                                 // States of fireworks, need to be loop friendly.
  switch (thisstatus) {
    case 0: firestart(); break;                               // Initialize the first element of array to shoot into the 'sky'.
    case 1: fireup(); break;                                  // Send it up. Once it reaches the top, we change status to '2'
    case 2: explodeinit(); break;                             // Initialize the array for the remainder of the fireworks elements. and change status.
    case 3: explode(); break;                                 // Now, let's explode them for a count duration and reset to 0 when done.
    default: break;
  }
} // fire()



void firestart() {                                            // Initialize variable for a new streamer being shot into the 'sky'.
  mygravs[0].distold = 0;
  mygravs[0].distance = 0;
  mygravs[0].velold = streamervelocity;                       // Goes to a maximum distance of ~18000. Needs more LED's
  mygravs[0].velocity = streamervelocity;
  mygravs[0].thishue = random8();                             // Might as well make it multi-coloured
  mygravs[0].thissat = 50;                                    // But not very saturated
  mygravs[0].thisbright = 128;
  thisstatus = 1;
} // firestart()



void fireup() {                                               // Shoot fireworks into the air.

  mygravs[0].velocity = mygravs[0].velold + gravity*timeinc;  // Split gravity math into two lines for simplicity.
  mygravs[0].distance = mygravs[0].distold + mygravs[0].velocity*timeinc;

  int i = map(mygravs[0].distance, 0, 32767, 0, NUM_LEDS);
    
  mygravs[0].velold = mygravs[0].velocity;                    // Capture the current velocity/distance.
  mygravs[0].distold = mygravs[0].distance;

  if (i < NUM_LEDS) leds[i] = CHSV(mygravs[0].thishue, mygravs[0].thissat, mygravs[0].thisbright);     // Let's get ready to display it, but not if it's too high.
  if(mygravs[0].velold <= 0) thisstatus = 2;                  // Starting to fall, so let's stop this nonsense.

  for (int j = 0; j < NUM_LEDS; j++) leds[j].fadeToBlackBy(16);  // The trail fades slowly over time.

  if (i > 3) {                                                // Add a bit of a sparkly trailer above the ground, but make sure that trailer is ABOVE the ground.
    if (random8() >220) {
      leds[i-3] = CHSV(mygravs[0].thishue, random8(100, 200), mygravs[0].thisbright);  // Add a bit more saturation to the colour.
    }
  }
} // fireup()


void explodeinit () {
  for (int k=1; k< numgravs; k++) {
    mygravs[k].distold = mygravs[0].distance;                 // We start off with a height where the original element stopped.
    mygravs[k].distance = mygravs[0].distance;
    mygravs[k].velold = random16(-50, explosionvelocity);     // Kablooey!!
    mygravs[k].velocity = mygravs[k].velold;
    mygravs[k].thishue = random8();                           // Different coloured elements, which are
    mygravs[k].thissat = thissat;                             // rich in colour
    mygravs[k].thisbright = thisbright;                       // and very bright.
  }
  mycount = 0;                                                // This is a counter for the # of times to go through the exploding loop.
  thisstatus = 3;                                             // Once done initializing, let's move onto the next step of exploding.
} // explodeinit()


void explode() {

  if (mycount++ >=maxcount) thisstatus = 0;                   // Only do this so many times.

  for (int k=1; k < numgravs; k++) {

    mygravs[k].velocity = mygravs[k].velold + gravity*timeinc;               // Split gravity math into two lines for simplicity
    mygravs[k].distance = mygravs[k].distold + mygravs[k].velocity*timeinc;

    int i = map(mygravs[k].distance, 0, 32767, 0, NUM_LEDS);
    
    mygravs[k].velold = mygravs[k].velocity;                                 // Capture the current velocity/distance
    mygravs[k].distold = mygravs[k].distance;

    if (mygravs[k].distance < 0) {mygravs[k].thisbright = 0;}                // It landed. Sorry, no bounce.
    
    if (i < NUM_LEDS && i >= 0) leds[i] += CHSV(mygravs[k].thishue, thissat, thisbright);    // Let's get ready to display it, but be careful of limits.
  }
  nscale8(leds,NUM_LEDS,224);                          // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);
} // explode()
