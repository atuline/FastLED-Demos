void rainbow_vertical() {                     // RAINBOW 'UP/DOWN' THE LOOP
  if (thisdir == 0) idex++; else idex--;
  if (idex <0 ) idex = TOP_INDEX;
  if (idex > TOP_INDEX) idex = 0;
  thishue = thishue + thisstep;
  if (thishue > 255) thishue = 0;
  int idexA = idex;
  int idexB = horizontal_index(idexA);
  leds[idexA] = CHSV(thishue, thissat, 255);
  leds[idexB] = CHSV(thishue, thissat, 255);
  show_at_max_brightness_for_power();
  
//  FastLED.delay(thisdelay*2.5);
  delay_at_max_brightness_for_power(thisdelay*2.5);
//  delay(thisdelay);


} // rainbow_vertical()
