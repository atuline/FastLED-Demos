
/* Matrix

By: funkboxing LED and significantly modified by Andrew Tuline

Date: Oct, 2014

Green (or other coloured) characters going up/down the strand, a la 'The Matrix'.

*/


#include "FastLED.h"                                          // FastLED library.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 20                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Initialize global variables for sequences
int thisdelay = 50;                                           // A delay value for the sequence(s)
int thishue = 95;
int thissat = 255;
int thisdir = 0;
bool huerot = 0;                                               // Does the hue rotate? 1 = yes


void setup() {
  Serial.begin(57600);
  
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // For WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA
} // setup()



void loop () {
  ChangeMe();
  matrix();
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(thisdelay*2.5);
  Serial.println(LEDS.getFPS());
} // loop()



void matrix() {                                               // One line matrix

  if (huerot) thishue=thishue+5;

  if (random16(90) > 80) {
    if (thisdir == 0) leds[0] = CHSV(thishue, thissat, 255); else leds[NUM_LEDS-1] = CHSV(thishue, thissat, 255);
  }
  else {leds[0] = CHSV(thishue, thissat, 0);}

  if (thisdir == 0) {
    for (int i = NUM_LEDS-1; i >0 ; i-- ) leds[i] = leds[i-1];
  } else {
    for (int i = 0; i < NUM_LEDS ; i++ ) leds[i] = leds[i+1];
  }
} // matrix()



void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  uint8_t secondHand = (millis() / 1000) % 60;                // Change '25' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    if (secondHand ==  0)  {thisdelay=30; thishue=95; huerot=0;}
    if (secondHand ==  5)  {thisdir=1; huerot=1;}
    if (secondHand == 10)  {thisdelay=10; thishue=0; huerot=0;}
    if (secondHand == 15)  {thisdelay=20; thishue=random8();}
    if (secondHand == 20)  {thishue=random8(); huerot=1;}
    if (secondHand == 25)  { }
    if (secondHand == 30)  { }
    if (secondHand == 35)  { }
    if (secondHand == 40)  { }
    if (secondHand == 45)  { }
    if (secondHand == 50)  { }
    if (secondHand == 55)  { }
  }
} // ChangeMe()
