/* fht_log_ripple
 * 
 * By: Andrew Tuline
 * 
 * Date: January, 2017
 * 
 * 
 *  This is an example of the FHT library with FastLED for an AVR based Arduino. This uses a Fast Hartley Transform with beat (err, low frequency peak) 
 *  detection for FastLED and displaying a ripple effect.
 * 
 * It does so my converting the input to bins. We then take one of the lowest frequency bins and adjust the peak detection. At least it checks for low frequency.
 * 
 * FHT is available at http://wiki.openmusiclabs.com/wiki/ArduinoFHT
 * 
 * You can use the low frequency peak detector from this sketch to trigger all sort of other routines.
 * 
 * 
 * Hardware setup:
 * 
 * Arduino Nano & Addressable LED strips
 * - Powered by USB power bank 
 * - APA102 or WS2812 data connected to pin 12.
 * - APA102 clock connected to pin 11.
 * - 5V on APA102 or WS2812 connected to 5V on Nano (good for short strips).
 * - Gnd to Gnd on Nano.
 * 
 * 
 * Sparkfun MEMS microphone
 * - Vcc on microphone is connected to 3.3V on Nano.
 * - AREF on Nano connected to 3.3V on Nano.
 * - Mic out connected to A5 on Nano.
 * - Gnd to Gnd on Nano.
 * 
 * 10K linear potentiometer
 * - One end connected to 5V on Nano.
 * - One end connected to Gnd on Nano.
 * - Middle connected to A4 on Nano.
 * 
 * 
 * Notes: If you are using a microphone powered by the 3.3V signal, such as the Sparkfun MEMS microphone, then connect 3.3V to the AREF pin.
 *        If you are using a sound sensor from aliexpress, then you're probably SOL.
 *        If you don't have a 3.3V and AREF pin for your 3.3V microphone, then I'd be looking for a different Arduino.
 * 
 */


#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin, if using 4 pin strips.
#define COLOR_ORDER BGR                                       // Are they RGB, GRB or what??
#define LED_TYPE APA102                                       // What kind of strip are you using?
#define NUM_LEDS 60                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

// Fast Hartley Transform Definitions
#define LOG_OUT 1                                             // Use the logarithmic scale.             
#define FHT_N 256                                             // Set to 256 point fht.
#define inputPin A5                                           // MEMS Microphone input pin.
#define DC_OFFSET -10                                         // DC offset of the input. Leave at 0 if unsure.
#include <FHT.h>                                              // FHT library at http://wiki.openmusiclabs.com/wiki/ArduinoFHT

// Potentiometer definition and variable
#define potPin A4                                             // Let's attach the potentiometer to A4 of the Arduino.
uint8_t noiseval;                                             // Adjust the potentiometer to increase/decrease detection sensitivity. 52 seems to be a good value.

// Ripple variables
uint8_t colour;                                               // Ripple colour is randomized and incremented.
int center = 0;                                               // Center of the current ripple.
int step = -1;                                                // -1 is the initializing step.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps 16                                           // Case statement wouldn't allow a variable.

unsigned long currentMillis = 0;                              // Used for timing between detections
unsigned long previousMillis = 0;                             
unsigned long interval = 60;                                  // 60ms interval seems reasonable



void setup() {
  
  analogReference(EXTERNAL);                                  // Connect 3.3V to AREF pin for any microphones using 3.3V
  
  Serial.begin(57600);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
  
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812B LED_TYPE
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102 LED_TYPE

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // This is used by the power management functionality and is currently set at 5V, 500mA.

  currentPalette = PartyColors_p;                             // Nice bright starting colours.
  currentBlending = LINEARBLEND;

} // setup()



void loop() {

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }

  GetFHT();                                                   // Sample the input and generate an FHT table
  isripple();                                                 // Is there a low frequency peak detected? If so, trigger a new ripple.
  ripple();                                                   // Let's cycle through the ripple routine.

  FastLED.show();                         // Run the FastLED.show() at full loop speed.

} // loop()



void GetFHT() {

//  Serial.println(analogRead(inputPin) - 512 - DC_OFFSET);
  
  cli();
  for (int i = 0 ; i < FHT_N ; i++) fht_input[i] = analogRead(inputPin) - 512 - DC_OFFSET;
  sei();
  
  fht_window();                                               // Window the data for better frequency response.
  fht_reorder();                                              // Reorder the data before doing the fht.
  fht_run();                                                  // Process the data in the fht.
  fht_mag_log();                                              // Let's use logarithmic bins.

//  Serial.println(abs(fht_log_out[3]));                      // Alternatives are fht_log_out[], fht_lin_out[], and fht_oct_out[]
//  if (abs(fht_log_out[3]) - noiseval > 0) Serial.println (abs(fht_log_out[3]) - noiseval);

//  Serial.println(noiseval);
  
} // GetFHT()



void isripple() {

  noiseval = map(analogRead(potPin), 0, 1023, 16, 96);        // Read potentiometer and map for sensitivity.

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {           // Wait for interval ms before allowing a new ripple to be generated.
    previousMillis = currentMillis;
    if (abs(fht_log_out[3]) - noiseval > 0) step = -1;        // If the sound > threshold then start a ripple.
  } 
} // isripple()



void ripple() {

      fadeToBlackBy(leds, NUM_LEDS, 64);                      // 8 bit, 1 = slow, 255 = fast
  
  switch (step) {

    case -1:                                                  // Initialize ripple variables.
      center = random(NUM_LEDS);
      step = 0;
      break;

    case 0:
      leds[center] = ColorFromPalette(currentPalette, colour, 255, currentBlending);  // Display the first pixel of the ripple.
      step ++;
      break;

    case maxsteps:                                            // At the end of the ripples.
      // step = -1;
      break;

    default:                                                  // Middle of the ripples.
      leds[(center + step + NUM_LEDS) % NUM_LEDS] += ColorFromPalette( currentPalette, colour, myfade/step, currentBlending);        // Simple wrap from Marc Miller
      leds[(center - step + NUM_LEDS) % NUM_LEDS] += ColorFromPalette( currentPalette, colour, myfade/step, currentBlending); 
      
      step ++;                                                // Next step.
      
      break;
      
  } // switch step
  
  colour+=2;                                                  // Rotate the palette colour.
  
} // ripple()

