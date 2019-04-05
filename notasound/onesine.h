#ifndef ONESINE_H
#define ONESINE_H

void onesine() {

// Local definitions

// Persistent local variable.
  static int thisphase = 0;                                                     // Phase change value gets calculated.

// Temporary local variables
  uint8_t allfreq = 32;                                                         // You can change the frequency, thus distance between bars. Wouldn't recommend changing on the fly.
  uint8_t thiscutoff = 192;                                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
 

  thiscutoff = 255 - sampleavg;
  
  thisphase += sampleavg/2 + beatsin16(20,-10,10);                              // Move the sine waves along as a function of sound plus a bit of sine wave.

  for (int k=0; k<NUM_LEDS; k++) {                                              // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int thisbright = qsuba(cubicwave8((k*allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = ColorFromPalette( currentPalette, millis()/2, thisbright, currentBlending);    // Let's now add the foreground colour. By Andrew Tuline.
  }

} // onesine()

#endif

