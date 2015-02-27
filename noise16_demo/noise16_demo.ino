
/* noise16_demo

By: Andrew Tuline

Date: Oct, 2014

This is a simple FastLED display sequence utilizing the FastLED noise function. This is one of those 'wait for a while' and see what it does routines. Very unpredictable and very cool.

*/


#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0


#include "FastLED.h"                                          // FastLED library.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initializxe our array


// Initialize noise specific variables
uint8_t thisdelay = 20;                                       // A delay value for the sequence(s)
uint8_t thisdir = 0;                                          // We can reverse the direction.
uint32_t  x,hue_time;                                         // x & time values
uint8_t octaves=2;       //2                                  // how many octaves to use for the brightness
uint8_t hue_octaves=3;   //3                                  // how many octaves to use for the hue
int xscale=57771;        //57771                              // the 'distance' between points on the x axis
uint32_t hxy = 43213;    //43213                              // not sure about this
int hue_scale=20;        //1                                  // the 'distance' between points for the hue noise
int hue_speed = 1;       //31                                 // how fast we move through hue noise
uint8_t x_speed = 0;     //331                                // adjust this value to move along the x axis between frames
int8_t hxyinc = 3;
uint8_t wavebright= 128;                                      // Usesd by qsub to set a fixed value to LED's depending on their current value


void setup() {
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);          // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);                // FastLED 2.1 Power management set at 5V, 500mA

  random16_set_seed(4832);                                     // Awesome randomizer
  random16_add_entropy(analogRead(2));
} // setup()



void loop () {
  ChangeMe();
  noise16();
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(thisdelay*2.5);
  Serial.println(LEDS.getFPS());
} // loop()



void noise16() {
  fill_noise16(leds, NUM_LEDS, octaves, x, xscale, hue_octaves, hxy, hue_scale, hue_time);
  if (thisdir == 0) {hxy+=hxyinc; x += x_speed; hue_time+= hue_speed;} else { hxy-=hxyinc; x -= x_speed; hue_time -= hue_speed;}

//  for (int i=0; i<NUM_LEDS; i++) {                            // We can filter, we can invert, we can do all sorts of things.
//    leds[i].r = 0;                                          // Totally filter out red.
//    leds[i].g = 0;
//    leds[i].b = 0;
//    leds[i].r =qsuba(leds[i].r, 32);                        // Set a min value to display.
//    leds[i].g =qsubd(leds[i].g, 32);                        // Set a min value to display.
//    leds[i].b =qsuba(leds[i].b, 32);                        // Set a min value to display.
//  }

} // noisy()



void ChangeMe() {
  uint8_t secondHand = (millis() / 1000)%60 ;                 // Increase this if you want a longer demo. Currently changes once per minute.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if (secondHand == 0)  {hxyinc = 3; octaves = 1; hue_octaves = 2; hxy = 6000; x = 5000; xscale=3000; hue_scale=50; hue_speed=15; x_speed=100; }
    if (secondHand == 10) {hxyinc = random16(1,15); octaves=random16(1,3); hue_octaves=random16(1,5); hue_scale=random16(10, 50);  x = random16(); xscale=random16(); hxy= random16(); hue_time = random16(); hue_speed = random16(1,3); x_speed = random16(1,30); }
    if (secondHand == 20) {}
    if (secondHand == 30) {}
    if (secondHand == 40) {}
    if (secondHand == 50) {}
  }
} // ChangeMe()
