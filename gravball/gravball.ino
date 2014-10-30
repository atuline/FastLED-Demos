
// Gravball for FastLED 2.1 or greater
//
// By: Andrew Tuline
//
// Date: Oct, 2014
//
// Multiple bouncing balls. For whatever reason, I call them gravs.
// 
// FastLED is available at https://github.com/FastLED/FastLED
//


#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for WS2812B or WS2801
#define COLOR_ORDER GRB                                       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B                                       // What kind of strip are you using?
#define NUM_LEDS 24                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Initialize global variables for sequences
int thisdelay = 15;                                            // A delay value for the sequence(s)
int thissat = 255;
int thisbright = 255;

// GRAVBALL VARIABLES
int gravity = -15;
int drag = 0;                   // Not required due to losses in 16 bit math.
int timeinc = 2;

#define numgravs 6              // How many gravs we are using.

typedef struct {                // Define a structure for the gravs.
      int distold;
      int distance;
      int velold;
      int velocity;
      int thishue;
  }  gravs;

gravs mygravs[numgravs];



void setup() {
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA
} // setup()



void loop () {
  gravball();
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(thisdelay*2.5);
  Serial.println(LEDS.getFPS());
} // loop()



void gravball() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (int k=0; k < numgravs; k++) {

    mygravs[k].velocity = mygravs[k].velold + gravity*timeinc;               // Split gravity math into two lines for simplicity
    mygravs[k].distance = mygravs[k].distold + mygravs[k].velocity*timeinc;

    int i = map(mygravs[k].distance, 0, 32767, 0, NUM_LEDS);
    
    mygravs[k].velold = mygravs[k].velocity;                                 // Capture the current velocity/distance
    mygravs[k].distold = mygravs[k].distance;

    if (i <= 1 && abs(mygravs[k].velold) < 700 ) {mygravs[k].velold = 0; mygravs[k].distold=random16(0,8000)+26000; mygravs[k].thishue=random8(255);}      // Reset!!!
    if (i <= 1 && mygravs[k].velold<0) {mygravs[k].velold = -mygravs[k].velold;}         // Bounce!!!
    leds[i] += CHSV(mygravs[k].thishue, thissat, thisbright);          // Let's get ready to display it.
  }
} // gravball()
