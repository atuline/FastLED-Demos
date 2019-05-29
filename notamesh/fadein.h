#ifndef FADEIN_H
#define FADEIN_H

void fadein() {

  random16_set_seed(535);                                                           // The randomizer needs to be re-set each time through the loop in order for the 'random' numbers to be the same each time through.

  for (int i = 0; i<NUM_LEDS; i++) {
    uint8_t fader = sin8(millis()/random8(10,20));                                  // The random number for each 'i' will be the same every time.
    leds[i] = ColorFromPalette(CurrentPalette, i*20, fader, currentBlending);       // Now, let's run it through the palette lookup.
  }

  random16_set_seed(millis());                                                      // Re-randomizing the random number seed for other routines.

} // fadein()


#endif
