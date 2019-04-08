#ifndef MYVUMETER_H
#define MYVUMETER_H

void myvumeter() {                                                        // A vu meter. Grabbed the falling LED from Reko MeriÃ¶.

// Local definitions
  #define GRAVITY 2

// Persistent local variables
  static uint8_t topLED;
  static int gravityCounter = 0;

// Temporary local variables
  uint8_t tempsamp = constrain(sampleavg,0,NUM_LEDS-1);                     // Keep the sample from overflowing.


  fadeToBlackBy(leds, NUM_LEDS, 160);
    
  for (int i=0; i<tempsamp; i++) {
    uint8_t index = inoise8(i*sampleavg+millis(), 5000+i*sampleavg); 
    leds[i] = ColorFromPalette(currentPalette, index, sampleavg, currentBlending);
  }

  if (tempsamp >= topLED)
    topLED = tempsamp;
  else if (gravityCounter % GRAVITY == 0)
    topLED--;

  if (topLED > 0) {
    leds[topLED] = ColorFromPalette(currentPalette, millis(), 255, LINEARBLEND);   // LED falls when the volume goes down.
  }
  
  gravityCounter = (gravityCounter + 1) % GRAVITY;
    
} // myvumeter()

#endif

