#ifndef MATRIX_SAW_H
#define MATRIX_SAW_H


/* Usage - matrix_saw();
 *  
 *  A simpler version of the matrix.
 *  
 *  
 * thisrot
 * thisindex
 * bgclr
 * bgbri
 * thisdir
 * thisbright
 */


void matrix_saw() {                                           // A simplified version of The Matrix

  static uint8_t thisrate[] = {2, 4, 6, 8, 10};               // Speed of each dot.

  thisbright = 255;

  fadeToBlackBy(leds, NUM_LEDS, thisfade);
  
  for (int i=0;i<numdots;i++) {
    leds[abs(255+thisdir*beat8(i+2))%NUM_LEDS] = ColorFromPalette(CurrentPalette, thisrate[i]*thisscale, thisbright, LINEARBLEND);           // 255 is used to make sure we don't go into negative numbers when subtracting by going in the other direction.
  }

} // matrix_saw()

#endif
