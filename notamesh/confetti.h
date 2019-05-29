#ifndef CONFETTI_H
#define CONFETTI_H

/* This is adapted from the confetti routine created by Mark Kriegsman */

/*  Usage - confetti_pal();
 *   
 *  thisfade
 *  thisdelay
 *  currentPalette and targetPalette
 *  thisdiff
 *  thisindex
 *  thisinc
 *  thisbright
 */


void confetti() {                                                                                               // random colored speckles that blink in and fade smoothly

  fadeToBlackBy(leds, NUM_LEDS, thisfade);                                                                      
  int pos = random8(NUM_LEDS);                                                                                      // Pick an LED at random.
  leds[pos] = ColorFromPalette(CurrentPalette, thisindex + random8(thisdiff)/4 , thisbright, currentBlending);     // Munge the values and pick a colour from the palette
  thisindex = thisindex + thisinc;                                                                                  // base palette counter increments here.
  
} // confetti()

#endif
