/* soundmems_noise
 *
 * By: Andrew Tuline
 * 
 * Date: February, 2017
 *
 * Another sound reactive routine that works with Perlin noise.
 * 
 * Again, I'm using the Sparkfun INMP401 microphone.
 *
 */

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B and BGR for APA102
#define LED_TYPE APA102                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 64                                           // Number of LED's.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

CRGBPalette16 currentPalette(OceanColors_p);
CRGBPalette16 targetPalette(LavaColors_p);
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND 


#define MIC_PIN    5                                          // Analog port for microphone
#define DC_OFFSET  0                                          // DC offset in mic signal - if unusure, leave 0

int max_bright = 192;

// Samples
#define NSAMPLES 64
unsigned int samplearray[NSAMPLES];
unsigned int samplesum = 0;
unsigned int sampleavg = 0;
unsigned int samplecount = 0;
unsigned int sample = 0;


static int16_t xdist;                                          // A random number for our noise generator.
static int16_t ydist;
uint16_t xscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint16_t yscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 24;                                      // Value for blending between palettes.



void setup() {

  analogReference(EXTERNAL);                                  // 3.3V reference for analog input.

  for (int i=0; i<NSAMPLES; i++) samplearray[i]=0;
 
  Serial.begin(115200);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);

} // setup()



void loop() {

  EVERY_N_MILLISECONDS(10) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
    fillnoise8();                                             // Update the LED array with noise based on sound input
    fadeToBlackBy(leds, NUM_LEDS, 1);                         // 8 bit, 1 = slow, 255 = fast
    sndwave();                                                // Move the pixels to the left/right.
  }
 
  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
  

  soundmems();                                                // Sample the sound.

  FastLED.show();                                             // Display everything.

} // loop()



void soundmems() {                                            // Rolling average counter - means we don't have to go through an array each time.

  int tmp = analogRead(MIC_PIN) - 512;
  sample = abs(tmp);

  samplesum += sample - samplearray[samplecount];             // Add the new sample and remove the oldest sample in the array 
  sampleavg = samplesum / NSAMPLES;                           // Get an average
  samplearray[samplecount] = sample;                          // Update oldest sample in the array with new sample
  samplecount = (samplecount + 1) % NSAMPLES;                 // Update the counter for the array

}



void fillnoise8() {                                           // Add Perlin noise with modifiers from the soundmems routine.

  if (sampleavg >NUM_LEDS) sampleavg = NUM_LEDS;

  
  for (int i= (NUM_LEDS-sampleavg)/2; i<(NUM_LEDS+sampleavg)/2; i++) {          // The louder the sound, the wider the soundbar.
    
    uint8_t index = inoise8(i*sampleavg+xdist, ydist+i*sampleavg);               // Get a value from the noise function. I'm using both x and y axis.
//      uint8_t index = inoise8(i*xscale, dist+i*yscale) % 255;                 // Get a value from the noise function. I'm using both x and y axis.

 
 
    leds[i] = ColorFromPalette(currentPalette, index, sampleavg, LINEARBLEND);  // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }

  xdist=xdist+beatsin8(5,0,3);
  ydist=ydist+beatsin8(4,0,3);
 
                                                                          
} // fillnoise8()



void sndwave() {                                              // Shifting pixels from the center to the left and right.
  
  for (int i = NUM_LEDS - 1; i > NUM_LEDS/2; i--) {           // Move to the right , and let the fade do the rest.
    leds[i] = leds[i - 1];
  }

  for (int i = 0; i < NUM_LEDS/2; i++) {                      // Move to the left, and let the fade to the rest.
    leds[i] = leds[i + 1];
  }
  
} // sndwave()

