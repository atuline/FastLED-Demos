#include <FastLED.h>
#define LED_PIN  13
#define NUM_LEDS 24
//#define COLOR_ORDER RBG
#define CHIPSET WS2811
#define BRIGHTNESS 128
CRGB leds[NUM_LEDS];


uint32_t x,v_time,hue_time;
int hxy = 12630;
uint8_t octaves=3;
uint8_t hue_octaves=2;
int xscale=30;
int hue_scale=8;
int hue_speed=5;
int x_speed=25;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop() {
  delay(40);
  mynoise();
  LEDS.show();
}

void mynoise() {
  fill_noise8(leds, NUM_LEDS, octaves, x, xscale, hue_octaves, hxy, hue_scale, hue_time);
  x += x_speed;
  hue_time += hue_speed;
}
