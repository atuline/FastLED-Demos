/* demoReel100_button_sound
 *
 * Originally By: Mark Kriegsman
 * 
 * Modified by: Andrew Tuline
 *
 * Date:    Jan 2017
 * Updated: Feb 2021
 * 
 * YouTube:
 * GitHub:
 * 
 * Here's a modification of Mark Kriegsman's demoReel100 program, where he animates 6 display sequences within 100 lines of code.
 * This version adds 3-mode button support, EEPROM read/write and basic volume reactive capabilities to his animations, but goes well 
 * beyond his 100 lines of code.
 * 
 * You don't need to implement an MSGEQ7 frequency chip or Fast Fourier transforms in order to make cool sound reactive displays.
 * Basic microphone sampling and using the results creatively in your display routines can go a long way.
 * 
 *
 * Hardware setup:
 * 
 * This has now been tested on Arduino UNO/Nano, ESP8266 and ESP32.
 * Button connected to an analog pin (see below) and ground. It'll use an internal pullup resistor
 * Microphone connected to pin below and to 3.3V (NOT, I repeat NOT 5V).
 * Microphones are typically INMP401, MAX4466, MAX9814 (configured for 40db gain).
 * On an UNO/Nano, the AREF pin is connected to the 3.3V pin.
 * 
 * Device   MIC_PIN    LED_PIN    BUTTON_PIN
 * 
 * UNO/Nano   A5          12          6
 * ESP8266    A0          D4          D5
 * ESP32      36          2           19
 * 
 * 
 * Operating instructions:
 * 
 * Click to change to the next mode.
 * Double click to reset to mode 0.
 * Long click to save current mode to EEPROM (for all 3 platforms).
 * Squelch value is typically < 20.
 * This does not have a sensitivity multiplier.
 *
 *
 * Sound variables used:
 * 
 * micIn - A uint8_t value, which is typically 0-64, so you may need to use a multiplier in order to range from 0-255.
 * squelch - Increase this value in order to reduce background 'noise'.
 * 
 * Note: Some devices exhibit a considerable amount of noise/spikes.  Could be the microphone, could be interference.
 * You may need to increase the squelch value significantly to eliminate it. If you have a problem with noise, just
 * write a basic sampling sketch and see what you get.
 * 
 * 
 * Feel free to experiment and find different ways to add sound reactivity to these or new effects.
 * 
 */


#include <FastLED.h>


 #include "EEPROM.h"                                          // Removed because it's different between platforms. Was developed for UNO/Nano.

#define MIC_PIN A5                                            // Tested with A5 for UNO/Nano, A0 for ESP8266, 36 for ESP32.
#define squelch 30                                            // Change to suit the noisiness of your microphone/environment.

#define BUTTON_PIN 6                                          // Tested with 6 for UNO/Nano, D5 for ESP8266, 19 for ESP32.
#include "jsbutton.h"                                         // Nice button routine by Jeff Saltzman.


#define LED_DT 12                                             // 12 for UNO/Nano, D4 for ESP8266 and 2 for ESP32.
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define NUM_LEDS 30

#define FRAMES_PER_SECOND 60

uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.
struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.
CRGBPalette16 palette = PartyColors_p;

int micIn;                                                    // Raw microphone input is up to 12 bit A/D.


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))            // Definition for the array of routines to display.

uint8_t gCurrentPatternNumber = 0;                            // Index number of which pattern is current.
uint8_t gHue = 0;                                             // Rotating "base color" used by many of the patterns.


#include "animations.h"                                       

typedef void (*SimplePatternList[])();                        // Define a type that's a pointer to an array.
SimplePatternList gPatterns = {rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };     // Here's an array of patterns.


void setup() {

  Serial.begin(115200);                                       // I use this for serial debugging. . . A LOT!
  delay(100);
  
  #ifdef AVR                                                  // We are using 3.3V microphones, so this requires a special setup on an AVR device.
    analogReference(INTERNAL);                                  
  #endif

  #ifndef AVR
    EEPROM.begin(1);                                          // Required for ESP8266 and ESP32 only.
  #endif
  
  pinMode(BUTTON_PIN, INPUT);                                 // Define button input pin.
  pinMode(BUTTON_PIN, INPUT_PULLUP);                          // Configure internal pullup. Don't use digitalWrite(BUTTON_PIN, HIGH).
  
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  
  gCurrentPatternNumber = EEPROM.read(0) % ARRAY_SIZE(gPatterns);
  
} // setup()



void loop() {

  analog_sample(); 
  readbutton();

//  EVERY_N_SECONDS(5) {                                     // Uncomment if you want the routines to automatically rotate every X seconds.
//    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
//  }

  EVERY_N_MILLIS(1000/FRAMES_PER_SECOND) {                    // You can optionally set the display frame rate to a fixed frame rate without
                                                              // resorting to the blocking method of FastLED.delay(1000/FRAMES_PER_SECOND).
    gPatterns[gCurrentPatternNumber]();                       // Call the current pattern function once, updating the 'leds' array.
    gHue++;
    FastLED.show();
  }

} // loop()



void analog_sample() {

  static float micLev;                        // Zeroing variable needs to be a float. Otherwise an integer smoothing calculation would be very inaccurate.
  static float micSm;                         // Used for exponential smoothing. Also needs to be a float.
  const float smWeight = 0.1;                 // Exponential smoothing, between 0 and 1.0. Lower value = smoother.
  
  micIn = analogRead(MIC_PIN);
  #ifdef ESP32                                // Convert 12 bit A/D to 10 bit A/D for ESP32.
    micIn = micIn >> 2;
  #endif
  micLev = ((micLev*31)+micIn)/32;            // Quickie exponential smoothing to get average value, which is used for zeroeing.
  micIn -= micLev;                            // Zero out micIn, so that we don't have to define an offset for different analog microphones.
  micIn = abs(micIn);
  micSm = (smWeight * (float)micIn + (1.0-smWeight) * micSm); // Longer version of exponential smoothing. Does the same thing.
  micIn = (uint8_t)micSm;                     // We have now zeroed out and smoothed the microphone input a bit. Can't do FFT with this.
  micIn = (micIn <= squelch) ? 0: micIn;      // Get rid of background noise.

} // analog_sample()



void readbutton() {
  uint8_t b = checkButton();
  if (b == 1) gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);   // Just a click event to advance to next pattern.
  if (b == 2) gCurrentPatternNumber = 0;                                                     // A double-click event to reset to 0 pattern.
  if (b == 3) {
    EEPROM.write(0,gCurrentPatternNumber);                  // A hold event to write current pattern to EEPROM.
    #ifndef AVR
      EEPROM.commit();
    #endif
  }
} // readbutton()
