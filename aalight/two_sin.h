#ifndef TWO_SIN_H
#define TWO_SIN_H

void two_sin() {                                                              // This is the heart of this program. Sure is short.

  if (thisdir == 0) {thisphase += thisspeed; thatphase += thatspeed;} 
  else {thisphase -= thisspeed; thatphase -= thatspeed;}

  thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  thathue = thathue + thatrot;                                                // It's also fun for thatwave.
  
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsuba(cubicwave8((k*allfreq)+thisphase), thiscutoff);     // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int thatbright = qsuba(cubicwave8((k*allfreq)+128+thatphase), thatcutoff); // This wave is 180 degrees out of phase (with the value of 128).

    leds[k] = CHSV(thishue, allsat, thisbright);                              // Assigning hues and brightness to the led array.
    leds[k] += CHSV(thathue, allsat, thatbright);                      
  }
} // two_sin()

#endif
