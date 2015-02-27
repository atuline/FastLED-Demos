/* two_sin_demo

By: Andrew Tuline

Date: Oct, 2014

A demo showing the flexibility of using two sine waves. A little code and lots of variables can be used for awesome effects.

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


struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Most of these variables can be mucked around with. Better yet, add some form of variable input or routine to change them on the fly. 1970's here I come. . 
//

// Don't forget to update resetvar() definitions if you change these.
uint8_t wavebright = 128;                                     // You can change the brightness of the waves/bars rolling across the screen. Best to make them not as bright as the sparkles.
uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thathue = 140;                                        // You can change the starting hue for other wave.
uint8_t thisrot = 0;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t thatrot = 0;                                          // You can change how quickly the hue rotates for the other wave. Currently 0.
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
bool thisdir = 0;                                             // A direction variable that will work with inputs.
int8_t thisspeed = 4;                                         // You can change the speed, and can use negative values.
int8_t thatspeed = 4;                                         // You can change the speed, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
int thisphase = 0;                                            // Phase change value gets calculated.
int thatphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
uint8_t thatcutoff = 192;                                     // You can change the cutoff value to display that wave. Lower value = longer wave.
int thisdelay = 10;                                           // You can change the delay. Also you can change the allspeed variable above. 
uint8_t twinkrun = 1;                                         // Enable/disable twinkles.
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



void loop() {
  ChangeMe();                                                 // Muck those variable around.
  two_sin();                                                  // Simple call to the routine.
  if(twinkrun == 1) twinkleover();                            // You can keep or lose the twinkles.
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(thisdelay*2.5);
} // loop()



void ChangeMe()
{
  uint8_t secondHand = (millis() / 1000) % 60;                // Increase this if you want a longer demo.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  
  // You can change variables, but remember to set them back in the next demo, or they will stay as is.
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0) {thisrot = 1; thatrot = 1;}                                             // Both rotating hues
    if( secondHand == 5)  {thisrot = 0; thisdir = 1;}                                                          // Just 1 rotating hue
    if( secondHand == 10)  {thatrot = 0; thishue = 255; thathue = 255;}                           // No rotating hues, all red.
    if( secondHand == 15)  {twinkrun = 0;}                                                        // Enough with the damn twinkles.
    if( secondHand == 20)  {allfreq = 16; thathue = 128;}                                         // Time to make a wider bar.
    if( secondHand == 25)  {thiscutoff = 96; thatcutoff = 240;}                                   // Change width of bars.
    if( secondHand == 30)  {thiscutoff = 96; thatcutoff = 96; thisrot = 1;}                       // Make those bars overlap, and rotate a hue
    if( secondHand == 35)  {thisspeed= -4; thatspeed= -4;}                                        // Change direction                                      // Change the direction.
    if( secondHand == 40)  {thiscutoff = 128; thatcutoff = 128; wavebright = 64; twinkrun = 1;}   // Yet more changes
    if( secondHand == 45)  {wavebright = 128; twinkrun = 0; thisspeed = 3;}                       // Now, we change speeds.
    if( secondHand == 50)  {thisspeed = 3; twinkrun = 0; thatspeed = -3;}                            // Opposite directions
    if( secondHand == 55)  {resetvar(); }                                                         // Getting complicated, let's reset the variables.
  }
} // ChangeMe()



void two_sin() {                                                              // This is the heart of this program. Sure is short.

  if (thisdir == 0) {thisphase += thisspeed; thatphase += thatspeed;} 
  else {thisphase -= thisspeed; thatphase -= thatspeed;}

  thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  thathue = thathue + thatrot;                                                // It's also fun for thatwave.
  
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);     // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int thatbright = qsubd(cubicwave8((k*allfreq)+128+thatphase), thatcutoff); // This wave is 180 degrees out of phase (with the value of 128).

    leds[k] = CHSV(thishue, allsat, thisbright);                              // Assigning hues and brightness to the led array.
    leds[k] += CHSV(thathue, allsat, thatbright);                      
  }
} // two_sin()




void twinkleover() {                                                          // This has been added for Mark Kriegsman.
  for (int i = 0; i < numtwinks; i++) {
    if (mytwinks[i].twinkbright <0) {
      mytwinks[i].twinkled = random8(0, NUM_LEDS-1);
      mytwinks[i].twinkbright = random8(220, 255);
    }
    leds[mytwinks[i].twinkled] = CHSV(80, 120, mytwinks[i].twinkbright);      // Trying to make a soft white twinkle
    mytwinks[i].twinkbright -= 8;
  }
} // twinkle()



void resetvar() {                      // Reset the variable back to the beginning.
  wavebright = 128;                    // You can change the brightness of the waves/bars rolling across the screen. Best to make them not as bright as the sparkles.
  thishue = 0;                         // You can change the starting hue value for the first wave.
  thathue = 140;                       // You can change the starting hue for other wave.
  thisrot = 0;                         // You can change how quickly the hue rotates for this wave. Currently 0.
  thatrot = 0;                         // You can change how quickly the hue rotates for the other wave. Currently 0.
  allsat = 255;                        // I like 'em fully saturated with colour.
  thisspeed = 4;                       // You can change the speed, and use negative values.
  thatspeed = 4;                       // You can change the speed, and use negative values.
  allfreq = 32;                        // You can change the frequency, thus overall width of bars.
  thisphase = 0;                       // Phase change value gets calculated.
  thatphase = 0;                       // Phase change value gets calculated.
  thiscutoff = 192;                    // You can change the cutoff value to display this wave. Lower value = longer wave.
  thatcutoff = 192;                    // You can change the cutoff value to display that wave. Lower value = longer wave.
  thisdelay = 10;                      // You can change the delay. Also you can change the allspeed variable above. 
  twinkrun = 1;                        // Enable/disable twinkles.
} // resetvar()
