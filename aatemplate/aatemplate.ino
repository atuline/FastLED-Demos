
/* Display Template for FastLED

By: Andrew Tuline

Date: Oct, 2014

This is a simple FastLED display sequence template. This template also includes a time (rather than loop) based demo sequencer as well as a frame rate counter.

Rather than use the standard 'FastLED.show()', I'm using the Power managed show, in order to ensure we don't exceed maximum current draw.

*/


#include "FastLED.h"                                          // FastLED library.


#if FASTLED_VERSION < 3001000                                 // Add this if you want to ensure they're using a minimum version of FastLED
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif 
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Define variables used by the sequences.
int      twinkrate = 100;                                     // The higher the value, the lower the number of twinkles.
uint8_t  thisdelay =  20;                                     // A delay value for the sequence(s).
uint8_t   thisfade =   8;                                     // How quickly does it fade? Lower = slower fade rate.
uint8_t    thishue =  50;                                     // The hue.
uint8_t    thissat = 100;                                     // The saturation, where 255 = brilliant colours.
uint8_t    thisbri = 255;                                     // Brightness of a sequence.
bool       randhue =   0;                                     // Do we want random colours all the time? 1 = yes.


void setup() {
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(57600);                                        // Setup debug capabilities, make sure your IDE speed matches

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);          // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  random16_set_seed(4832);                                     // Awesome randomizer left here for reference
  random16_add_entropy(analogRead(2));

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA.
} // setup()


void loop () {
  ChangeMe();                                                 // Check the demo loop for changes to the variables.
  twinkle();                                                  // Call our sequence.
  show_at_max_brightness_for_power();                         // Power managed display of LED's.
  delay_at_max_brightness_for_power(2.5*thisdelay);           // Power managed FastLED delay.
  Serial.println(LEDS.getFPS());                              // Display frames per second on the serial monitor. Disable the delay in order to see how fast/efficient your sequence is.
} // loop()


void twinkle() {
  if (twinkrate < NUM_LEDS) twinkrate = NUM_LEDS;             // Makes sure the twinkrate will cover ALL of the LED's as it's used as the maximum LED index value.
  int i = random16(twinkrate);													      // A random number based on twinkrate. Higher number => fewer twinkles.
  if (randhue) thishue = random16(0,255);                     // Randomize every LED if TRUE
  if (i < NUM_LEDS) leds[i] = CHSV(thishue, thissat, thisbri); // Only the lowest probability twinkles will do. You could even randomize the hue/saturation.
  for (int j = 0; j < NUM_LEDS; j++) leds[j].fadeToBlackBy(thisfade); // Use the FastLED fade method.
} // twinkle()


void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  uint8_t secondHand = (millis() / 1000) % 15;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: thisdelay = 20; randhue = 1; thissat=255; thisfade=16; thisdelay=50; twinkrate=20; break;  // You can change values here, one at a time , or altogether.
      case  5: thisdelay = 20; randhue = 0; thishue=128; thisfade=64; thisdelay=10; twinkrate=100; break;
      case 10: thisdelay = 20; thishue=random16(0,64); twinkrate=40; break;                               // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 15: break;                                                                                     // Matches the '15' at the start of this function
    }
  }
} // ChangeMe()
