/*
#
# Gravities
#
# By: Andrew Tuline
# www.tuline.com
# atuline@gmail.com
#
# Date: July, 2014
#
*/

/*
# LICENSE - Open Source.
*/

/*
# Introduction:
#
# This routine demonstrates bouncing gravs using the Arduino UNO, a strand of WS2812B LED's and the FastLED library.
#
*/

#include <FastLED.h>

#define LED_PIN     13
#define NUM_LEDS    24
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int thisdelay = 20;
int thissat = 255;
int thisbright = BRIGHTNESS;

int gravity = -15;
int drag = 0;                   // Not required due to losses in 16 bit math.
int timeinc = 2;

#define numgravs 6              // How many gravs we are using.

typedef struct {                // Define a structure for the gravs.
      int distanceold;
      int distance;
      int velocityold;
      int velocity;
      int thishue;
  }  gravs;

gravs mygravs[numgravs];          // Array is called mygravs, i.e. mygravs[0].thishue = 80;

void setup() {
  delay(2000);                    // Power-up safety delay or something like that.
  Serial.begin(9600);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
}


void loop() {
  gravball();
}


void gravball() {
  
  memset8( leds, 0x000000, NUM_LEDS * sizeof(CRGB));      // clear the pixel buffer

  for (int k=0; k < numgravs; k++) {

    mygravs[k].velocity = mygravs[k].velocityold + gravity*timeinc;               // Split gravity math into two lines for simplicity
    mygravs[k].distance = mygravs[k].distanceold + mygravs[k].velocity*timeinc;

    int i = map(mygravs[k].distance, 0, 32767, 0, NUM_LEDS);
    
    mygravs[k].velocityold = mygravs[k].velocity;                                 // Capture the current velocity/distance
    mygravs[k].distanceold = mygravs[k].distance;

    if (i <= 1 && abs(mygravs[k].velocityold) < 700 ) {mygravs[k].velocityold = 0; mygravs[k].distanceold=random(0,8000)+26000; mygravs[k].thishue=random(0,255);}      // Reset!!!
    if (i <= 1 && mygravs[k].velocityold<0) {mygravs[k].velocityold = -mygravs[k].velocityold;}         // Bounce!!!
    leds[i] += CHSV(mygravs[k].thishue, thissat, thisbright);          // Let's get ready to display it.
  }
  FastLED.show();
  delay(thisdelay);
}
