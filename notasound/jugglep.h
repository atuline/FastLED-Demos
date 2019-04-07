#ifndef JUGGLEP_H
#define JUGGLEP_H

void jugglep() {                                                                // Use the juggle routine, but adjust the timebase based on sampleavg for some randomness.

// Local definitions

// Persistent local variables
  static int thistime = 20;                                                     // Time shifted value keeps changing thus interrupting the juggle pattern.

// Temporary local variables


  fadeToBlackBy(leds, NUM_LEDS, 32);                                            // Fade the strand.
  
  leds[beatsin16(thistime,0,NUM_LEDS-1, 0, 0)] += ColorFromPalette( currentPalette, millis()/4, sampleavg*2, currentBlending);
  leds[beatsin16(thistime-3,0,NUM_LEDS-1, 0, 0)] += ColorFromPalette( currentPalette, millis()/4, sampleavg*2, currentBlending);
  
  EVERY_N_MILLISECONDS(250) {
    thistime = sampleavg/2;                                                     // Change the beat frequency every 250 ms. By Andrew Tuline.
  }
 
} // jugglep()

#endif

