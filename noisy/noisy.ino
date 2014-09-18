

// Noisy
//
// By: Andrew Tuline
//
// Date: Sept, 2014
//
// This is a simple FastLED (2.1 and greater) display sequence utilizing the FastLED noise function.
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
uint8_t thisdelay = 20;                                       // A delay value for the sequence(s)
uint8_t thisdir = 0;                                          // We can reverse the direction.


// x & time values
uint32_t  x,hue_time;

// how many octaves to use for the brightness and hue functions
uint8_t octaves=4;
uint8_t hue_octaves=2;

// the 'distance' between points on the x axis
int xscale=30;

uint32_t hxy = 0;

// the 'distance' between points for the hue noise
int hue_scale=50;

// how fast we move through hue noise
int hue_speed = 5;

// adjust this value to move along the x axis between frames
uint8_t x_speed = 5;


void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);                // FastLED 2.1 Power management set at 5V, 500mA
  random16_set_seed(4832);
  random16_add_entropy(analogRead(2));
} // setup()


void loop () {
  noisy();
} // loop()


void noisy() {
  fill_noise16(leds, NUM_LEDS, octaves, x, xscale, hue_octaves, hxy, hue_scale, hue_time);
  if (thisdir == 0) x += x_speed; else x -= x_speed;
  hue_time += hue_speed;
  show_at_max_brightness_for_power();

//  FastLED.delay(thisdelay*2.5);
  delay_at_max_brightness_for_power(thisdelay*2.5);
//  delay(thisdelay);
}

