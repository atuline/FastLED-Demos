
// Moving Dots
//
// By: John Burroughs
// Modified by: Andrew Tuline
//
// Date: Sept, 2014
//
// This is a simple FastLED (2.1 and greater) display sequence using some FastLED functions.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//
// Note: If you receive compile errors (as I have in the Stino add-on for Sublime Text), set the compiler to 'Full Compilation'.
//


#include <FastLED.h>                                           // FastLED library
 
#define LED_DT 13                                              // Data pin
#define NUM_LEDS 24                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  196                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array


// Initialize global variables for sequences
int thisdelay = 4;                                                 // A delay value for the sequence(s)
uint8_t count;                                                     // Count up to 255 and then reverts to 0

long currentMillis = 0;
long previousMillis = 0;


void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
} // setup()


void loop () {
  dots();
} // loop()


void dots() {
currentMillis = millis();
  if(currentMillis - previousMillis > thisdelay) {

  uint8_t middle = 0;
  uint8_t side = 0;
  uint8_t other = 0;
  count++;                                                     // overflowing a byte => 0

  byte x = quadwave8(count);

  other = map(x, 0, 255, NUM_LEDS/4, NUM_LEDS/4*3);            // 1/4 to 3/4 of strip
  side = map(x, 0, 255, 0, NUM_LEDS-1);                        // full length of strip
  middle = map(x, 0, 255, NUM_LEDS/3, NUM_LEDS/3*2);           // 1/3 to 2/3 of strip


  leds[middle] = CRGB::Purple;
  leds[side] = CRGB::Blue;
  leds[other] = CRGB::Aqua;

  LEDS.show();

  for ( byte i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(32);
//    leds[i].nscale8(180);
  }

  LEDS.show();
//  delay(thisdelay);
  previousMillis = currentMillis;
  }
}