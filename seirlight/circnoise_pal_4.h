#ifndef CIRCNOISE_PAL_4_H
#define CIRCNOISE_PAL_4_H

/* This is adapted from a routine created by Stefan Petrick */

/*  Usage - circnoise_pal_4();
 *  
 */


// no x/y shifting but scrolling along z
void circnoise_pal_4() {

  uint8_t scale = 1000;                               // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = beatsin8(15);                          // x as a function of time
    uint16_t shift_y = beatsin8(17);                          // y as a function of time
    uint32_t real_z = millis() / 11;                          // z is also a function of time


    uint32_t real_x = (cos8(i)+beatsin8(5,0,32) + shift_x) * scale;             // calculate the coordinates within the noise field
    uint32_t real_y = (sin8(i)+beatsin8(3,0,32) + shift_y) * scale;             // based on the precalculated positions

    
    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;           // get the noise data and scale it down

    uint8_t index = noise * 3;                        // map led color based on noise data
    uint8_t bri   = noise;

    leds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.

  }
} // circnoise_pal_4()


#endif