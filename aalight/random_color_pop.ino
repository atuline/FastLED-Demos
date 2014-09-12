
// Leave the line above blank
void random_color_pop() {                                     // RANDOM COLOR POP FADE
  idex = random8();
  thishue = random16(0,255);                                  // For some reason, the second random8() wasn't that random
  if (idex < NUM_LEDS) leds[idex] = CHSV(thishue, 255, 255);  // Only the lowest probability twinkles will do. You could even randomize the hue/saturation. .
  for (int i = 0; i <NUM_LEDS; i++) leds[i].fadeToBlackBy(8); // Go through the array and reduce each RGB value by a percentage.
  show_at_max_brightness_for_power();
  delay(thisdelay);
}


