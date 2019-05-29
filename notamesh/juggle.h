#ifndef JUGGLE_H
#define JUGGLE_H

/* This is adapted from a routine created by Mark Kriegsman */

/*  Usage - juggle();
 *  
 *  thishue
 *  thisfade
 *  numdots
 *  thisbeat
 *  thisbright
 *  thisdiff
 */


void juggle() {                                           // Several colored dots, weaving in and out of sync with each other
  
  thisindex = 0;                                              // Reset the hue values.
  fadeToBlackBy(leds, NUM_LEDS, thisfade);
  for( int i = 0; i < numdots; i++) {
    leds[beatsin16(thisbeat+i+numdots,0,NUM_LEDS-1)] += ColorFromPalette(CurrentPalette, thisindex , thisbright, currentBlending);    // Munge the values and pick a colour from the palette
    thisindex += thisdiff;
  }
  
} // juggle()

#endif
