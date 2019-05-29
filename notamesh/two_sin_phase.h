#ifndef TWO_SIN_PHASE_H
#define TWO_SIN_PHASE_H

/* Usage - two_sin_phase();
 *
 * Loads of variables to play with
 * 
 */


// two_sin variables-------------------------------------------------------------------------




void two_sin_phase() {                                                              // This is the heart of this program. Sure is short.

  thisphase +=thisspeed*thisdir;
  thatphase +=thatspeed*thisdir;

  thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  thathue = thathue + thatrot;                                                // It's also fun for thatwave.
  
  for (int k=0; k<NUM_LEDS; k++) {
    int thisbright = qsuba(cubicwave8((k*allfreq)+thisphase*k/16), thiscutoff);     // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int thatbright = qsuba(cubicwave8((k*allfreq)+128+thatphase*(NUM_LEDS-k)/16), thatcutoff); // This wave is 180 degrees out of phase (with the value of 128).

    leds[k] = ColorFromPalette(CurrentPalette, thishue, thisbright, currentBlending);
    leds[k] += ColorFromPalette(CurrentPalette, thathue, thatbright, currentBlending);
  }
  
} // two_sin_phase()

#endif
