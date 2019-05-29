#ifndef THREE_SIN_H
#define THREE_SIN_H



// three_sin variables ---------------------------------------------------------------------

int wave1;
int wave2;
int wave3;
uint8_t mul1;
uint8_t mul2;
uint8_t mul3;


void three_sin() {
  
  wave1 += beatsin8(10,-4,4)*thisdir;
  wave2 += beatsin8(15,-2,2)*thisdir;
  wave3 += beatsin8(12,-3, 3)*thisdir;

  for (int k=0; k<NUM_LEDS; k++) {
    uint8_t tmp = sin8(mul1*k + wave1) + sin8(mul1*k + wave2) + sin8(mul1*k + wave3);
    leds[k] = ColorFromPalette(CurrentPalette, tmp, 255, currentBlending);
  }
  
} // three_sin()


#endif
