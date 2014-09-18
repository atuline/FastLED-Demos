
// Leave the line above blank
void ems_lightsONE() {                                // EMERGENCY LIGHTS (TWO COLOR SINGLE LED)

  if (thisdir == 0) idex++; else idex--;
  if (idex >= NUM_LEDS) idex = 0;
  if (idex < 0) idex = NUM_LEDS - 1;
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue = (thishue + 160) % 255;

  for (int i = NUM_LEDS-1; i >=0; i-- ) {
    if (i == idexR) {leds[i] = CHSV(thishue, thissat, 255);}
    else if (i == idexB) {leds[i] = CHSV(thathue, thissat, 255);}
//    else {leds[i] = CHSV(0, 0, 0);}
      else {leds[i].fadeToBlackBy(128);}
  }
  show_at_max_brightness_for_power();

//  FastLED.delay(thisdelay*2.5);
  delay_at_max_brightness_for_power(thisdelay*2.5);
//  delay(thisdelay);

} // ems_lightsONE()
