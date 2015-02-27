/* one_sine_demo

By: Andrew Tuline

Date: Oct, 2014

A demo showing the flexibility of just using a single sine wave. A little code and a lot of variables can go a long way. This includes an optional twinkle overlay for Mark Kriegsman.

*/

#define qsubd(x, b)  ((x>b)?wavebright:0)                   // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                          // Analog Unsigned subtraction macro. if result <0, then => 0


#include "FastLED.h"                                          // FastLED library.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Most of these variables can be mucked around with. Better yet, add some form of variable input or routine to change them on the fly. 1970's here I come. . 

// Don't forget to update resetvar() definitions if you change these.
uint8_t wavebright = 128;                                     // You can change the brightness of the waves/bars rolling across the screen. Best to make them not as bright as the sparkles.
uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 0;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
bool thisdir = 0;                                             // You can change direction.
int8_t thisspeed = 4;                                         // You can change the speed, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
int loopdelay = 10;                                           // You can change the delay. Also you can change the allspeed variable above. 
uint8_t twinkrun = 1;                                         // Enable/disable twinkles.

uint8_t bgclr = 0;                                            // A rotating background colour.
// End of resetvar() redefinitions.


typedef struct {                                              // Define a structure for the twinkles that get overlaid on the moving waves.
      int twinkled;                                           // Supports a long strand of LED's.
      int twinkbright;                                        // Defined as 'int', so that we can trigger change on a negative brightness value.
  }  twinks;

#define numtwinks 4                                           // You can change the number of twinkles.
twinks mytwinks[numtwinks];                                   // The structure is called mytwinks.



void setup() {
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);          // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA
  FastLED.setBrightness(max_bright);
} // setup()



void loop()
{
  ChangeMe();                                                 // Muck those variable around.
  one_sin();                                                  // Simple call to the routine.
  if(twinkrun == 1) twinkover();                              // You can keep or lose the twinkles.
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(loopdelay*2.5);
  Serial.println(LEDS.getFPS());
} // loop()



void ChangeMe()
{
  uint8_t secondHand = (millis() / 1000) % 60;                // Increase this if you want a longer demo.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  
  // You can change variables, but remember to set them back in the next demo, or they will stay as is.
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if (secondHand ==  0)  {twinkrun = 1; thisrot = 1; thiscutoff=254; allfreq=8;}                                                         // Both rotating hues
    if (secondHand ==  5)  {thisrot = 0; thisdir=1;}                            // Just 1 rotating hue
    if (secondHand == 10)  {thishue = 255;}                                     // No rotating hues, all red.
    if (secondHand == 15)  {twinkrun = 0;}                                      // Enough with the damn twinkles.
    if (secondHand == 20)  {allfreq = 16;}                                      // Time to make a wider bar.
    if (secondHand == 25)  {thiscutoff = 96;}                                   // Change width of bars.
    if (secondHand == 30)  {thiscutoff = 96; thisrot = 1;}                      // Make those bars overlap, and rotate a hue
    if (secondHand == 35)  {thisdir = 1;}                                       // Change the direction.
    if (secondHand == 40)  {thiscutoff = 128; wavebright = 64; twinkrun = 1;}   // Yet more changes
    if (secondHand == 45)  {wavebright = 128; twinkrun = 0; thisspeed = 3;}     // Now, we change speeds.
    if (secondHand == 50)  {thisdir = 0; twinkrun = 0;}                         // Opposite directions
    if (secondHand == 55)  {resetvar(); }                                       // Getting complicated, let's reset the variables.
  }
} // ChangeMe()



void one_sin() {                                                                // This is the heart of this program. Sure is short.
  if (thisdir == 0) thisphase+=thisspeed; else thisphase-=thisspeed;            // You can change direction and speed individually.
    thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, 40);
    leds[k] += CHSV(thishue, allsat, thisbright);                               // Assigning hues and brightness to the led array.
  }
  bgclr++;
} // one_sin()



void twinkover() {                                                              // Twinkle overlay has been added for Mark Kriegsman.
  for (int i = 0; i < numtwinks; i++) {
    if (mytwinks[i].twinkbright <0) {
      mytwinks[i].twinkled = random8(0, NUM_LEDS-1);
      mytwinks[i].twinkbright = random8(220, 255);
    }
    leds[mytwinks[i].twinkled] = CHSV(80, 120, mytwinks[i].twinkbright);        // Trying to make a soft white twinkle
    mytwinks[i].twinkbright -= 8;
  }
} // twinkle()



void resetvar() {                                             // Reset the variable back to the beginning.
  wavebright = 128;                                           // You can change the brightness of the waves/bars rolling across the screen. Best to make them not as bright as the sparkles.
  thishue = 0;                                                // You can change the starting hue value for the first wave.
  thisrot = 0;                                                // You can change how quickly the hue rotates for this wave. Currently 0.
  allsat = 255;                                               // I like 'em fully saturated with colour.
  thisdir = 0;                                                // You can change direction.
  thisspeed = 4;                                              // You can change the speed, and use negative values.
  allfreq = 32;                                               // You can change the frequency, thus overall width of bars.
  thisphase = 0;                                              // Phase change value gets calculated.
  thiscutoff = 192;                                           // You can change the cutoff value to display this wave. Lower value = longer wave.
  loopdelay = 10;                                             // You can change the delay. Also you can change the allspeed variable above. 
  twinkrun = 1;                                               // Enable/disable twinkles.
} // resetvar()
