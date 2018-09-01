#ifndef PIXEL_H
#define PIXEL_H

void pixel() {

// Persistent local variable
  static uint16_t currLED;                                                      // Persistent local value to count the current LED location.

  timeval = 0;                                                                  // Our EVERY_N_MILLIS_I timer value.
  
  currLED = (currLED+1)	% (NUM_LEDS);                                           // Cycle through all the LED's. By Andrew Tuline.

  CRGB newcolour = ColorFromPalette(currentPalette, oldsample, oldsample, currentBlending);   // Colour of the LED will be based on oldsample, while brightness is based on sampleavg.
  nblend(leds[currLED], newcolour, 192);                                        // Blend the old value and the new value for a gradual transitioning.

} // pixel()

#endif
