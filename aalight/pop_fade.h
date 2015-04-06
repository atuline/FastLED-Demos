#ifndef POP_FADE_H
#define POP_FADE_H

void pop_fade() {
  unsigned long thiscolour;
  if (ranamount >NUM_LEDS) ranamount = NUM_LEDS;              // Make sure we're at least utilizing ALL the LED's.
  int idex = random16(0, ranamount);

  if (idex < NUM_LEDS) {                                      // Only the lowest probability twinkles will do.
    boolcolours ? thiscolour = random(0, 0xffffff) : thiscolour =  colours[random16(0, numcolours)];     
    int barlen = random16(1,maxbar);
    for (int i = 0; i <barlen; i++) 
      if (idex+i < NUM_LEDS) leds[idex+i] = thiscolour;       // Make sure we don't overshoot the array.
  }
     nscale8(leds,NUM_LEDS,fadeval);                          // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);
}

#endif
