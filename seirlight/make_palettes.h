#ifndef MAKE_PALETTES_H
#define MAKE_PALETTES_H


void SetupRandom16Palette() {
  for (int i = 0; i < 16; i++) targetPalette[i] = CHSV(random8(), 255, 255);
} // SetupRandom16Palette()


void SetupRandom4Palette() {
   targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, 255));
} // SetupRandom4Palette()


void SetupSimilar4Palette() {
  thishue=random8();
  targetPalette = CRGBPalette16(CHSV(thishue+random8(32), 255, random8(128,255)),
                              CHSV(thishue+random8(32), 255, random8(192,255)),
                              CHSV(thishue+random8(32), 192, random8(192,255)),
                              CHSV(thishue+random8(32), 255, random8(128,255)));
} // SetupSimilar4Palette()

void SetupMySimilar4Palette() {
  targetPalette = CRGBPalette16(CHSV(thishue+random8(32), 255, random8(128,255)),
                              CHSV(thishue+random8(32), 255, random8(192,255)),
                              CHSV(thishue+random8(32), 192, random8(192,255)),
                              CHSV(thishue+random8(32), 255, random8(128,255)));
} // SetupSimilar4Palette()



#endif
