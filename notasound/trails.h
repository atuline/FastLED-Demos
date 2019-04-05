#ifndef TRAILS_H
#define TRAILS_H

/* 
 *  Adapted from Reko Merio.
 */

void trails() {                                         // Spawns trails that move

// Local definitions

// Persistent local variables

// Temporary local variables


  if (samplepeak) {
    
    samplepeak = 0;
    int randomTrail = random(maxTrails);
    if (trail[randomTrail].exist) {
      trail[randomTrail].life /= 2;                     // Extend life of trail
      trail[randomTrail].NewColor();                    // And give new color
    } else {
      trail[randomTrail].exist = true;
      trail[randomTrail].maxLife = 40;
    }
  }
  
  for (int i = 0; i < maxTrails; i++) {
    if (trail[i].exist) {
      leds[trail[i].pos] = CHSV(trail[i].color, 255, trail[i].brightness);
      trail[i].Move();
    }
  }

  fadeToBlackBy(leds, NUM_LEDS, 100); 

} // trails()

#endif

