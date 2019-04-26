/* soundmems
 *
 * By: Andrew Tuline
 *
 * Date: October, 2014
 *
 * Basic code to read from the Sparkfun INMP401 microphone and display CHSV output.
 *
 */

#define qsubd(x, b)  ((x>b)?b:0)                              // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 60                                           // Number of LED's.

#define MIC_PIN    5                                          // Analog port for microphone
#define DC_OFFSET  0                                          // Not used at this point. DC offset in mic signal - if unusure, leave 0


// Initialize changeable global variables.
uint8_t max_bright = 192;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

int wavebright = 20;



void setup() {
  
  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.). Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V. COMMENT OUT THE FOLLOWING LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);
  
  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);

} // setup()



void loop() {

  EVERY_N_MILLISECONDS(20) {
    soundmems();
  }
  
  FastLED.show();

} // loop()



void soundmems() {

  int n;
  n = analogRead(MIC_PIN);
  n = qsuba(abs(n-512), 10);
//  Serial.println(n);
  nblend(leds[0], CHSV(n,255,n), 128);
  for (int i = NUM_LEDS-1; i>0; i--) {
    leds[i] = leds[i-1];
  }

} // soundmems()

