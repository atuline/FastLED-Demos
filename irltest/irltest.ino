/* irltest
 * 
 * By: Andrew Tuline
 * 
 * Date: Nov, 2014
 * 
 * Updated: May, 2019
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



#include "FastLED.h"                                          // https://github.com/FastLED/FastLED     Use 3.2
#include "IRLremote.h"                                        // https://github.com/NicoHood/IRLremote  Use 2.0.2

#include "commands.h"                                         // The IR commands.

#if IRL_VERSION < 202
#error "Requires IRLRemote 2.0.2 or later; check github for latest code."
#endif

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// choose a valid PinInterrupt pin of your Arduino board
#define pinIR 2                                               // I'm using pin D2
#define IRL_BLOCKING true

CNec IRLremote;

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 60                                           // Number of LED's.

struct CRGB leds[NUM_LEDS];

// Global variables can be changed on the fly.
uint8_t max_bright = 128;                                      // Overall brightness.



void setup() {

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  
  delay(1000);                                                // Soft startup to ease the flow of electrons.
  
  if (!IRLremote.begin(pinIR))
    Serial.println(F("You did not choose a valid pin."));
  
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // For WS2812B - Does not work due to 3 pin configuration.
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801

  FastLED.setBrightness(max_bright);
  
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
} // setup()



void loop() {

   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CRGB::Gray;
      if (!IRLremote.receiving()) {
        FastLED.show();                                                             // Power managed display of LED's.
        FastLED.delay(10);
      }
      leds[whiteLed] = CRGB::Black;
   }

  irltest();
  
} // loop()



void irltest() {                                      // This is the built-in IR function that just selects a mode.


  if (IRLremote.available()) {
    
    auto data = IRLremote.read();           // Get the new data from the remote

//    Serial.print(F("Address: "));           // Print the protocol data. Note that there's also 65535, which we don't use.
//    Serial.println(data.address);
//    Serial.print(F("Command: "));
//    Serial.println(data.command);
//    Serial.println();

    if (data.address == IR_ADD) {     
      switch(data.command) {

      case IR_A1:  change_mode(1);   break;          //a1 - 
      case IR_A2:  change_mode(2);   break;          //a2 - 
      case IR_A3:  change_mode(3);   break;          //a3 - 
      case IR_A4:  change_mode(4);   break;          //a4 - 

      case IR_B1:  change_mode(5);   break;          //b1 - 
      case IR_B2:  change_mode(6);   break;          //b2 - 
      case IR_B3:  change_mode(7);   break;          //b3 - 
      case IR_B4:  change_mode(8);   break;          //b4 - 

      case IR_C1:  change_mode(9);   break;          //c1 - 
      case IR_C2:  change_mode(10);  break;          //c2 - 
      case IR_C3:  change_mode(11);  break;          //c3 - 
      case IR_C4:  change_mode(12);  break;          //c4 -

      case IR_D1:  change_mode(13);  break;          //d1 -
      case IR_D2:  change_mode(14);  break;          //d2 - 
      case IR_D3:  change_mode(15);  break;          //d3 - 
      case IR_D4:  change_mode(16);  break;          //d4 -

      case IR_E1:  change_mode(17);  break;          //e1 -
      case IR_E2:  change_mode(18);  break;          //e2 - 
      case IR_E3:  change_mode(19);  break;          //e3 - 
      case IR_E4:  change_mode(20);  break;          //e4 -

      case IR_F1:  change_mode(21);  break;          //f1 -
      case IR_F2:  change_mode(22);  break;          //f2 - 
      case IR_F3:  change_mode(98);  break;          //f3 - 
      case IR_F4:  change_mode(99);  break;          //f4 -

      default: break;                                // We could do something by default
      
      } // switch IRCommand
    } // if IR_ADD
  } // if IRLRemote


} // irltest()



void change_mode(int newmode) {
  
  Serial.print("New mode: ");
  Serial.println(newmode);
  
} // change_mode()
