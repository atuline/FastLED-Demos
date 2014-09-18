
// Leave the line above blank
void mynoise() {

  fill_noise16(leds, NUM_LEDS, octaves, x, xscale, hue_octaves, hxy, hue_scale, hue_time);
  if (thisdir == 0) x += x_speed; else x -= x_speed;
  hue_time += hue_speed;
  show_at_max_brightness_for_power();

//  FastLED.delay(thisdelay*2.5);
  delay_at_max_brightness_for_power(1000 / thisdelay);
//  delay(thisdelay);


}
