#ifndef ONE_SIN_H
#define ONE_SIN_H

void one_sin() {                                                               // This is the heart of this program. Sure is short.
  if (thisdir == 0) thisphase+=thisspeed; else thisphase-=thisspeed;           // You can change direction and speed individually.
    thishue = thishue + thisrot;                                               // Hue rotation is fun for thiswave.
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);     // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbri);
    leds[k] += CHSV(thishue, allsat, thisbright);                              // Assigning hues and brightness to the led array.
  }
  bgclr++;
} // one_sin()

#endif
