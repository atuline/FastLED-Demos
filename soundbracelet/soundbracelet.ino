
/* soundbracelet

By: John Burroughs

Converted to FastLED by: Andrew Tuline

Date: Oct, 2014

soundbracelet is based Neopixel code by John Burroughs:

https://www.youtube.com/watch?v=JjX8X5D8RW0&feature=youtu.be
https://plus.google.com/105445034001275025240/posts/jK2fxRx79kj
http://www.slickstreamer.info/2014/07/led-bracelet-vu-meter-3dprinting.html

That was based on the Adafruit LED Ampli-tie project at:

https://learn.adafruit.com/led-ampli-tie/overview

This version was written for a Sparkfun INMP401 MEMS microphone/pre-amp. In this case, it's plugged into A5 of the Arduino.

You may need to only use battery power as noise from the USB can affect the signals.

Plug Vcc of the microphone into 3.3V of Arduino. Connect 3.3V of Arduino to aref pin, and gnd to gnd.

*/


#include "FastLED.h"                                          // FastLED library.
 

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 20                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


#define MIC_PIN 5                                             // Analog port for microphone

#define DC_OFFSET  32                                         // DC offset in mic signal - if unusure, leave 0
                                                              // I calculated this value by serialprintln lots of mic values
#define NOISE     100                                         // Noise/hum/interference in mic signal and increased value until it went quiet
#define SAMPLES   60                                          // Length of buffer for dynamic level adjustment
#define TOP (NUM_LEDS + 2)                                    // Allow dot to go slightly off scale
#define PEAK_FALL 40                                          // Rate of peak falling dot
 
byte
  peak      = 0,                                              // Used for falling dot
  dotCount  = 0,                                              // Frame counter for delaying dot-falling speed
  volCount  = 0;                                              // Frame counter for storing past volume data
int
  vol[SAMPLES],                                               // Collection of prior volume samples
  lvl       = 10,                                             // Current "dampened" audio level
  minLvlAvg = 0,                                              // For dynamic adjustment of graph low & high
  maxLvlAvg = 512;


void setup() {
  
  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);

  delay(1000);                                                // Power-up safety delay or something like that.  
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);        // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA
} // setup()

 
void loop() {
  soundbracelet();
  show_at_max_brightness_for_power();                         // Power managed FastLED display
} // loop()


void soundbracelet() {

  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int      n, height;
   
  n = analogRead(MIC_PIN);                                    // Raw reading from mic
  n = abs(n - 512 - DC_OFFSET);                               // Center on zero
  
  n = (n <= NOISE) ? 0 : (n - NOISE);                         // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;                                 // "Dampened" reading (else looks twitchy)
 
  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);
 
  if (height < 0L)       height = 0;                          // Clip output
  else if (height > TOP) height = TOP;
  if (height > peak)     peak   = height;                     // Keep 'peak' dot at top
 
 
  // Color pixels based on rainbow gradient
  for (i=0; i<NUM_LEDS; i++) {
    if (i >= height)   leds[i].setRGB( 0, 0,0);
    else leds[i] = CHSV(map(i,0,NUM_LEDS-1,30,150), 255, 255);
  }
 
  // Draw peak dot  
  if (peak > 0 && peak <= NUM_LEDS-1) leds[peak] = CHSV(map(peak,0,NUM_LEDS-1,30,150), 255, 255);

// Every few frames, make the peak pixel drop by 1:
 
    if (++dotCount >= PEAK_FALL) {                            // fall rate 
      if(peak > 0) peak--;
      dotCount = 0;
    }
  
  vol[volCount] = n;                                          // Save sample for dynamic leveling
  if (++volCount >= SAMPLES) volCount = 0;                    // Advance/rollover sample counter
 
  // Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for (i=1; i<SAMPLES; i++) {
    if (vol[i] < minLvl)      minLvl = vol[i];
    else if (vol[i] > maxLvl) maxLvl = vol[i];
  }
  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6;                 // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6;                 // (fake rolling average)

} // fastbracelet()
