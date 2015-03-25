#ifndef CONFETTI_H
#define CONFETTI_H

/* This is adapted from a routine created by Mark Kriegsman */


void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(thishue + random8(64), 200, 255);
  thishue++;
}

#endif
