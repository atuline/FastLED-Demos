
// Leave the line above blank
void one_color_all(int cred, int cgrn, int cblu) {      // SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < NUM_LEDS; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}
