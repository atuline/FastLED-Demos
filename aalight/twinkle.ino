
void twinkle() {
  if (twinkrate < NUM_LEDS) twinkrate = NUM_LEDS;             // Make sure the twinkrate will cover ALL of the LED's as it's used as the maximum LED index value.
  int i = random16(twinkrate);                                // A random number based on twinkrate. Higher number => fewer twinkles.
  if (i < NUM_LEDS) leds[i] = CHSV(thishue, thissat, thisbright); // Only the lowest probability twinkles will do. You could even randomize the hue/saturation.
  for (int j = 0; j < NUM_LEDS; j++) leds[j].fadeToBlackBy(thisfade); // Use the FastLED 2.1 fade method.
} // twinkle()

