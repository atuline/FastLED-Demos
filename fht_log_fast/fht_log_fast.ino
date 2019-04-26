/* fht_log_fast
 * 
 * By: Andrew Tuline
 *
 * Date: Mar, 2019
 * 
 *
 * Why use a 7 band MSGEQ7 graphic equalizer chip when you can get many times more frequency bins with software alone and on an Arduino UNO to boot?
 * 
 * This example combines high speed A/D conversion, FHT (Fast Hartley Transform) pitch detection combined with simple FastLED library display using
 * an Arduino Nano, an ADMP401 MEMS microphone and a strip of APA102 led's.
 *
 *
 * The advantage of using MSGEQ7 chips is that the coding is relatively simple. The disadvantage is that:
 * 
 *  - you need extra hardware.
 *  - you get a measly 7 frequency bins.
 *  - the results can often be jittery.
 * 
 * By using the Open Music Labs FHT (Fast Hartley Transform) library in conjunction with high speed A/D conversion, you can perform pitch detection on an 
 * Arduino UNO with much more capability than an MSGEQ7 chip. Oh, and that library gives you about a 5 millisecond conversion, and combined with FastLED
 * you can get on the order of 125 frames per second. That's no slouch for an 8 bit 16MHz microcontroller.
 * 
 * The FHT library is available at http://wiki.openmusiclabs.com/wiki/ArduinoFHT
 *
 * The A/D conversion uses direct port manipulation and can run at 19KHz, 38KHz or even higher as opposed to 8KHz for a standard analogRead() conversion.
 * Furthermore, by disabling interrupts for a short period of time, you can reduce jitter during the sampling. This has a side effect of making 3 pin WS2812
 * displays less reliable and, as a result, we'll use 4 pin APA102 LED's instead. Reference:
 * 
 * https://github.com/FastLED/FastLED/wiki/Interrupt-problems
 *  
 *  
 * The microphone used for this routine is a Sparkfun ADMP401 MEMS microphone, which runs at 3.3V. With this microphone, we need to tie the AREF pin to 3.3V,
 * and in setup(), perform:
 * 
 * analogReference(EXTERNAL); 
 * 
 * or use the direct port equivalent, which is:
 * 
 * ADMUX |= (0 << REFS0);
 * 
 * 
 * Testing has been performed with a an Android based Function Generator by Keuwlsoft. With so many frequency bins to choose from, you can select the ranges you
 * want to work with and discard the rest.
 * 
 */


#define qsubd(x, b) ((x>b)?b:0)                               // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x>b)?x-b:0)                             // Unsigned subtraction macro. if result <0, then => 0.

// FHT Definitions
#define LOG_OUT 1                                             // Use logarithmic based bins (is required for the library to run).
#define FHT_N 256                                             // Set to 256 point fht. Any less, and the upper ranges won't work well.
#define DC_OFFSET  509                                        // DC offset in mic signal. Should probably be about 512.
#define MIC_PIN 5                                             // We're using A5.

#include <FHT.h>                                              // FHT library at http://wiki.openmusiclabs.com/wiki/ArduinoFHT

#include "FastLED.h"                                          // FastLED library at https://github.com/FastLED/FastLED

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for APA102 or WS2801
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (APA102, WS2801 or WS2812B)
#define NUM_LEDS 40                                           // Number of LED's.


// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.




void setup() {
  
  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
  
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

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
  ADCSRA |= bit (ADPS1) | bit (ADPS2);                //  Set ADC clock with 64 prescaler where 16mHz/64=250kHz and 250khz/13 instruction cycles = 19.2khz sampling.
//  ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);    // 128 prescaler with 9.6 KHz sampling
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1);  // Set ADC clock with 64 prescaler where 16mHz/64=250kHz and 250khz/13 instruction cycles = 19.2khz sampling.
// ADCSRA |= (1 << ADPS2) | (1 << ADPS0);   // Set ADC clock with 32 prescaler for 38.5 KHz sampling.
  ADCSRA |= (1 << ADATE);                 // Enable auto trigger.
//  ADCSRA |= (1 << ADIE);                  // Enable interrupts when measurement complete (if using ISR method). Sorry, we're using polling here.
  ADCSRA |= (1 << ADEN);                  // Enable ADC.
  ADCSRA |= (1 << ADSC);                  // Start ADC measurements.
  sei();                                  // Re-enable interrupts.

} // setup()



void loop() {

//  showfps();                                                  // Debug output of how many frames per second we're getting. Comment this out in production.
  getFHT();                                                   // Let's take FHT_N samples and crunch 'em.
  fhtDisplay();                                               // Let's calculate the LED display from our FHT output array.
  FastLED.show();                                             // And then display it.

}  // loop()



void getFHT() {
  
  get_sound();                                                // High speed sound sampling.
  fht_window();                                               // Window the data for better frequency response.
  fht_reorder();                                              // Reorder the data before doing the fht.
  fht_run();                                                  // Process the data in the fht.
  fht_mag_log();                                              // I guess we'll be converting to logarithm.
                                                              // Really, I don't know what these do to tell you the truth.
  
} // GetFHT()



void get_sound() {                                            // Uses high speed polled analog sampling and NOT analogRead().

// Here's the slow and jittery 8KHz method of sampling sound, which we no longer use.
//  for (int i = 0 ; i < FHT_N ; i++) fht_input[i] = analogRead(inputPin) - DC_OFFSET;
  
    cli();
    for (int i = 0 ; i < FHT_N ; i++) {                       // Save 256 samples. No more, no less.
      while(!(ADCSRA & 0x10));                                // Wait for adc to be ready.
      ADCSRA = 0xf5; // restart adc
      fht_input[i] = ADC - DC_OFFSET;                         // Get the full 10 bit A/D conversion and center it.
    
/*    Serial.print(abs(sample));                              // Serial plot graph of our sampling.
      Serial.print(" ");                                      // Lowest and highest values are graphed so that the plot isn't auto-scaled.
      Serial.print(0);                                        // Lowest value
      Serial.print(" ");
      Serial.print(512);                                      // Highest value
      Serial.println(" ");
*/
    }
    sei();
    
} // get_sound()



void fhtDisplay() {

  #define hueinc 0                                            // A hue increment value to make it rotate a bit.
  #define micmult 10                                          // Bin values are very low, to let's crank 'em up.
  #define noiseval 32                                         // Increase this to reduce sensitivity.
  
  for (int i= 0; i < NUM_LEDS; i++) {                         // Run through the LED array.
    int tmp = qsuba(fht_log_out[2*i+2], noiseval);            // Get the sample and subtract the 'quiet' normalized values, but don't go < 0.
    if (tmp > (leds[i].r + leds[i].g + leds[i].b))            // Refresh an LED only when the intensity is low. By Andrew Tuline.
        leds[i] = CHSV(tmp*micmult+hueinc, 255, tmp*micmult); // Note how we really cranked up the tmp value to get BRIGHT LED's. Also increment the hue for fun.
    leds[i].nscale8(224);                                     // Let's fade the whole thing over time as well.
  }

} // fhtDisplay()



void showfps() {

  long currentMillis = 0;
  static long lastMillis = 0;
  static long loops = 0;
  
  currentMillis=millis();                                     // Determine frames per second
  loops++;
  if(currentMillis - lastMillis > 1000){
    Serial.println(loops);
    lastMillis = currentMillis;
    loops = 0;
  }
} // showfps()
