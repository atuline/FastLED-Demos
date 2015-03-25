#ifndef RAINBOW_MARCH_H
#define RAINBOW_MARCH_H

void rainbow_march() {                                           // The fill_rainbow call doesn't support brightness levels
  if (thisdir == 0) thishue += thisrot; else thishue-= thisrot;  // I could use signed math, but 'thisdir' works with other routines.
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);               // I don't change deltahue on the fly as it's too fast near the end of the strip.
}

#endif
