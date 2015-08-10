
/* soundmems

By: Andrew Tuline

Date: June 2015

Code to read from the Sparkfun INMP401 microphone on pin A5 and display a palette based output.

*/


#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for WS2812B or WS2801
#define LED_CK 11
#define COLOR_ORDER BGR                                       // Are they RGB, GRB or what??
#define LED_TYPE APA102                                       // What kind of strip are you using?
#define NUM_LEDS 20                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

#define MIC_PIN 5                                             // Analog port for microphone
#define DC_OFFSET  0                                         // DC offset in mic signal - if unusure, leave 0
 
// Palette definitions
CRGBPalette16 currentPalette;
TBlendType    currentBlending;


// Initialize global variables for sequences
int thisdelay = 60;                                            // A delay value for the sequence(s)



// Function that printf and related will use to print
int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return !Serial.write(c);
}

FILE serial_stdout;



void setup() {
  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);
  
  Serial.begin(57600);
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 1000);

  // Also, RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p
  currentPalette = LavaColors_p;
  currentBlending = LINEARBLEND;


  // Set up stdout
  fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &serial_stdout;
} // setup()



void loop() {
  ChangePaletteAndSettingsPeriodically();
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    soundmems();
    show_at_max_brightness_for_power();
  }
} // loop()



void soundmems() {
  int n;
  for (int i = 0; i<NUM_LEDS; i++) {
    n = analogRead(MIC_PIN);                                  // Raw reading from mic
    n = qsuba(abs(n-512), 10);                                // Center on zero and get rid of low level noise
    leds[i] = ColorFromPalette(currentPalette, n*4, n, currentBlending);
  }
} // soundmems()



void ChangePaletteAndSettingsPeriodically()
{
  uint8_t secondHand = ((millis() / 1000) / 1) % 100;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    switch(secondHand)
    {
      case  0: currentPalette = RainbowColors_p; break;
      case 10: SetupRandomPalette(); break;
      case 20: SetupBlackAndWhiteStripedPalette();
      case 30: currentPalette = ForestColors_p; break;
      case 40: currentPalette = CloudColors_p; break;
      case 50: currentPalette = LavaColors_p; break;
      case 60: currentPalette = OceanColors_p; break;
      case 70: currentPalette = PartyColors_p; break;
      case 80: SetupRandomPalette(); break;
      case 90: SetupRandomPalette(); break;
      case 100: break;
    }
  }
} // ChangePaletteAndSettingsPeriodically()


void SetupRandomPalette() {
  currentPalette = CRGBPalette16(CHSV( random8(), 255, 32), CHSV( random8(), 255, 255), CHSV( random8(), 128, 255), CHSV( random8(), 255, 255));
} // SetupRandomPalette()


void SetupBlackAndWhiteStripedPalette() {
  fill_solid(currentPalette, 16, CRGB::Black);               // 'black out' all 16 palette entries...
  currentPalette[0] = CRGB::White;                            // and set every fourth one to white.
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;
} // SetupBlackAndWhiteStripedPalette()
