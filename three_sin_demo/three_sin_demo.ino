
/* three_sin demo

By: Andrew Tuline

Date: Oct, 2014

3 sine waves, one for each colour. I'm already doing a lot with 2 sine waves, so I didn't take this far.

*/


#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for WS2812B or WS2801
#define LED_CK 11
#define COLOR_ORDER BGR                                       // Are they RGB, GRB or what??
#define LED_TYPE APA102                                       // What kind of strip are you using?
#define NUM_LEDS  12                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 64;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Initialize global variables for sequences
uint8_t thisdelay = 20;                                       // A delay value for the sequence(s)

int wave1=0;                                            // Current phase is calculated.
int wave2=0;
int wave3=0;

uint8_t inc1 = 2;                                       // Phase shift. Keep it low, as this is the speed at which the waves move.
uint8_t inc2 = 1;
uint8_t inc3 = -2;

uint8_t lvl1 = 80;                                      // Any result below this value is displayed as 0.
uint8_t lvl2 = 80;
uint8_t lvl3 = 80;

uint8_t mul1 = 20;                                      // Frequency, thus the distance between waves
uint8_t mul2 = 25;
uint8_t mul3 = 22;



void setup() {
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA

  random16_set_seed(4832);                                     // Awesome randomizer (which we don't yet need here.)
  random16_add_entropy(analogRead(2));

} // setup()



void loop () {
  ChangeMe();
  three_sin();                                                  // Improved method of using non-blocking delay
  show_at_max_brightness_for_power();                         // Power managed display of LED's 
  delay_at_max_brightness_for_power(thisdelay*2.5);
} // loop()



void three_sin() {
  wave1 += inc1;
  wave2 += inc2;
  wave3 += inc3;
  for (int k=0; k<NUM_LEDS; k++) {
    leds[k].r = qsub8(sin8(mul1*k + wave1/128), lvl1);         // Another fixed frequency, variable phase sine wave with lowered level
    leds[k].g = qsub8(sin8(mul2*k + wave2/128), lvl2);         // A fixed frequency, variable phase sine wave with lowered level
    leds[k].b = qsub8(sin8(mul3*k + wave3/128), lvl3);         // A fixed frequency, variable phase sine wave with lowered level
  }
} // three_sin()



void ChangeMe()
{
  uint8_t secondHand = (millis() / 1000) % 60;                // Increase this if you want a longer demo.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  
  // You can change variables, but remember to set them back in the next demo, or they will stay as is.
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  {thisdelay = 8; mul1 = 20; mul2 = 25; mul3 = 22; lvl1 = 80; lvl2 = 80; lvl3 = 80; inc1 = 1; inc2 = 1; inc3 = -1;}
//  if( secondHand == 5)   {mul1 = 5; mul2 = 8; mul3 = 7;}
//  if( secondHand == 10)  {thisdelay = 20; lvl1 = 220; lvl2 = 220; lvl3 = 220;}
    if( secondHand == 15)  {}
    if( secondHand == 20)  {}
    if( secondHand == 25)  {}
    if( secondHand == 30)  {}
    if( secondHand == 35)  {}
    if( secondHand == 40)  {}
    if( secondHand == 45)  {}
    if( secondHand == 50)  {}
    if( secondHand == 55)  {}
  } // if lastSecond
} // ChangeMe()
