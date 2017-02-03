#ifndef CONFETTI_PAL_H
#define CONFETTI_PAL_H

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


void confetti_pal() {                                                                                               // random colored speckles that blink in and fade smoothly

//  EVERY_N_SECONDS(5) {
//    SetupSimilar4Palette();
//  }

  fadeToBlackBy(leds, NUM_LEDS, thisfade);                                                                      
  int pos = random8(NUM_LEDS);                                                                                      // Pick an LED at random.
  leds[pos] = ColorFromPalette(currentPalette, thisindex + random8(thisdiff)/4 , thisbright, currentBlending);      // Munge the values and pick a colour from the palette
  thisindex = thisindex + thisinc;                                                                                  // base palette counter increments here.
} // confetti_pal()

#endif
