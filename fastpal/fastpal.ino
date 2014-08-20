 #include <FastLED.h>
#define LED_PIN  13
#define NUM_LEDS 24
#define COLOR_ORDER BRG
#define CHIPSET WS2811
#define BRIGHTNESS 128
CRGB leds[NUM_LEDS];

byte startIndex = 0;
CRGBPalette16 currentPalette;
TBlendType  currentBlending;
extern CRGBPalette16 myPalette;
extern const TProgmemPalette16 myPalette_p PROGMEM;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  LEDS.setDither(1);        // Enable or disable. Up to you.
}

void loop() {
  startIndex +=1;
  currentPalette = myPalette_p;
  currentBlending = BLEND;
  PaletteColors(startIndex);
  LEDS.show();
}

void PaletteColors(uint8_t colorIndex){  
    for (int i = 0; i <NUM_LEDS/2; i++) {
    leds[NUM_LEDS/2-i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    leds[NUM_LEDS/2+i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    colorIndex -=4;
  }
  delay(20);
}

const TProgmemPalette16 myPalette_p PROGMEM = {
  CRGB::AliceBlue,
  CRGB::BlueViolet,
  CRGB::DarkBlue,
  CRGB::DarkOrchid,
  
  CRGB::Black,
  CRGB::Black,
  CRGB::Chocolate,
  CRGB::Brown,
  
  CRGB::LawnGreen,
  CRGB::Gold,
  CRGB::Honeydew,
  CRGB::HotPink,
  
  CRGB::Indigo,
  CRGB::IndianRed,
  CRGB::LightCoral,
  CRGB::DodgerBlue
};