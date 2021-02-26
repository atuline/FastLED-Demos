
void addGlitter(uint16_t chanceOfGlitter) {                   // Changed fract8 to uint16_t.
  if(random8() < chanceOfGlitter) leds[ random16(NUM_LEDS) ] += CRGB::White;
} // addGlitter()


void rainbow() {
//   fill_rainbow( leds, NUM_LEDS, gHue, 7);                  // Original routine.
  fadeToBlackBy(leds, NUM_LEDS,32);
  int tmp = (random8()+micIn) % NUM_LEDS;
  leds[tmp] += CHSV(gHue+tmp*7,255,micIn*4);
} // rainbow()


void rainbowWithGlitter() {
  rainbow();                                                  // Built-in FastLED rainbow, plus some random sparkly glitter.
//  addGlitter(80);                                           // Original chance of glitter.
  addGlitter(micIn*8);                                        // Increase our chance of glitter.
} // rainbowWithGlitter()


void confetti() {                                             // Random colored speckles that blink in and fade smoothly.
  fadeToBlackBy(leds, NUM_LEDS, 64);
//  leds[pos] += CHSV( gHue + random8(64), 200, 255);         // Original routine.
  for (int i=0; i<8; i++) leds[random16(NUM_LEDS)] += CHSV(gHue + random8(64), 200, micIn*6);  // Updated routine with slower fps needs more action.
} // confetti()


void sinelon() {                                              // A colored dot sweeping back and forth, with fading trails.
  fadeToBlackBy(leds, NUM_LEDS, 20);
//  int pos = beatsin16(17,0,NUM_LEDS-1);                     // Original routine.
//  leds[pos] += CHSV( gHue, 255, 192);                       // Original routine.
  for (int i=0; i<7; i++) {
    int pos = beatsin16((i+1)*4,0,NUM_LEDS-1);
    leds[pos] += CHSV(gHue, 255, micIn*6);
  }
} // sinelon()


void juggle() {                                               // Eight colored dots, weaving in and out of sync with each other.
  fadeToBlackBy(leds, NUM_LEDS, 20);
//  byte dothue = 0;                                          // Original routine.
  byte dothue = micIn;
  for(int i = 0; i < 8; i++) {
//    leds[beatsin16(i+7,0,NUM_LEDS-1)] |= CHSV(dothue, 200, 255);  // Original routine.
    leds[beatsin16(i+7,0,NUM_LEDS-1)] |= CHSV(dothue, 200, micIn*4);
    dothue += 32;
  }
} // juggle()


void bpm() {                                                  // Colored stripes pulsing at a defined Beats-Per-Minute.
  fadeToBlackBy(leds, NUM_LEDS, 20);
  uint8_t beat = beatsin8(17, 64, 255);
  for(int i = 0; i < NUM_LEDS; i++) leds[i] = ColorFromPalette(palette,gHue+(i*3), beat-gHue+(i*10) );
  if (micIn > 0) leds[random16() % NUM_LEDS] = CRGB::White;
} // bpm()
