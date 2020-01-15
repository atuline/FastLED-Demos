/* matrix_ray
 *
 * By: Andrew Tuline
 *
 * Date: January, 2018
 * 
 * 
 * This is a mishmash of the Matrix code as well as my one_sine code. The idea was to send out variable pulse width modulated sine wave at various speeds/frequencies, along with palette rotating colours.
 *  
 * It makes good use of a variable EVERY_N_MILLIS function call as well as beatsin8().
 * 
 * As usual, I don't use blocking delays, nested for loops or even floating point. Also, the code is kept as short as possible, variables defined as small and localized as possible.
 * As a result, I get about 450fps on the configuration below.
 * 
 * The advantage of this routine over my one_sine routine is that I can change the frequency on the fly without causing jittering.
 * 
 */


// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for WS2812 or WS2801.
#define LED_CK 11                                             // Serial clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // Are they GRB for WS2812 and GBR for APA102
#define LED_TYPE APA102                                       // What kind of strip are you using? WS2812, APA102. . .
#define NUM_LEDS 60                                           // Number of LED's.

uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Palette definitions
CRGBPalette16 currentPalette = LavaColors_p;
CRGBPalette16 targetPalette = LavaColors_p;
TBlendType    currentBlending = LINEARBLEND;



void setup() {

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Slow startup so we can recover if code is buggy.
 
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  //WS2801 and APA102
//  LEDS.addLeds<LED_TYPE, LED_DT,COLOR_ORDER>(leds, NUM_LEDS);         // WS2812

  FastLED.setBrightness(max_bright);

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
} // setup()



void loop () {
  
                                                              // This section changes the delay, which adjusts how fast the 'rays' are travelling down the length of the strand.
  EVERY_N_MILLIS_I(thisTimer,100) {                           // This only sets the Initial timer delay. To change this value, you need to use thisTimer.setPeriod(); You could also call it thatTimer and so on.
    uint8_t timeval = beatsin8(10,20,50);                     // Create/modify a variable based on the beastsin8() function.
    thisTimer.setPeriod(timeval);                             // Use that as our update timer value.

    matrix_ray(millis()>>4);                                  // This is the main function that's called. We could have not passed the millis()>>4, but it's a quick example of passing an argument.
  }

  EVERY_N_MILLISECONDS(100) {                                 // Fixed rate of a palette blending capability.
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }

  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
  
  FastLED.show();

  Serial.println(LEDS.getFPS());

} // loop()



void matrix_ray(uint8_t colorIndex) {                                                 // Send a PWM'd sinewave instead of a random happening of LED's down the strand.

  static uint8_t thisdir = 0;                                                         // We could change the direction if we want to. Static means to assign that value only once.
  static int thisphase = 0;                                                           // Phase change value gets calculated. Static means to assign that value only once.
  uint8_t thiscutoff;                                                                 // You can change the cutoff value to display this wave. Lower value = longer wave.

  thisphase += beatsin8(1,20, 50);                                                    // You can change direction and speed individually.
  thiscutoff = beatsin8(50,164,248);                                                  // This variable is used for the PWM of the lighting with the qsubd command below.
  
  int thisbright = qsubd(cubicwave8(thisphase), thiscutoff);                          // It's PWM time. qsubd sets a minimum value called thiscutoff. If < thiscutoff, then thisbright = 0. Otherwise, thisbright = thiscutoff.
 
  if (thisdir == 0) {                                                                 // Depending on the direction, we'll put that brightness in one end of the array. Andrew Tuline wrote this.
    leds[0] = ColorFromPalette(currentPalette, colorIndex, thisbright, currentBlending); 
    memmove(leds+1, leds, (NUM_LEDS-1)*3);                                            // Oh look, the FastLED method of copying LED values up/down the strand.
  } else {
    leds[NUM_LEDS-1] = ColorFromPalette( currentPalette, colorIndex, thisbright, currentBlending);
    memmove(leds, leds+1, (NUM_LEDS-1)*3);    
  }

} // matrix_ray()

