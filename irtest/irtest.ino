/* irtest
 * 
 * By: Andrew Tuline
 * 
 * Date: Nov, 2014
 * 
 * TEST program for IRLRemote and FastLED to ensure they work together with the strip you're using. It's also a good routine to test IR keyboard mapping.
 * 
 * I'm using the TSOP38238 IR receiver with an el cheapo aliexpress IR transmitter.
 * 
 * Libraries required:
 * 
 * https://github.com/NicoHood/IRLremote
 * https://github.com/FastLED/FastLED
 * 
 * Note: 3 pin LED strips such as WS2812B running FastLED won't work with this or with Ken Shiriff's IR library.
 * 
 */


#include "IRLremote.h"                                            // https://github.com/NicoHood/IRLremote
#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// choose a valid PinInterrupt pin of your Arduino board
#define pinIR 2                                               // I'm using pin D2
#define IRL_BLOCKING true

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 60                                           // Number of LED's.

struct CRGB leds[NUM_LEDS];

// Global variables can be changed on the fly.
uint8_t max_bright = 128;                                      // Overall brightness.

// temporary variables to save latest IR input
uint8_t IRProtocol = 0;
uint16_t IRAddress = 0;
uint32_t IRCommand = 0;



void setup() {

  Serial.begin(57600);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
  
  attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_NEC>, CHANGE);    // IR definition
  
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // For WS2812B - Does not work due to 3 pin configuration.
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
} // setup()



void loop() {

   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CRGB::Gray;
      FastLED.show();
      FastLED.delay(10);
      leds[whiteLed] = CRGB::Black;
   }

  irtest();
  
} // loop()



void irtest() {                                      // This is the built-in IR function that just selects a mode.

 if (IRProtocol)  {                                  // Read the IR receiver. Result are poor if using 3 pin strands such as WS2812B.
    Serial.print("Command:");
    Serial.println(IRCommand);
    switch(IRCommand) {

      case 65280:  change_mode(1);   break;          //a1 - 
      case 65025:  change_mode(2);   break;          //a2 - 
      case 64770:  change_mode(3);   break;          //a3 - 
      case 64515:  change_mode(4);   break;          //a4 - 

      case 64260:  change_mode(5);   break;          //b1 - 
      case 64005:  change_mode(6);   break;          //b2 - 
      case 63750:  change_mode(7);   break;          //b3 - 
      case 63495:  change_mode(8);   break;          //b4 - 

      case 63240:  change_mode(9);   break;          //c1 - 
      case 62985:  change_mode(10);  break;          //c2 - 
      case 62730:  change_mode(11);  break;          //c3 - 
      case 62475:  change_mode(12);  break;          //c4

      case 62220:  change_mode(13);  break;          //d1
      case 61965:  change_mode(14);  break;          //d2 - 
      case 61710:  change_mode(15);  break;          //d3 - 
      case 61455:  change_mode(16);  break;          //d4

      case 61200:  change_mode(17);  break;          //e1 -
      case 60945:  change_mode(18);  break;          //e2 - 
      case 60690:  change_mode(19);  break;          //e3 - 
      case 60435:  change_mode(20);  break;          //e4

      case 60180:  change_mode(21);  break;          //f1
      case 59925:  change_mode(22);  break;          //f2 - 
      case 59670:  change_mode(98);  break;          //f3 - 
      case 59415:  change_mode(99);  break;          //f4

      default: break;                                // We could do something by default
    } // switch
    IRProtocol = 0;
  } // if IRLavailable()

} // irtest()



void IREvent(uint8_t protocol, uint16_t address, uint32_t command) {      // This is called by the IRLremote library
  
  if (IRL_BLOCKING && !IRProtocol) {
    IRProtocol = protocol;                           // update the values to the newest valid input
    IRAddress = address;
    IRCommand = command;
  }
  
} // IREvent()



void change_mode(int newmode) {
  
  Serial.print("New mode: ");
  Serial.println(newmode);
  
} // change_mode()

