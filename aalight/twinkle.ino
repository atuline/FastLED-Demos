
// Leave the line above blank
void twinkle() {
  int i = random8();                                          // A random number. Higher number => fewer twinkles.
  if (i < NUM_LEDS) leds[i] = CHSV(thishue, 100, 255);        // Only the lowest probability twinkles will do. You could even randomize the hue/saturation. .
  for (int j = 0; j <NUM_LEDS; j++) leds[j].fadeToBlackBy(8); // Go through the array and reduce each RGB value by a percentage.
  show_at_max_brightness_for_power();

//  FastLED.delay(thisdelay*2.5);
  delay_at_max_brightness_for_power(thisdelay*2.5);
//  delay(thisdelay);

}

