
// Leave the line above blank
void rainbow_fade() {                                 // FADE ALL LEDS THROUGH HSV RAINBOW

  thishue++;
  if (thishue > 255) {thishue = 0;}
  for (int idex = 0 ; idex < NUM_LEDS; idex++ ) {
    leds[idex] = CHSV(thishue, thissat, 255);
  }
  show_at_max_brightness_for_power();

//  FastLED.delay(thisdelay*2.5);
  delay_at_max_brightness_for_power(thisdelay*2.5);
//  delay(thisdelay);

}

