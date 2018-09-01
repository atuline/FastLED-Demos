#ifndef NOISEWIDE_H
#define NOISEWIDE_H

void noisewide() {

  timeval = 10;                                                                  // Our EVERY_N_MILLIS_I timer value.
  
  uint8_t tempsamp = constrain(sampleavg/2,0,NUM_LEDS/2);

  memset(leds, 0, NUM_LEDS * 3);                                // Quick clearing of the LED's.
  
  for (int i=0; i<tempsamp; i++) {

    uint8_t index = inoise8(i*sampleavg+millis(), 5000+i*sampleavg); 
//        index = inoise8(i*20,millis()*3*NUM_LEDS/255); 

    leds[NUM_LEDS/2-i] = ColorFromPalette(currentPalette, index, sampleavg, currentBlending);
    leds[NUM_LEDS/2+i] = ColorFromPalette(currentPalette, index, sampleavg, currentBlending);

  }
  
} // noisewide()

#endif
