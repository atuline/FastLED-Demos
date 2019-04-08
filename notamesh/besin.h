#ifndef BESIN_H
#define BESIN_H

void besin() {                                           // Add a Perlin noise soundbar. This looks really cool.
                                                         // Shows you don't need a crap ton of code for a cool effect.
// Local definitions

// Persistent local variables

// Temporary local variables

  
  leds[NUM_LEDS/2] = ColorFromPalette(currentPalette, millis(), sampleavg, NOBLEND);
  leds[NUM_LEDS/2-1] = ColorFromPalette(currentPalette, millis(), sampleavg, NOBLEND);

  waveit();                                              // Move the pixels to the left/right, but not too fast.

  fadeToBlackBy(leds, NUM_LEDS, 2);                      // Fade the center, while waveit moves everything out to the edges.                                                                       

} // besin()

#endif

