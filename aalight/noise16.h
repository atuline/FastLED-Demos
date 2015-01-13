#ifndef NOISE16_H
#define NOISE16_H

void noise16() {
  fill_noise16(leds, NUM_LEDS, octaves, x, xscale, hue_octaves, hxy, hue_scale, hue_time);
  if (thisdir == 0) {hxy+=hxyinc; x += x_speed; hue_time+= hue_speed;} else { hxy-=hxyinc; x -= x_speed; hue_time -= hue_speed;}

//  for (int i=0; i<NUM_LEDS; i++) {                            // We can filter, we can invert, we can do all sorts of things.
//    leds[i].r = 0;                                            // Filter out red
//    leds[i].g = 0;                                            // Filter out green
//    leds[i].b = 0;                                            // Filter out blue
//    leds[i].g =qsub(leds[i].g, 40);                           // Set limits
//  }

} // noise16()

#endif