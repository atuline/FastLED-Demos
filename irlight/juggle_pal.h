#ifndef JUGGLE_PAL_H
#define JUGGLE_PAL_H

/* This is adapted from a routine created by Mark Kriegsman */

/*  Usage - juggle_pal();
 *  
 *  thishue
 *  thisfade
 *  numdots
 *  thisbeat
 *  thisbright
 *  thisdiff
 */

uint8_t numdots;

void juggle_pal() {                                            // Several colored dots, weaving in and out of sync with each other
  thisindex = 0;                                           // Reset the hue values.
  fadeToBlackBy(leds, NUM_LEDS, thisfade);
  for( int i = 0; i < numdots; i++) {
    leds[beatsin16(thisbeat+i+numdots,0,NUM_LEDS)] += ColorFromPalette(currentPalette, thisindex , thisbright, currentBlending);    // Munge the values and pick a colour from the palette
    thisindex += thisdiff;
  }
} // juggle_pal()

#endif
