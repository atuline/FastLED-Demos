#ifndef MATRIX_H
#define MATRIX_H

void matrix() {                                                  // A 'Matrix' like display using sampleavg for brightness.

// Local definitions

// Persistent local variables

// Temporary local variables


  if (thisdir == 1) {
      leds[0] = ColorFromPalette(currentPalette, millis(), sample, currentBlending); 
  } else {
      leds[NUM_LEDS-1] = ColorFromPalette( currentPalette, millis(), sample, currentBlending);
  }

  if (thisdir == 1) {
    for (int i = NUM_LEDS-1; i >0 ; i-- ) leds[i] = leds[i-1];
  } else {
    for (int i = 0; i < NUM_LEDS-1 ; i++ ) leds[i] = leds[i+1];
  }

} // matrix()

#endif

