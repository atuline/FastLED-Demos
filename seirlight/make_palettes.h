#ifndef MAKE_PALETTES_H
#define MAKE_PALETTES_H


void SetupRandom16Palette() {
  for (int i = 0; i < 16; i++) targetPalette[i] = CHSV(random8(), random8(192,255), random8(128,255));
} // SetupRandom16Palette()


void SetupRandom4Palette() {
   targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, 255));
} // SetupRandom4Palette()


void SetupSimilar4Palette() {
  thishue=random8();                                                                    // This one picks a random hue for you.
  targetPalette = CRGBPalette16(CHSV(thishue+random8(32), 255, random8(128,255)),
                              CHSV(thishue+random8(32), 255, random8(192,255)),
                              CHSV(thishue+random8(32), 192, random8(192,255)),
                              CHSV(thishue+random8(32), 255, random8(128,255)));
} // SetupSimilar4Palette()

void SetupMySimilar4Palette() {                                                         // This one allows you to select your own hue in advance.
  targetPalette = CRGBPalette16(CHSV(thishue+random8(32), 255, random8(128,255)),
                              CHSV(thishue+random8(32), 255, random8(192,255)),
                              CHSV(thishue+random8(32), 192, random8(192,255)),
                              CHSV(thishue+random8(32), 255, random8(128,255)));
} // SetupMySimilar4Palette()



#endif
