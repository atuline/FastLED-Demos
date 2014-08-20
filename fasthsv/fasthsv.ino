
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

#define LED_DT 13
#define MIC_PIN 5           // Analog port for microphone
#define NUM_LEDS  24        // Number of pixels in strand


struct CRGB leds[NUM_LEDS];


int thisdelay = 25;

void setup() {
  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);
  
  Serial.begin(9600);         // DEBUG

  //LEDS.addLeds<WS2801, LED_CK, LED_DT, BGR, DATA_RATE_MHZ(1)>(leds, NUM_LEDS);
  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, NUM_LEDS);
}
 
void loop() {
  fasthsv();
}

void fasthsv() {
  uint8_t j,k;
  int i;

  for (j = 1; j < 50; j++) {
    k = 0;
    for (i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(k, 255, 255);
      k = k + j;
    }
    LEDS.show();
    delay(thisdelay);
  }

for (j = 49; j >0; j--) {
    k = 0;
    for (i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(k, 255, 255);
      k = k + j;
    }
    LEDS.show();
    delay(thisdelay);
  }
}