#ifndef ADDGLITTER_H
#define ADDGLITTER_H


/* Usage - addGlitter(80);        // Higher = more
 *
 * chanceOfGlitter
 */

void addglitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random8(NUM_LEDS-1) ] += CRGB::White;
  }
}

#endif
