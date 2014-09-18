
// TITLE
//
// Originally from: the Neopixel Ampli-Tie project.
//
// By: Andrew Tuline
//
// Date: Aug, 2014
//
// Basic code to read from the Sparkfun INMP401 microphone and display CHSV output.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//


#include <FastLED.h>                                           // FastLED library
 
#define LED_DT 13                                              // Data pin
#define NUM_LEDS 24                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  196                                        // How bright do we want to go


#define MIC_PIN 5                                              // Analog port for microphone

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array


// Initialize global variables for sequences
int thisdelay = 4;                                                 // A delay value for the sequence(s)

#define DC_OFFSET  32                                          // DC offset in mic signal - if unusure, leave 0
 

void setup() {
  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);
  
  Serial.begin(9600);         // DEBUG

  LEDS.addLeds<LED_TYPE, LED_DT, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps( 5, 1000);
} // setup()


void loop() {
  memsmic();
}

void memsmic() {
  int n;
  for (int i = 0; i<NUM_LEDS; i++) {
    n = analogRead(MIC_PIN);                                   // Raw reading from mic
    n = abs(n - 512 - DC_OFFSET);                              // Center on zero
    leds[i] = CHSV((n*2 % 255), 255, (n*2)% 255);

//  FastLED.delay(thisdelay*2.5);
//  delay_at_max_brightness_for_power(thisdelay*2.5);
  delay(thisdelay);

  show_at_max_brightness_for_power();
  }
}