#ifndef TWINKLE_H
#define TWINKLE_H

void twinkle() {
  if (twinkrate < NUM_LEDS) twinkrate = NUM_LEDS;                 // Make sure the twinkrate will cover ALL of the LED's as it's used as the maximum LED index value.
  int i = random16(twinkrate);                                    // A random number based on twinkrate. Higher number => fewer twinkles.
  if (i < NUM_LEDS) leds[i] = CHSV(thishue, thissat, thisbright); // Only the lowest probability twinkles will do. You could even randomize the hue/saturation.
     nscale8(leds,NUM_LEDS,fadeval);                              // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);
} // twinkle()

#endif