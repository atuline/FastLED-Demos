
// Rainbow Bounce
//
// By: Andrew Tuline
//
// Date: Sep, 2014
//
// A bar of led's with a rotating hue.
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
#define BRIGHTNESS  128                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array


// Initialize global variables for sequences
uint8_t thisdelay = 50;                                        // A delay value for the sequence(s)
uint8_t thisbounce = 0;
uint8_t thishue = 90;

void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);                // FastLED 2.1 Power management set at 5V, 500mA
} // setup()


void loop () {
 rainbow_bounce();                                             
} // loop()


void rainbow_bounce() {
  for (int i = 0; i <NUM_LEDS-8; i++) {                       // Fill from starting array value for 8 LED's  
    fill_rainbow(&(leds[i]), 8, thishue);                     // The only problem, is that it will overwrite above thearray.
    thishue+=2;
    show_at_max_brightness_for_power();
    fill_solid(leds, NUM_LEDS, CRGB::Black);

//  FastLED.delay(thisdelay*2.5);
//  delay_at_max_brightness_for_power(thisdelay*2.5);
  delay(thisdelay);

  } // for i

  for (int i = NUM_LEDS-8; i>0; i--) {
    fill_rainbow(&(leds[i]), 8, thishue);                     // Fill from starting array value for 8 LED's  
    thishue+=2;
    show_at_max_brightness_for_power();
    fill_solid(leds, NUM_LEDS, CRGB::Black);
//  FastLED.delay(thisdelay*2.5);
//  delay_at_max_brightness_for_power(thisdelay*2.5);
  delay(thisdelay);

  } // for i

} // rainbow_bounce()

