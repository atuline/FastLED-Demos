#ifndef PLASMA_H
#define PLASMA_H

/* Usage - plasma(freq1, freq2, phase1, phase2);
 *
 * 
 *
 */

#endif



void plasma(uint8_t freq1, uint8_t freq2, int8_t phase1, int8_t phase2) {                 // This is the heart of this program. Sure is short. . . and fast.

  int thisPhase = beatsin8(phase1,-64,64);                                                // Setting phase change for a couple of waves.
  int thatPhase = beatsin8(phase2,-64,64);

  for (int k=0; k<NUM_LEDS; k++) {                                                        // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int colorIndex = cubicwave8((k*freq1)+thisPhase)/2 + cos8((k*freq2)+thatPhase)/2;     // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish, but don't change on the fly.
    int thisBright = qsuba(colorIndex, beatsin8(7,0,96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..
    leds[k] = ColorFromPalette(CurrentPalette, colorIndex, thisBright, currentBlending);  // Let's now add the foreground colour.
  }

} // plasma()
