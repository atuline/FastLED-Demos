
// noise8
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// This is uses a FastLED noise generator.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//


#include <FastLED.h>                                           // FastLED library
 
#define LED_DT 13                                              // Data pin
#define NUM_LEDS 24                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  128                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array


// Initialize global variables for sequences
int thisdelay = 40;                                                // A delay value for the sequence(s)

uint32_t x,v_time,hue_time;
int hxy = 12630;
uint8_t octaves=3;
uint8_t hue_octaves=2;
int xscale=30;
int hue_scale=8;
int hue_speed=5;
int x_speed=25;


void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA
} // setup()


void loop() {
  mynoise();
}


void mynoise() {
  fill_noise8(leds, NUM_LEDS, octaves, x, xscale, hue_octaves, hxy, hue_scale, hue_time);
  x += x_speed;
  hue_time += hue_speed;
  LEDS.show();
  delay(thisdelay);
}
