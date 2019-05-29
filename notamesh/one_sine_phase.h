#ifndef ONE_SIN_PHASE_H
#define ONE_SIN_PHASE_H

/* 
 * thisspeed
 * thisrot
 * allfreq
 * thiscutoff
*/

void one_sine_phase() {                                                       // This is the heart of this program. Sure is short.
  
  thisphase += thisspeed;                                                                     // You can change direction and speed individually.

  thisindex = millis() / thisrot;
  
  for (int k=0; k<NUM_LEDS-1; k++) {                                                          // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase*k), thiscutoff);                  // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = ColorFromPalette(CurrentPalette, thisindex, thisbright, currentBlending);     // Let's now add the foreground colour.
    thisindex +=256/NUM_LEDS;
  }
 
} // one_sine_phase()

#endif
