
/* soundmems_wave
 *
 * By: Andrew Tuline
 * 
 * Wave adapted from: Stefan Petrick
 *
 * Date: February, 2017
 *
 * Basic code to read from the Sparkfun INMP401 microphone, and create waves based on sampled input. This does NOT include sensitivity adjustment.
 * 
 * My hardware setup:
 * 
 * Arduino Nano & Addressable LED strips
 *  - Powered by USB power bank 
 *  - APA102 or WS2812 data connected to pin 12.
 *  - APA102 clock connected to pin 11.
 *  - 5V on APA102 or WS2812 connected to 5V on Nano (good for short strips).
 *  - Gnd to Gnd on Nano.
 * 
 * 
 * Sparkfun MEMS microphone
 *  - Vcc on microphone is connected to 3.3V on Nano.
 *  - AREF on Nano connected to 3.3V on Nano.
 *  - Mic out connected to A5.
 *  - Gnd to Gnd on Nano.
 * 
 * Note: If you are using a microphone powered by the 3.3V signal, such as the Sparkfun MEMS microphone, then connect 3.3V to the AREF pin.
 * 
 */

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

// Microphone definitions
#define MIC_PIN    5                                          // Analog port for microphone
#define DC_OFFSET  0                                          // DC offset in mic signal - if unusure, leave 0

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

int max_bright = 255;

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

unsigned int sample = 0;                                      // Our sampled sound. Real basic like.


void setup() {
  analogReference(EXTERNAL);                                  // 3.3V reference for analog input.
 
  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  currentPalette = OceanColors_p;                             // Initial palette.
  currentBlending = LINEARBLEND; 

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

} // setup()



void loop() {


  EVERY_N_SECONDS(5) {                                        // Change the palette every 5 seconds.
    for (int i = 0; i < 16; i++) {
      targetPalette[i] = CHSV(random8(), 255, 255);
    }
  }
  
  EVERY_N_MILLISECONDS(100) {                                 // AWESOME palette blending capability once they do change.
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }


  EVERY_N_MILLIS_I(thistimer,20) {                            // For fun, let's make the animation have a variable rate.
    uint8_t timeval = beatsin8(10,20,50);                     // Use a sinewave for the line below. Could also use peak/beat detection.
    thistimer.setPeriod(timeval);                             // Allows you to change how often this routine runs.
    fadeToBlackBy(leds, NUM_LEDS, 16);                        // 1 = slow, 255 = fast fade. Depending on the faderate, the LED's further away will fade out.
    sndwave();
    soundmems();
  }

  FastLED.show();

} // loop()



void soundmems() {                                            // Quick and dirty sampling of the microphone.
  
  int tmp = analogRead(MIC_PIN) - 512 - DC_OFFSET;
  sample = abs(tmp);
  
}  // soundmems()



void sndwave() {
  
  leds[NUM_LEDS/2] = ColorFromPalette(currentPalette, sample, sample*2, currentBlending); // Put the sample into the center
  
  for (int i = NUM_LEDS - 1; i > NUM_LEDS/2; i--) {       //move to the left      // Copy to the left, and let the fade do the rest.
    leds[i] = leds[i - 1];
  }

  for (int i = 0; i < NUM_LEDS/2; i++) {                  // move to the right    // Copy to the right, and let the fade to the rest.
    leds[i] = leds[i + 1];
  }
  
} // sndwave()

