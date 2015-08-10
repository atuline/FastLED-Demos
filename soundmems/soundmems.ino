
/* soundmems

By: Andrew Tuline

Date: Oct, 2014

Basic code to read from the Sparkfun INMP401 microphone and display CHSV output.

This uses some nice debug output with a printf statement.

*/

#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B and BGR for APA102
#define LED_TYPE APA102                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

#define MIC_PIN    5                                          // Analog port for microphone
#define DC_OFFSET  0                                          // DC offset in mic signal - if unusure, leave 0
 

// Initialize global variables for sequences
int wavebright = 10;

// Function that printf and related will use to print
int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return !Serial.write(c);
}
FILE serial_stdout;



void setup() {
  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);
  
  Serial.begin(57600);
   // Set up stdout
    fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);   // Used for printf based debugging.
    stdout = &serial_stdout;

  //  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);
} // setup()


void loop() {
  soundmems();
  show_at_max_brightness_for_power();
} // loop()


void soundmems() {
  int p, q, r;

  for (int i = 0; i<NUM_LEDS; i++) {
    p = analogRead(MIC_PIN);                                  // Raw reading from mic
    q = abs(p - 512 - DC_OFFSET);                             // Center on zero
    r = qsuba(q, wavebright);                                 // Get rid of noise
    printf("%6d  %6d  %6d\n", p, q, r);                       // Nice debug output.
    
    leds[i] = CHSV((r*4 % 255), 255, (r*2)% 255);             // Amplify it and use it as hue and brightness.
  }
} // soundmems()
