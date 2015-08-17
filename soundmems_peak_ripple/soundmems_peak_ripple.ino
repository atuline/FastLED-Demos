
/* soundmems_peak_ripple

By: Andrew Tuline

Date: August, 2015

Basic code to read from the Sparkfun INMP401 microphone and display CHSV output.

Does some simple peak detection based on a number of samples. Peaks then trigger a ripple.

A 10K potentiometer is used to adjust peak detection sensitivity.



My hardware setup:

Arduino Nano & Addressable LED strips
- Powered by USB power bank 
- APA102 or WS2812 data connected to pin 12.
- APA102 clock connected to pin 11.
- 5V on APA102 or WS2812 connected to 5V on Nano (good for short strips).
- Gnd to Gnd on Nano.


Sparkfun MEMS microphone
- Vcc on microphone is connected to 3.3V on Nano.
- AREF on Nano connected to 3.3V on Nano.
- Mic out connected to A5.
- Gnd to Gnd on Nano.

10K linear potentiometer
- One end connected to 5V on Nano.
- One end connected to Gnd on Nano.
- Middle connected to A4 on Nano.


Note: If you are using a microphone powered by the 3.3V signal, such as the Sparkfun MEMS microphone, then connect 3.3V to the AREF pin.


*/

#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B and BGR for APA102
#define LED_TYPE WS2812                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 30                                           // Number of LED's.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

#define MIC_PIN    5                                          // Analog port for microphone
#define DC_OFFSET  0                                          // DC offset in mic signal - if unusure, leave 0

#define POT_PIN    4


int max_bright = 255;

// Samples
#define NSAMPLES 64
unsigned int samplearray[NSAMPLES];
unsigned long samplesum = 0;
unsigned int sampleavg = 0;

int samplecount = 0;
unsigned int sample = 0;

unsigned long oldtime = 0;
unsigned long newtime = 0;


// Ripple variables
uint8_t colour;                                               // Ripple colour is randomized.
int center = 0;                                               // Center of the current ripple.
int step = -1;                                                // -1 is the initializing step.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps 16                                           // Case statement wouldn't allow a variable.
int peakspersec = 0;
int peakcount = 0;

uint8_t bgcol = 0;                                             // Background colour rotates.



void setup() {
  analogReference(EXTERNAL);                                  // 3.3V reference for analog input.
  pinMode(13, OUTPUT);                                        // Peak detection LED
 
  Serial.begin(57600);

  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // Use this for WS2812B
//  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);

  Serial.println("Start");
} // setup()



void loop() {

  EVERY_N_MILLISECONDS(1000) {
    peakspersec = peakcount;                                       // Count the peaks per second. This value will become the foreground hue.
    peakcount = 0;                                                 // Reset the counter every second.
  }

  soundmems();

  EVERY_N_MILLISECONDS(20) {
   ripple();
  }

   show_at_max_brightness_for_power();

} // loop()


void soundmems() {                                                  // Rolling average counter - means we don't have to go through an array each time.
  newtime = millis();
  int tmp = analogRead(MIC_PIN) - 512;
  sample = abs(tmp);

  int potin = map(analogRead(POT_PIN), 0, 1023, 0, 60);

  samplesum = samplesum + sample - samplearray[samplecount];        // Add the new sample and remove the oldest sample in the array 
  sampleavg = samplesum / NSAMPLES;                                 // Get an average
  samplearray[samplecount] = sample;                                // Update oldest sample in the array with new sample
  samplecount = (samplecount + 1) % NSAMPLES;                       // Update the counter for the array

  if (newtime > (oldtime + 200)) digitalWrite(13, LOW);             // Turn the LED off 200ms after the last peak.

  if ((sample > (sampleavg + potin)) && (newtime > (oldtime + 60)) ) { // Check for a peak, which is 30 > the average, but wait at least 60ms for another.
    step = -1;
    peakcount++;
    digitalWrite(13, HIGH);
    oldtime = newtime;
  }
}  // soundmems()



void ripple() {
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(bgcol, 255, sampleavg*2);  // Set the background colour.

  switch (step) {

    case -1:                                                          // Initialize ripple variables.
      center = random(NUM_LEDS);
      colour = (peakspersec*10) % 255;                                             // More peaks/s = higher the hue colour.
      step = 0;
      bgcol = bgcol+8;
      break;

    case 0:
      leds[center] = CHSV(colour, 255, 255);                          // Display the first pixel of the ripple.
      step ++;
      break;

    case maxsteps:                                                    // At the end of the ripples.
      // step = -1;
      break;

    default:                                                             // Middle of the ripples.
      leds[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade/step*2);       // Simple wrap from Marc Miller.
      leds[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade/step*2);
      step ++;                                                         // Next step.
      break;  
  } // switch step
} // ripple()
