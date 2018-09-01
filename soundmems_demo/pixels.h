#ifndef PIXELS_H
#define PIXELS_H

void pixels() {

// Persistent local variable
  static uint16_t currLED;                                                      // Persistent local value to count the current LED location.

  timeval = 50;                                                                  // Our EVERY_N_MILLIS_I timer value.
  
  currLED = beatsin8(16,0,10);

  for (int i=0; i<NUM_LEDS; i++) {

    CRGB newcolour = ColorFromPalette(currentPalette, oldsample+i*8, sampleavg, currentBlending);   // Colour of the LED will be based on oldsample, while brightness is based on sampleavg.
    nblend(leds[(i+currLED)%NUM_LEDS], newcolour, 192);                                             // Blend the old value and the new value for a gradual transitioning.
  
}

} // pixels()

#endif
