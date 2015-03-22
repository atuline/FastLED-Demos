#ifndef MATRIX_H
#define MATRIX_H

void matrix() {                                               // One line matrix

  if (huerot) thishue=thishue+5;
  
  if (random16(90) > 80) {
    if (thisdir == 0) leds[0] = CHSV(thishue, thissat, 255); else leds[NUM_LEDS-1] = CHSV(thishue, thissat, 255);
  }
  else {
    if (thisdir ==0) leds[0] = CHSV(bgclr, thissat, bgbri); else leds[NUM_LEDS-1] = CHSV(bgclr, thissat, bgbri);
  }

  if (thisdir == 0) {
    for (int i = NUM_LEDS-1; i >0 ; i-- ) leds[i] = leds[i-1];
  } else {
    for (int i = 0; i < NUM_LEDS ; i++ ) leds[i] = leds[i+1];
  }
} // matrix()

#endif