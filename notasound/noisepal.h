#ifndef NOISEWIDE_H
#define NOISEWIDE_H

void noisewide() {                                                              // Create fire based on noise and sampleavg. 

// Local definitions
  #define xscale 20                                                             // How far apart they are
  #define yscale 3                                                              // How fast they move

// Persistent local variables

// Temporary local variables
  uint16_t index = 0;                                                            // Current colour lookup value.


  for(int i = 0; i < NUM_LEDS; i++) {

    index = inoise8(i*xscale,millis()*yscale*NUM_LEDS/255);                     // X location is constant, but we move along the Y at the rate of millis(). By Andrew Tuline.

    index = (255 - i*256/NUM_LEDS) * index / 128;                                 // Now we need to scale index so that it gets blacker as we get close to one of the ends
    
    leds[NUM_LEDS/2-i/2+1] = ColorFromPalette(currentPalette, index, sampleavg*sampleavg/64, NOBLEND);      // With that value, look up the 8 bit colour palette value and assign it to the current LED. 
    leds[NUM_LEDS/2+i/2-1] = ColorFromPalette(currentPalette, index, sampleavg*sampleavg/64, NOBLEND);      // With that value, look up the 8 bit colour palette value and assign it to the current LED. 
    
  }                                                                             // The higher the value of i => the higher up the palette index (see palette definition).

} // noisewide()

#endif
