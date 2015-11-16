
/* Fast Hartley Transform

By: Andrew Tuline

Date: Oct, 2014

Disclaimer: Apparently, the FHT library (as of this writing) doesn't work with the Teensy.

This is an implementation of the FHT library with FastLED

The FHT library is available at http://wiki.openmusiclabs.com/wiki/ArduinoFHT

The example I started off with was:

https://github.com/TJC/arduino/blob/master/fhttest/fhttest.cpp


Note: If you are using a microphone powered by the 3.3V signal, such as the Sparkfun MEMS microphone, then connect 3.3V to the AREF pin.
Also make sure you uncomment analogReference(EXTERNAL);  in Setup().


Test with: http://onlinetonegenerator.com/frequency-sweep-generator.html

*/


#define qsubd(x, b) ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.

#define wavebright 128                                        // qsubd result will be this value if subtraction is >0.

#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for APA102 or WS2801
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (APA102, WS2801 or WS2812B)
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


#define LOG_OUT 1

#define FHT_N 256                                             // Set to 256 point fht.
#define inputPin A5

#include <FHT.h>                                              // FHT library


uint8_t hueinc;                                               // A hue increment value to make it rotate a bit.
uint8_t micmult = 10;

uint8_t noiseval = 32;                                        // Increase this to reduce sensitivity.

void setup() {
  analogReference(EXTERNAL);                                  // Connect 3.3V to AREF pin for any microphones using 3.3V
  Serial.begin(57600);                                        // use the serial port
  
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
}


void loop() {
  EVERY_N_MILLISECONDS(20) {
    fhtsound();
  }
  show_at_max_brightness_for_power();

//  Serial.println(LEDS.getFPS());                              // Display frames per second on the serial monitor.
}  


void fhtsound() {
  hueinc++;                                                   // A cute little hue incrementer.
  GetFHT();                                                   // Let's take FHT_N samples and crunch 'em.

  for (int i= 0; i < NUM_LEDS; i++) {                         // Run through the LED array.
    int tmp = qsuba(fht_log_out[2*i+2], noiseval);           // Get the sample and subtract the 'quiet' normalized values, but don't go < 0.
    if (tmp > (leds[i].r + leds[i].g + leds[i].b))            // Refresh an LED only when the intensity is low
        leds[i] = CHSV(tmp*micmult+hueinc, 255, tmp*micmult);  // Note how we really cranked up the tmp value to get BRIGHT LED's. Also increment the hue for fun.
    leds[i].nscale8(224);                                     // Let's fade the whole thing over time as well.
  }
} // fhtsound()


void GetFHT() {
  cli();
  for (int i = 0 ; i < FHT_N ; i++) fht_input[i] = analogRead(inputPin) - 512;
  sei();

  fht_window();                                               // Window the data for better frequency response.
  fht_reorder();                                              // Reorder the data before doing the fht.
  fht_run();                                                  // Process the data in the fht.
  fht_mag_log();
} // GetFHT()
