#ifndef SERENDIPITOUS_H
#define SERENDIPITOUS_H

/*  This is from Serendipitous Circles from the August 1977 and April 1978 issues of Byte Magazine. I didn't do a very good job of it, but am at least getting some animation and the routine is very short.
*/


/*  Usage - serendipitous_pal();
 *  
 */

uint16_t Xorig = 0x013;
uint16_t Yorig = 0x021;
uint16_t X=Xorig;
uint16_t Y=Yorig;
uint16_t Xn;
uint16_t Yn;


void serendipitous() {

  EVERY_N_SECONDS(5) {
    X = Xorig;
	  Y = Yorig;
  }

  Xn = X-(Y/2); Yn = Y+(Xn/2);
//  Xn = X-Y/2;   Yn = Y+Xn/2;
//  Xn = X-(Y/2); Yn = Y+(X/2.1);
//  Xn = X-(Y/3); Yn = Y+(X/1.5);
//  Xn = X-(2*Y); Yn = Y+(X/1.1);
  
  X = Xn;
  Y = Yn;
  thisindex = (sin8(X)+cos8(Y))/2;
  leds[X%(NUM_LEDS)] = ColorFromPalette(CurrentPalette, thisindex, 255, currentBlending);
  fadeToBlackBy(leds, NUM_LEDS, 16);                     // 8 bit, 1 = slow, 255 = fast
  
} // serendipitous()

#endif
