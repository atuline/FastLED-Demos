// Fastbounce
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// A bar of LED's moving up and down the strand with rotating hues.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//

#include <FastLED.h>

#define LED_PIN     13
#define NUM_LEDS    24
#define BRIGHTNESS  128
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

int TOP_INDEX = int(NUM_LEDS/2);

  int thisdelay = 40;
  int thisinc = 1;

  int thishue = 0;
  int thisbright = BRIGHTNESS;

  int mylen = 0;
  int mylenold = 20;


void setup() {
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
}


void loop() {
  rainbow_bounce();
}


void rainbow_bounce() {
  for (int i = 0; i <NUM_LEDS-8; i++) {

//  fill_rainbow(leds, NUM_LEDS, 1);                           // Fill a complete string with a rainbow
//  fill_solid(leds, NUM_LEDS, CRGB::Red);
//  fill_solid(&(leds[i]), 8, CRGB::Green);

    fill_rainbow(&(leds[i]), 8, thishue);                      // Fill from starting array value for 8 LED's  
                                                               // The only problem, is that it will overwrite above thearray.
    thishue+=2;
    FastLED.show();
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    delay(thisdelay);
  } // for i

for (int i = NUM_LEDS-8; i>0; i--) {

//  fill_rainbow(leds, NUM_LEDS, 1);                           // Fill a complete string with a rainbow
//  fill_solid(leds, NUM_LEDS, CRGB::Red);
//  fill_solid(&(leds[i]), 8, CRGB::Green);

    fill_rainbow(&(leds[i]), 8, thishue);                      // Fill from starting array value for 8 LED's  
    thishue+=2;
    FastLED.show();
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    
    delay(thisdelay);
  } // for i
} // rainbow_bounce()
