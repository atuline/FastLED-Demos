/* lightnings
 *
 * By: Daniel Wilson
 *
 * Modified by: Andrew Tuline
 * Furher modified to be less blocking by @PanMan
 *
 * Date: January/August, 2017
 *
 * Lightnings is a program that lets you make an LED strip look like a 1D cloud of lightning.
 * I wouldn't use this code in conjunction with any controls because it uses those nasty blocking delays which I haven't bothered to remove.
 *
 */

#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 60                                           // Number of LED's.

uint8_t frequency = 30;                                       // controls the interval between strikes
#define MAX_FLASHES 8                                         //the upper limit of flashes per strike
unsigned int toflash=4; //Numer of times to flash
unsigned int dimmer = 1;
uint8_t ledstart;                                             // Starting location of a flash
uint8_t ledlen;
int flashCounter = 0;


// Global variables can be changed on the fly.
uint8_t max_bright = 255;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

uint8_t frequency = 50;                                       // controls the interval between strikes
uint8_t flashes = 8;                                          //the upper limit of flashes per strike
unsigned int dimmer = 1;

uint8_t ledstart;                                             // Starting location of a flash
uint8_t ledlen;                                               // Length of a flash



void setup() {

  Serial.begin(57600);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
} // setup()

void loop(){
  lightning();
  //Do other stuff such as checking buttons
}


void lightning() {
  //Based on https://github.com/atuline/FastLED-Demos/blob/master/lightnings/lightnings.ino but rewritten to be less blocking for buttons
  if (flashCounter == 0 || flashCounter > toflash) {
    //if is 0 (first loop) or more than planned, recalculate next series
    // Serial.println("Calc flashes!");
    fadeToBlackBy( leds, NUM_LEDS, 255); //Turn to black
    ledstart = random8(NUM_LEDS);                               // Determine starting location of flash
    ledlen = random8(NUM_LEDS-ledstart);                        // Determine length of flash (not to go beyond NUM_LEDS-1)
    toflash=random8(3,MAX_FLASHES);     //Amount of flashes
    flashCounter=0;
    //    Serial.print(ledstart);
    //    Serial.println("-ledstart:");
    //    Serial.print(ledlen);
    //    Serial.println("-ledlen"); 
  }

  flashCounter++;
  if(flashCounter == 0) {
    dimmer = 5;                         // the brightness of the leader is scaled down by a factor of 5
  } else {
    dimmer = random8(1,3);                               // return strokes are brighter than the leader
  }

  fill_solid(leds+ledstart,ledlen,CHSV(255, 0, 255/dimmer));
  FastLED.show();                       // Show a section of LED's
  delay(random8(4,10));                                     // each flash only lasts 4-10 milliseconds
  fill_solid(leds+ledstart,ledlen,CHSV(255,0,0));           // Clear the section of LED's
  FastLED.show();
  if (flashCounter == 0) {
    delay (150);                       // longer delay until next flash after the leader
  }

  delay(50+random8(100));                                   // shorter delay between strokes  

  //  delay(random8(frequency)*100); //Not wanted in my version as I want quicker flashes  
} // lightning

