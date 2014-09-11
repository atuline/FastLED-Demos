
// WAVE
//
// By: John Burroughs
// Modified by: Andrew Tuline
//
// Date: Aug, 2014
//
// This is a simple FastLED (2.1 and greater) display sequence using sin8() math.
// 
// FastLED 2.1 is available at https://github.com/FastLED/FastLED/tree/FastLED2.1
//
// Note: If you receive compile errors (as I have in the Stino add-on for Sublime Text), set the compiler to 'Full Compilation'.
//


#include <FastLED.h>                                           // FastLED library
 
#define LED_DT 13                                              // Data pin
#define NUM_LEDS 24                                            // Number of LED's
#define COLOR_ORDER GRB                                        // Change the order as necessary
#define LED_TYPE WS2811                                        // What kind of strip are you using?
#define BRIGHTNESS  196                                        // How bright do we want to go

struct CRGB leds[NUM_LEDS];                                    // Initializxe our array

// Initialize global variables for sequences
int mycount = 0;

uint8_t hue = 0;
uint8_t temp[NUM_LEDS];
uint8_t wave;
int wave_scale = 20;

long previousMillis;
int thisdelay = 40;


void setup() {
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED 2.1 Power management set at 5V, 500mA
  hue_array();
} // setup()


void loop () {
  hue_wave();
  hue_inc();
} // loop()


//populate a separate array for 'hue mask'
void hue_array() {
  for (byte i = 0; i < ((NUM_LEDS/2)+ 1); i++) {
    hue += 5;
    temp[i] = hue;
  }  
} // hue_array()


// Let's rotate the hue array over time
void hue_inc() {
  mycount++;
  if (mycount == 500) { 
    mycount = 0;
    for (byte i = 0; i < ((NUM_LEDS/2)+ 1); i++) temp[i]++;
  }
} // hue_inc()


// Displaying the wave of hues
void hue_wave(){
  static byte middle = NUM_LEDS/2;    
  if(millis() - previousMillis >= thisdelay) {
    for (byte i = 0; i <= middle; i++) {
      wave = sin8((millis() / wave_scale) * i);
      leds[i] = CHSV(temp[i], 255, wave);          
    }
    for (byte i = middle; i <= NUM_LEDS-1; i++) {
      leds[i] = leds[NUM_LEDS - i];
    }
    previousMillis = millis();
    show_at_max_brightness_for_power();
  }
} // hue_wave()