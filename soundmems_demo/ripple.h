#ifndef RIPPLE_H
#define RIPPLE_H

void ripple() {                                                                 // Display ripples triggered by peaks.

// Local definitions
  #define maxsteps 16                                                           // Maximum number of steps.

// Persistent local variables.
  static uint8_t colour;                                                        // Ripple colour is based on samples.
  static uint16_t center = 0;                                                   // Center of current ripple.
  static int8_t step = -1;                                                      // Phase of the ripple as it spreads out.

  timeval = 20;                                                                 // Our EVERY_N_MILLIS_I timer value.
  
  if (samplepeak == 1) {step = -1;}                                             // Trigger a new ripple if we have a peak.
  
  fadeToBlackBy(leds, NUM_LEDS, 64);                                            // Fade the strand, where 1 = slow, 255 = fast

  switch (step) {

    case -1:                                                                    // Initialize ripple variables. By Andrew Tuline.
      center = random(NUM_LEDS);
      colour = (oldsample) % 255;                                               // More peaks/s = higher the hue colour.
      step = 0;
      break;

    case 0:
      leds[center] += ColorFromPalette(currentPalette, colour, 255, currentBlending); // Display the first pixel of the ripple.
      step ++;
      break;

    case maxsteps:                                                              // At the end of the ripples.
      // step = -1;
      break;

    default:                                                                    // Middle of the ripples.

      leds[(center + step + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(currentPalette, colour, 255/step*2, currentBlending);  // A spreading and fading pattern up the strand.
      leds[(center - step + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(currentPalette, colour, 255/step*2, currentBlending);  // A spreading and fading pattern down the strand.
      step ++;                                                                  // Next step.
      break;  
      
  } // switch step


  addGlitter(sampleavg);                                                        // Add glitter baesd on sampleavg.
  
} // ripple()

#endif
