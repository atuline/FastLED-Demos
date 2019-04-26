/* soundmems_pal
 *
 * By: Andrew Tuline
 *
 * Date: June 2015
 * 
 * Code to read from the Sparkfun INMP401 microphone on pin A5 and display a palette based output.
 *
 */

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 60                                           // Number of LED's.

#define MIC_PIN 5                                             // Analog port for microphone
#define DC_OFFSET  0                                          // Not used at this point. DC offset in mic signal - if unusure, leave 0


// Initialize changeable global variables.
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

// Initialize global variables for sequences
int thisdelay = 20;                                           // A delay value for the sequence(s)



void setup() {

// This is only needed on 5V Arduinos (Uno, Leonardo, etc.). Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
 // line.  Audio samples are 'cleaner' at 3.3V. COMMENT OUT THE NEXT LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
  
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

  currentBlending = LINEARBLEND;

} // setup()



void loop() {

  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    
    for (int i = 0; i < 16; i++) {
      targetPalette[i] = CHSV(random8(), 255, 255);
    }
  }
  
  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }
  
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    soundmems();
    FastLED.show();
  }
} // loop()



void soundmems() {

  int n;
  n = analogRead(MIC_PIN);                                    // Raw reading from mic
  n = qsuba(abs(n-512), 10);                                  // Center on zero and get rid of low level noise
  CRGB newcolour = ColorFromPalette(currentPalette, constrain(n,0,255), constrain(n,0,255), currentBlending);
  nblend(leds[0], newcolour, 128);
  
  for (int i = NUM_LEDS-1; i>0; i--) {
    leds[i] = leds[i-1];
  }
  
} // soundmems()

