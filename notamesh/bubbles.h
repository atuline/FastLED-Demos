#ifndef BUBBLES_H
#define BUBBLES_H

/* 
 *  Adapted from Reko MeriÃ¶. A pretty cool looking routine.
 */

void bubbles() {                                          // Spawns bubbles that move when audio peaks enough

// Local definitions

// Persistent local variables

// Temporary local variables


  if (samplepeak) {
    samplepeak = 0;

    int randomBubble = random(maxBubbles);
    
    if (bubble[randomBubble].exist) {
      bubble[randomBubble].life /= 2;                     // Give that bubble longer life.
      bubble[randomBubble].NewColor();                    // And new color
    } else {
      bubble[randomBubble].exist = true;
      bubble[randomBubble].NewColor();
    }
  }
  
  for (int i = 0; i < maxBubbles; i++) {
    if (bubble[i].exist) {
      leds[bubble[i].pos] = ColorFromPalette(currentPalette, bubble[i].color + millis(), bubble[i].brightness, LINEARBLEND);      
      bubble[i].Move();
    }
  }

  fadeToBlackBy(leds, NUM_LEDS, 160); 

} // bubbles()

#endif

