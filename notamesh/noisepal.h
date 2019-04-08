#ifndef NOISEPAL_H
#define NOISEPAL_H

void noisepal() {                                                              // Create fire based on noise and sampleavg. 

// Local definitions
  #define xscale 20                                                             // How far apart they are
  #define yscale 3                                                              // How fast they move

// Persistent local variables

// Temporary local variables
  static uint16_t dist;                                                     // A random number for our noise generator.

  
  for(int i = 0; i < NUM_LEDS; i++) {                                       // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i*xscale, dist+i*yscale);                         // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);    // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  dist += beatsin8(10,1, 4);                                                // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
                                                                            // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
} // noisepal()

#endif

