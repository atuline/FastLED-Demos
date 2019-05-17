/*
 * Ripples
 * 
 * Originally by: Reko Meriö
 * 
 * Highly modified by: Andrew Tuline
 * 
 * Date: February, 2019
 * 
 * A ripple routine that supports simultaneous ripples via a structure.
 * 
 * 
 */



#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif


// Fixed definitions cannot change on the fly.
#define LED_DT 12                                                               // Serial data pin for all strands
#define LED_CK 11                                                               // Serial clock pin for WS2801 or APA102
#define COLOR_ORDER BGR                                                         // It's GRB for WS2812
#define LED_TYPE APA102                                                         // Alternatively WS2801, or WS2812
#define NUM_LEDS 40                                                             // Maximum number of LED's defined (at compile time).

uint8_t max_bright = 255;                                                       // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                                     // Initialize our LED array. We'll be using less in operation.

CRGBPalette16 currentPalette = OceanColors_p;                                   // Use palettes instead of direct CHSV or CRGB assignments
CRGBPalette16 targetPalette = OceanColors_p;                                    // Also support smooth palette transitioning

#define maxRipples 6                                                            // Min is 2 and value has to be divisible by two because each ripple has a left and right component. This cuts down on bouncing code.


//----------------- Ripple structure definition ----------------------------------------------------------------

struct ripple {                                                                 // Reko Meriö's structures

// Local definitions

// Persistent local variables

// Temporary local variables
  uint8_t brightness;                                   // 0 to 255
  int8_t color;                                         // 0 to 255
  int16_t pos;                                           // -1 to NUM_LEDS  (up to 127 LED's)
  int8_t velocity;                                      // 1 or -1
  uint8_t life;                                         // 0 to 20
  uint8_t maxLife;                                      // 10. If it's too long, it just goes on and on. . .
  uint8_t fade;                                         // 192
  bool exist;                                           // 0 to 1

  
  void Move() {
    
    pos += velocity;
    life++;
    
    if (pos > NUM_LEDS - 1) {                           // Bounce back from far end.
      velocity *= -1;
      pos = NUM_LEDS - 1;
    }
    
    if (pos < 0) {                                      // Bounce back from 0.
      velocity *= -1;
      pos = 0;
    }

    brightness = scale8(brightness, fade);              // Adjust brightness accordingly to strip length

    if (life > maxLife) exist = false;                  // Kill it once it's expired.
    
  } // Move()


  
  void Init(uint8_t Fade, uint8_t MaxLife) {                 // Typically 216, 20
    
    pos = random8(NUM_LEDS/8, NUM_LEDS-NUM_LEDS/8);     // Avoid spawning too close to edge.
    velocity = 1;                                       // -1 or 1
    life = 0;                                           // 0 to Maxlife
    maxLife = MaxLife;                                  // 10 called
    exist = true;                                       // 0, 1
    brightness = 255;                                   // 0 to 255
    color = millis();                                   // hue;
    fade = Fade;                                        // 192 called
    
  } // Init()
  
}; // struct ripple

typedef struct ripple Ripple;

Ripple ripples[maxRipples];



void setup() {

  Serial.begin(115200);                                                            // Setup serial baud rate
  delay(1000);                                                                    // Slow startup so we can re-upload in the case of errors.
  LEDS.setBrightness(max_bright);                                                 // Set the generic maximum brightness value.
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER >(leds, NUM_LEDS);           // APA102 or WS2801 definition
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER >(leds, NUM_LEDS);                 // WS2812 definition
  
  set_max_power_in_volts_and_milliamps(5, 1000);                                  // 5V, 1A

} // setup()



void loop() {

  EVERY_N_MILLISECONDS(50) {                                                      // Smooth palette transitioning runs continuously.
    uint8_t maxChanges = 24; 
      nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  
  }

  EVERY_N_SECONDS(5) {
    SetupMySimilar4Palette();                                                     // Change palette colours every 5 seconds.
  }

  EVERY_N_MILLIS(50) {                                                            // Sets the original delay time.
    rippless();                                                                   // Run the ripple routine.
  }

  FastLED.show();                                                                 // Power managed display of LED's.
  
} // loop()



void rippless() {

  for (int i = 0; i < maxRipples; i += 2) {                                       // Check to see if ripple has expired, and if so, create a new one.
    if (random8() > 224 && !ripples[i].exist) {                                   // Randomly create a new ripple if this one has expired.
      ripples[i].Init(192, 10);                                                   // Initialize the ripple array with Fade and MaxLife values.
      ripples[i + 1] = ripples[i];                                                // Everything except velocity is the same for the ripple's other half. Position starts out the same for both halves.
      ripples[i + 1].velocity *= -1;                                              // We want the other half of the ripple to go opposite direction.
    }
  }
  
  for (int i = 0; i < maxRipples; i++) {                                          // Move the ripple if it exists
    if (ripples[i].exist) {
      leds[ripples[i].pos] = ColorFromPalette(currentPalette, ripples[i].color, ripples[i].brightness, LINEARBLEND);
      ripples[i].Move();
    }
  }

  fadeToBlackBy(leds, NUM_LEDS, 160);
  
} // rippless()



void SetupMySimilar4Palette() {                                                   // Create a palette with colours close to each other.

  uint8_t thishue = random8();
  
  targetPalette = CRGBPalette16(CHSV(thishue+random8(32), 255, random8(128,255)),
                                CHSV(thishue+random8(32), 255, random8(192,255)),
                                CHSV(thishue+random8(32), 192, random8(192,255)),
                                CHSV(thishue+random8(32), 255, random8(128,255)));
                                
} // SetupMySimilar4Palette()
