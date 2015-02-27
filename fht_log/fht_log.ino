
/* Fast Hartley Transform with FastLED 3.0 or greater

By: Andrew Tuline

Date: Oct, 2014


This is an implementation of the FHT library with FastLED 3.0

FastLED is available at https://github.com/FastLED/FastLED

FHT is available at http://wiki.openmusiclabs.com/wiki/ArduinoFHT


The example I started off with was:

https://github.com/TJC/arduino/blob/master/fhttest/fhttest.cpp


Note: If you are using a microphone powered by the 3.3V signal, such as the Sparkfun MEMS microphone, then connect 3.3V to the AREF pin.

*/


#define qsubd(x, b) ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.

#define wavebright 128                                        // qsubd result will be this value if subtraction is >0.

#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.
 
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
// #define OCT_NORM 1
// #define OCTAVE 1


#define FHT_N 256                                             // Set to 256 point fht.
#define inputPin A5

#include <FHT.h>                                              // FHT library

uint8_t D[NUM_LEDS][12];                                      // Our initial samples used to normalize the output.
uint8_t E[NUM_LEDS];                                          // Our normalized values.

uint8_t hueinc;                                               // A hue increment value to make it rotate a bit.
uint8_t micmult = 10;


void setup() {
  analogReference(EXTERNAL);                                  // Connect 3.3V to AREF pin for any microphones using 3.3V
  Serial.begin(57600);                                        // use the serial port
  
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);

  FastLED.setBrightness(max_bright);

  for (int k = 0; k<12; k++) {                                // Let's get some initial FHT samples.
    GetFHT();                                                 // Get 'em fast
    for (int i=0; i<NUM_LEDS; i++){
      D[i][k] = fht_log_out[2*i+2];                           // Get every 2nd array element starting with the 2nd one.
//      D[i][k] = fht_oct_out[2*i+2];
//      Serial.print(fht_log_out[2*i+2]);
//      Serial.print(fht_oct_out[2*i+2]);
//      Serial.print(", ");
    }
//    Serial.println();
  }

  for (int m = 0; m <NUM_LEDS; m++) {                         // We need to average out those samples to mellow out the noise.
      E[m] = (D[m][0] + D[m][1] + D[m][2] + D[m][3] + D[m][4] + D[m][5] + D[m][6] + D[m][7] + D[m][8] + D[m][9] + D[m][10] + D[m][11])/12*1.75;      // Let's average and 'fudge' the noise limiter.
//      Serial.print(E[m]);
//      Serial.print(", ");
  }
}

void loop() {
  fhtsound();
  FastLED.show();
}  


void fhtsound() {
  hueinc++;                                                   // A cute little hue incrementer.
  GetFHT();                                                   // Let's take FHT_N samples and crunch 'em.
//  for (int i = 0 ; i < FHT_N/2 ; i++)                       // You can process up to FHT_N/2 of them. Hmm, Nyquist.
//    B[i] = fht_log_out[i];

  for (int i= 0; i < NUM_LEDS; i++) {                         // Run through the LED array.
//        Serial.print(fht_oct_out[2*i+2]);
//        Serial.print(", ");
  
//  int tmp = qsuba(fht_oct_out[2*i+2], E[i]);                // Get the sample and subtract the 'quiet' normalized values, but don't go < 0.
  int tmp = qsuba(fht_log_out[2*i+2], E[i]);                  // Get the sample and subtract the 'quiet' normalized values, but don't go < 0.

    if (tmp > (leds[i].r + leds[i].g + leds[i].b))            // Refresh an LED only when the intensity is low
        leds[i] = CHSV(tmp*micmult+hueinc, 255, tmp*micmult);  // Note how we really cranked up the tmp value to get BRIGHT LED's. Also increment the hue for fun.
    leds[i].nscale8(224);                                     // Let's fade the whole thing over time as well.
  }
//          Serial.println();
} // fhtsound()



void GetFHT() {
//  Serial.println("GetFHT");
  cli();
  for (int i = 0 ; i < FHT_N ; i++) {
    fht_input[i] = analogRead(inputPin) - 512;
//    Serial.print(fht_input[i]);
//    Serial.print(", ");
  }
  sei();
//  Serial.println();
    // save 256 samples
  fht_window();                                               // Window the data for better frequency response.
  fht_reorder();                                              // Reorder the data before doing the fht.
  fht_run();                                                  // Process the data in the fht.
//  fht_mag_octave();                                         // Take the output of the fht and put in an array called fht_log_out[FHT_N/2]
  fht_mag_log();
} // GetFHT()
