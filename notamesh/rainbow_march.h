#ifndef RAINBOW_MARCH_H
#define RAINBOW_MARCH_H

/* Usage - rainbow_march();
 *  
 * thisindex
 * thisdiff
 * thisrot
 * thisdir
 */


void rainbow_march() {                                           
  
  thisindex += thisrot*thisdir;

//  fill_rainbow(leds, NUM_LEDS, thisindex, thisdiff);                                                // This doesn't support palettes.

  for (int i=0; i<NUM_LEDS; i++) {                                                                    // This version supports palettes.
    leds[i] = ColorFromPalette(CurrentPalette, thisindex + i*256/NUM_LEDS, 255, currentBlending);
  }

} // rainbow_march()

#endif
