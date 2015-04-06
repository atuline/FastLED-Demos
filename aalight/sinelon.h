#ifndef SINELON_H
#define SINELON_H

/* This is adapted from a routine created by Mark Kriegsman */

void sinelon() {                                              // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( thishue, 255, 192);
  thishue++;
}

#endif
