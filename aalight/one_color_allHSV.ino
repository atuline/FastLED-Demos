
// Leave the line above blank
void one_color_allHSV(int ahue, int abright) {        // SET ALL LEDS TO ONE COLOR (HSV)
  for (int i = 0 ; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(ahue, thissat, abright);
  }
}

