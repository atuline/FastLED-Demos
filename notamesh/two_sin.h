#ifndef TWO_SIN_H
#define TWO_SIN_H

/* Usage - two_sin();
 *
 * Loads of variables to play with
 * 
 */


// two_sin variables-------------------------------------------------------------------------

uint8_t thathue = 140;                                        // You can change the starting hue for other wave.
uint8_t thatrot = 0;                                          // You can change how quickly the hue rotates for the other wave. Currently 0.
int8_t thatspeed = 4;                                         // You can change the speed, and use negative values.
int thatphase = 0;                                            // Phase change value gets calculated.
uint8_t thatcutoff = 192;                                     // You can change the cutoff value to display that wave. Lower value = longer wave.


void two_sin() {                                                              // This is the heart of this program. Sure is short.

  thisphase +=thisspeed*thisdir;
  thatphase +=thatspeed*thisdir;

  thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  thathue = thathue + thatrot;                                                // It's also fun for thatwave.
  
  for (int k=0; k<NUM_LEDS; k++) {
    int thisbright = qsuba(cubicwave8((k*allfreq)+thisphase), thiscutoff);     // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int thatbright = qsuba(cubicwave8((k*allfreq)+128+thatphase), thatcutoff); // This wave is 180 degrees out of phase (with the value of 128).

    leds[k] = ColorFromPalette(CurrentPalette, thishue, thisbright, currentBlending);
    leds[k] += ColorFromPalette(CurrentPalette, thathue, thatbright, currentBlending);
  }
  
} // two_sin()

#endif
