#ifndef RAINBOWPEAK_H
#define RAINBOWPEAK_H

void rainbowpeak() {

// Local definitions

// Persistent local variables

// Temporary local variables
  uint8_t beatA = beatsin8(17, 0, 255);                                           // Starting hue.

  
  if (samplepeak) {                                                               // Trigger a rainbow with a peak.
    
    samplepeak = 0;                                                               // Got a peak, now reset it.
    
    fill_rainbow(leds + random8(0,NUM_LEDS/2), random8(0,NUM_LEDS/2), beatA, 8);  // Use FastLED's fill_rainbow routine.
  }
  
  fadeToBlackBy(leds, NUM_LEDS, 40);                                              // Fade everything. By Andrew Tuline.

} // rainbowpeak()

#endif

