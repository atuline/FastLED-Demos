 /* fht_log_ripple
 * 
 * By: Andrew Tuline
 * 
 * Date: January, 2017
 * 
 * Update: March, 2019 (with high speed A/D sampling)
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
 * The microphone used for this routine is a Sparkfun ADMP401 MEMS microphone, which runs at 3.3V. With this microphone, we need to tie the AREF pin to 3.3V (if
 * using a 5V Arduino UNO/Nano/?. In setup(), you would perform:
 * 
 * analogReference(EXTERNAL); 
 * 
 * or use the direct port equivalent, which is:
 * 
 * ADMUX |= (0 << REFS0);
 * 
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
#define MIC_PIN 5                                           // MEMS Microphone input pin.
#define DC_OFFSET  509                                        // DC offset in mic signal. Should probably be about 512.
#include <FHT.h>                                              // FHT library at http://wiki.openmusiclabs.com/wiki/ArduinoFHT


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
  
//  analogReference(EXTERNAL);                                  // This is configured in the get_sound routine. In general, you would connect 3.3V to AREF pin for any microphones using 3.3V
  
  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
  
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812B LED_TYPE
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102 LED_TYPE

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // This is used by the power management functionality and is currently set at 5V, 500mA.

  currentPalette = PartyColors_p;                             // Nice bright starting colours.
  currentBlending = LINEARBLEND;

// Setup the ADC for polled 10 bit sampling on analog pin 5 at 19.2kHz.
  cli();                                  // Disable interrupts.
  ADCSRA = 0;                             // Clear this register.
  ADCSRB = 0;                             // Ditto.
  ADMUX = 0;                              // Ditto.
  ADMUX |= (MIC_PIN & 0x07);              // Set A5 analog input pin.
  ADMUX |= (0 << REFS0);                  // Set reference voltage  (analog reference(external), or using 3.3V microphone on 5V Arduino.
                                          // Set that to 1 if using 5V microphone or 3.3V Arduino.
//  ADMUX |= (1 << ADLAR);                  // Left justify to get 8 bits of data.                                          
  ADMUX |= (0 << ADLAR);                  // Right justify to get full 10 A/D bits.

//  ADCSRA |= bit (ADPS0) | bit (ADPS2);                //  32 scaling or 38.5 KHz sampling
//  ADCSRA |= bit (ADPS1) | bit (ADPS2);                //  Set ADC clock with 64 prescaler where 16mHz/64=250kHz and 250khz/13 instruction cycles = 19.2khz sampling.
  ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);    // 128 prescaler with 9.6 KHz sampling


  ADCSRA |= (1 << ADATE);                 // Enable auto trigger.
//  ADCSRA |= (1 << ADIE);                  // Enable interrupts when measurement complete (if using ISR method). Sorry, we're using polling here.
  ADCSRA |= (1 << ADEN);                  // Enable ADC.
  ADCSRA |= (1 << ADSC);                  // Start ADC measurements.
  sei();                                  // Re-enable interrupts.

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

  FastLED.show();                                             // Run the FastLED.show() at full loop speed.

} // loop()



void GetFHT() {

  get_sound();                                                // High speed sample of sound which is input into the FHT array.
  fht_window();                                               // Window the data for better frequency response.
  fht_reorder();                                              // Reorder the data before doing the fht.
  fht_run();                                                  // Process the data in the fht.
  fht_mag_log();                                              // Let's use logarithmic bins.

} // GetFHT()



void get_sound() {                                            // Uses high speed polled analog sampling and NOT analogRead().

    cli();                                                    // Disable interrupts for the duration of the sampling.
    for (int i = 0 ; i < FHT_N ; i++) {                       // Save 256 samples. No more, no less.
      while(!(ADCSRA & 0x10));                                // Wait for adc to be ready.
      ADCSRA = 0xf5;                                          // Restart adc
      fht_input[i] = ADC - DC_OFFSET;                         // Get the full 10 bit A/D conversion and center it.
    }
    sei();                                                    // Re-enable interrupts.
    
} // get_sound()



void isripple() {

  uint8_t noiseval = 32;                                        // Our detection value.

//  noiseval = map(analogRead(potPin), 0, 1023, 16, 96);        // Read potentiometer and map for sensitivity. Removed.

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {           // Wait for interval ms before allowing a new ripple to be generated.
    previousMillis = currentMillis;
    if (abs(fht_log_out[3]) - noiseval > 0) step = -1;        // If the low frequency sound > threshold then start a ripple.
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

