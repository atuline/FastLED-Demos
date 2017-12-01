#ifndef JUGGLEP_H
#define JUGGLEP_H

void jugglep() {                                                                // Use the juggle routine, but adjust the timebase based on sampleavg for some randomness.

// Persistent local variables
  static uint8_t curhue;                                                        // Let's rotate the hue.
  static int thistime = 20;                                                     // Time shifted value keeps changing thus interrupting the juggle pattern.

  timeval = 20;                                                                 // Our EVERY_N_MILLIS_I timer value.

  curhue = curhue + 4;                                                          // Change the hue pretty quickly, so that the juggle changes colours on the fly.
  
  fadeToBlackBy(leds, NUM_LEDS, 12);                                            // Fade the strand.
  
  leds[beatsin16(thistime,0,NUM_LEDS-1, 0, 0)] += ColorFromPalette( currentPalette, curhue, sampleavg, currentBlending);
  leds[beatsin16(thistime-3,0,NUM_LEDS-1, 0, 0)] += ColorFromPalette( currentPalette, curhue, sampleavg, currentBlending);
  

  EVERY_N_MILLISECONDS(250) {
    thistime = sampleavg/2;                                                     // Change the beat frequency every 250 ms. By Andrew Tuline.
  }

  addGlitter(sampleavg/2);                                                      // Add glitter based on sampleavg.
  
}

#endif
