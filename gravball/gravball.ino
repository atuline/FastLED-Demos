
// Gravball
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// Multiple bouncing balls. For whatever reason, I call them gravs.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//


#include <FastLED.h>                                           // FastLED library
 
#define LED_DT 13                                              // Data pin
#define NUM_LEDS 24                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  196                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array


// Initialize global variables for sequences
int thisdelay = 15;                                                 // A delay value for the sequence(s)
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
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA
} // setup()


void loop () {
  gravball();
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
  show_at_max_brightness_for_power();
  
//  FastLED.delay(thisdelay*2.5);
  delay_at_max_brightness_for_power(thisdelay*2.5);
//  delay(thisdelay);

} // gravball()
