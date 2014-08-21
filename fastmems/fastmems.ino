
#include <FastLED.h>        // FastLED library

/* The fastbracelet is based Neopixel code by John Burroughs:
 #
 # - https://www.youtube.com/watch?v=JjX8X5D8RW0&feature=youtu.be
 # - https://plus.google.com/105445034001275025240/posts/jK2fxRx79kj
 # - http://www.slickstreamer.info/2014/07/led-bracelet-vu-meter-3dprinting.html
 #
 # That was based on the Adafruit LED Ampli-tie project at:
 #
 # - https://learn.adafruit.com/led-ampli-tie/overview
 #
*/

#include <FastLED.h>                                           // FastLED library
 
#define LED_DT 13                                              // Data pin
#define NUM_LEDS 24                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  196                                        // How bright do we want to go


#define MIC_PIN 5                                              // Analog port for microphone

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array


// Initialize global variables for sequences
int thisdelay;                                                 // A delay value for the sequence(s)

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

  // Initializing code/variables for the sequence
  thisdelay = 4;
}
 
void loop() {
  memsmic();
}

void memsmic() {
  int n;
  for (int i = 0; i<NUM_LEDS; i++) {
    n = analogRead(MIC_PIN);                                   // Raw reading from mic
    n = abs(n - 512 - DC_OFFSET);                              // Center on zero
    leds[i] = CHSV((n*2 % 255), 255, (n*2)% 255);
    delay(thisdelay);
    LEDS.show();
  }
}