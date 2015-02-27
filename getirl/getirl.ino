/* getirl

By: Andrew Tuline

Date: Nov, 2014

This is a test program for IRLRemote and FastLED.

Libraries required:

https://github.com/NicoHood/IRLremote
https://github.com/FastLED/FastLED

Note: 3 pin LED strips such as WS2812B running FastLED won't work with IRLRemote or Ken Shiriff's IR library.
*/


#include "FastLED.h"
#include "IRLremote.h"


const int interruptIR = 0;

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 20                                           // Number of LED's

struct CRGB leds[NUM_LEDS];


void setup() {
  Serial.begin(57600);
  IRLbegin<IR_ALL>(interruptIR);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // For WS2812B - Does not work due to 3 pin configuration.
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801
}

void loop() {
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CRGB::White;
      FastLED.show();
//      FastLED.delay(10);                // This is a bad practice
      leds[whiteLed] = CRGB::Black;
   } // for()

//  FastLED.delay(20);
  getirl();
} // loop()


/*
IR Commands on 2nd Arduino

The following section provides a list of commands that are sent to the main Arduino.

I'm using a 24 pin IR controller from Aliexpress. IR button locations are defined as follows:

- letters correspond to rows
- numbers corresponding to columns

For example, A1 = top left, A4 = top right, and F4 = bottom right.

Serial (below) is the string being sent over the Tx line to the Arduino performing the display.


Serial    Command                       IR Button location
------    --------                      ------------------
m889      Set mode 889                        F4 
m888      Set mode 888                        E4
m1        Set mode 1                          A3
m0        Set mode 0                          A4
e1, e2    Increase/Decrease delay             C2, C3
n0, n1    Direction reverse/forward           E2, E3
o1, o2    Decrease/Increase mode              D2, D3
r1, r2    Increase/Decrease brightness        A1, A2
u1, u2    Decrease/Increase hue               F2, F3

Not yet implemented
y         Increase/decrease # of LED's        B1, with A1, A2
          Save # of LED's to flash            B1

*/



void getirl() {                                       // This is the built-in IR function that just selects a mode.
  if (IRLavailable()) {                              // Read the IR receiver. Result are poor if using 3 pin strands such as WS2812B.
//    Serial.print("Command:");
//    Serial.println(IRLgetCommand());
    switch(IRLgetCommand()) {

      case 65280:  change_mode(1);   break;          //a1 - maxbright++;
      case 65025:  change_mode(2);   break;          //a2 - maxbright--;
      case 64770:  change_mode(3);   break;          //a3 - change_mode(1);
      case 64515:  change_mode(4);   break;          //a4 - change_mode(0);

      case 64260:  change_mode(5);   break;          //b1 - 
      case 64005:  change_mode(6);   break;          //b2 - 
      case 63750:  change_mode(7);   break;          //b3 - 
      case 63495:  change_mode(8);   break;          //b4 - 

      case 63240:  change_mode(9);   break;          //c1 - 
      case 62985:  change_mode(10);  break;          //c2 - thisdelay++;
      case 62730:  change_mode(11);  break;          //c3 - thisdelay--;
      case 62475:  change_mode(12);  break;          //c4

      case 62220:  change_mode(13);  break;          //d1
      case 61965:  change_mode(14);  break;          //d2 - ledMode--; change_mode(ledMode);
      case 61710:  change_mode(15);  break;          //d3 - ledMode++; change_mode(ledMode);
      case 61455:  change_mode(16);  break;          //d4

      case 61200:  change_mode(17);  break;          //e1 -
      case 60945:  change_mode(18);  break;          //e2 - thisdir = 1;
      case 60690:  change_mode(19);  break;          //e3 - thisdir = 0;
      case 60435:  change_mode(20);  break;          //e4

      case 60180:  change_mode(21);  break;          //f1
      case 59925:  change_mode(22);  break;          //f2 - thishue--;
      case 59670:  change_mode(98);  break;          //f3 - thishue++;
      case 59415:  change_mode(99);  break;          //f4

      default: break;                                // We could do something by default
    } // switch
    IRLreset();
  } // if IRLavailable()
} // getir()



void change_mode(int newmode) {
  Serial.print("New mode: ");
  Serial.println(newmode);
}