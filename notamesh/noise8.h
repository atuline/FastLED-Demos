#ifndef NOISE8_H
#define NOISE8_H

/* This is adapted from a routine created by Mark Kriegsman */

/*  Usage - noise8();
 *  
 */

void noise8() {
 
  for(int i = 0; i < NUM_LEDS; i++) {                                      // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i*thisscale, thisdist+i*thisscale) % 255;                  // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(CurrentPalette, index, 255, currentBlending);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  thisdist += beatsin8(10,1,4)*thisdir;                                                // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
                                                                                  // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
} // noise8()

#endif
