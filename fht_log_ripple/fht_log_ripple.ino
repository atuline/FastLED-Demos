
/* fht_log_ripple

By: Andrew Tuline

Date: Aug, 2015

Disclaimer: Apparently, the FHT library (as of this writing) doesn't work with the Teensy.

Fast Hartley Transform with beat detection for FastLED using ripples.

Faster the beat, the higher the foreground hue colour.

FHT is available at http://wiki.openmusiclabs.com/wiki/ArduinoFHT

This is a work in progress.

My hardware setup:

Arduino Nano & Addressable LED strips
- Powered by USB power bank 
- APA102 or WS2812 data connected to pin 12.
- APA102 clock connected to pin 11.
- 5V on APA102 or WS2812 connected to 5V on Nano (good for short strips).
- Gnd to Gnd on Nano.


Sparkfun MEMS microphone
- Vcc on microphone is connected to 3.3V on Nano.
- AREF on Nano connected to 3.3V on Nano.
- Mic out connected to A5.
- Gnd to Gnd on Nano.

10K linear potentiometer
- One end connected to 5V on Nano.
- One end connected to Gnd on Nano.
- Middle connected to A4 on Nano.




Note: If you are using a microphone powered by the 3.3V signal, such as the Sparkfun MEMS microphone, then connect 3.3V to the AREF pin.

*/


#define qsubd(x, b) ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.

#define wavebright 128                                        // qsubd result will be this value if subtraction is >0.

#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin, if using 4 pin strips.
#define COLOR_ORDER BGR                                       // Are they RGB, GRB or what??
#define LED_TYPE APA102                                       // What kind of strip are you using?
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


#define LOG_OUT 1

#define FHT_N 256                                             // Set to 256 point fht.
#define inputPin A5
#define potPin A4

#include <FHT.h>                                              // FHT library

uint8_t hueinc;                                               // A hue increment value to make it rotate a bit.
uint8_t micmult = 10;                                         // Make it louder and easier to see.
uint8_t noiseval = 32;                                        // Increase this to reduce sensitivity.


// Ripple variables
uint8_t colour;                                               // Ripple colour is randomized.
int center = 0;                                               // Center of the current ripple.
int step = -1;                                                // -1 is the initializing step.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps 16                                           // Case statement wouldn't allow a variable.
int volmax = 0;
uint8_t volcount = 0;

uint8_t bgcol = 0;                                            // Background colour rotates.
int thisdelay = 100;                                           // Standard delay value.



void setup() {
  analogReference(EXTERNAL);                                  // Connect 3.3V to AREF pin for any microphones using 3.3V
  Serial.begin(57600);                                        // use the serial port
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812B LED_TYPE
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102 LED_TYPE

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // This is used by the power management functionality and is currently set at 5V, 500mA.

} // setup()



void loop() {
  noiseval = map(analogRead(potPin), 0, 1023, 16, 48);          // Adjust sensitivity of cutoff.

  EVERY_N_MILLISECONDS(1000) {
    volmax = volcount;
    volcount = 0;
  }

  GetFHT();
  GetBeat();
  ripple();

  show_at_max_brightness_for_power();                           // Run the FastLED.show() at full loop speed.
//  Serial.println(LEDS.getFPS());                              // Display frames per second on the serial monitor.  

} // loop()



void GetBeat() {

  bool beattime = 0;
  static uint8_t fhtold = 0;
  static uint8_t fhtoold = 0;
  int tmpfht = fht_log_out[4];

  if ((tmpfht+fhtold+fhtoold)/3 > noiseval && beattime == 0) {volcount++; beattime = 1; step = -1; /* START BEAT HERE */}
  if ((tmpfht+fhtold+fhtoold)/3 < noiseval) beattime = 0;
  fhtold = tmpfht;
  fhtoold = fhtold;

} // GetBeat()



void GetFHT() {
  for (int i = 0 ; i < FHT_N ; i++) fht_input[i] = analogRead(inputPin) - 512;

  fht_window();                                               // Window the data for better frequency response.
  fht_reorder();                                              // Reorder the data before doing the fht.
  fht_run();                                                  // Process the data in the fht.
  fht_mag_log();                                              // Let's use logarithmic bins.
} // GetFHT()



void ripple() {
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(bgcol, 255, 23);  // Set the background colour.

  switch (step) {

    case -1:                                                          // Initialize ripple variables.
      center = random(NUM_LEDS);
      colour = volmax*10;                                             // More beat/s = higher the hue colour.
      step = 0;
      bgcol = bgcol+8;
      break;

    case 0:
      leds[center] = CHSV(colour, 255, 255);                          // Display the first pixel of the ripple.
      step ++;
      break;

    case maxsteps:                                                    // At the end of the ripples.
      // step = -1;
      break;

    default:                                                             // Middle of the ripples.
      leds[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade/step*2);       // Simple wrap from Marc Miller
      leds[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade/step*2);
      step ++;                                                         // Next step.
      break;  
  } // switch step
} // ripple()
