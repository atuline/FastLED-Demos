#ifndef CIRCNOISE_PAL_2_H
#define CIRCNOISE_PAL_2_H

/* This is adapted from a routine created by Stefan Petrick */

/*  Usage - circnoise_pal_2();
 *  
 */


// just moving along one axis = "lavalamp effect"
void circnoise_pal_2() {

  uint8_t scale = 1000;                               // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = millis() / 10;                 // x as a function of time
    uint16_t shift_y = 0;

    uint32_t real_x = (xd[i] + shift_x) * scale;       // calculate the coordinates within the noise field
    uint32_t real_y = (yd[i] + shift_y) * scale;       // based on the precalculated positions

    uint8_t noise = inoise16(real_x, real_y, 4223) >> 8;           // get the noise data and scale it down

    uint8_t index = noise * 3;                        // map led color based on noise data
    uint8_t bri   = noise;

    leds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.

  }
} // circnoise_pal_2_2()


#endif
