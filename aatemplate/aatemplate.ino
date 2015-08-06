
/* Display Template for FastLED

By: Andrew Tuline

Date: July, 2015

This is a simple non-blocking FastLED display sequence template.

FastLED is available at https://github.com/FastLED/FastLED

*/



#include "FastLED.h"                                          // FastLED library. Please use the latest development version.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 20                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 64;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Define variables used by the sequences.
int      twinkrate = 100;                                     // The higher the value, the lower the number of twinkles.
uint8_t  thisdelay =  60;                                     // A delay value for the sequence(s).
uint8_t   thisfade =   8;                                     // How quickly does it fade? Lower = slower fade rate.
uint8_t    thishue =  50;                                     // The hue.
uint8_t    thissat = 100;                                     // The saturation, where 255 = brilliant colours.
uint8_t    thisbri = 255;                                     // Brightness of a sequence.
bool       randhue =   0;                                     // Do we want random colours all the time? 1 = yes.



void setup() {
  delay(3000);                                                // Power-up safety delay.
  Serial.begin(57600);                                        // Initialize serial port for debugging.

  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812
  
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
} // setup()



void loop () {
  ChangeMe();                                                 // Check the demo loop for changes to the variables.

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    twinkle();
    show_at_max_brightness_for_power();
  }

  Serial.println(LEDS.getFPS());                              // Display frames per second on the serial monitor.
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
    if (secondHand ==  0)  {thisdelay = 60; randhue = 1; thissat=255; thisfade=16; twinkrate=20;}  // You can change values here, one at a time , or altogether.
    if (secondHand ==  5)  {thisdelay = 60; randhue = 0; thishue=128; thisfade=64; twinkrate=100;}
    if (secondHand == 10)  {thisdelay = 60; thishue=random16(0,64); twinkrate=40;}                 // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
  }
} // ChangeMe()

