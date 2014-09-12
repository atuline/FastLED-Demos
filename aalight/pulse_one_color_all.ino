
// Leave the line above blank
void pulse_one_color_all() {                          // PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR

  if (thisbright >= 255) {thisbounce = -1;}
  if (thisbright <= 1) {thisbounce = 1;}
  thisbright += thisbounce;
  for (int idex = 0 ; idex < NUM_LEDS; idex++ ) {
    leds[idex] = CHSV(thishue, thissat, thisbright);
  }
  show_at_max_brightness_for_power();
  delay(thisdelay);

}

