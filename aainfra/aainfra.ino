
/* aainfra for aalight LED effects

By: Andrew Tuline

Date: March, 2015


Introduction

This program is for a second Arduino that receives IR signals, decodes them and serially transmits a command to an Arduino
that is controlling an LED strip with FastLED running aalight.ino.

If you are using 4 pin strips, such as the WS2801 or APA102, then you can use the IR built right into aalight. This is really only means for
3 pin strips, such as Neopxixels aka WS2812B.

Important note: Make sure you have disconnected USB to your computer before connecting two Arduino's Tx and Rx lines together.


Library Required

https://github.com/NicoHood/IRLremote



Commands Supported

The following section provides a list of commands that are sent to the main Arduino.

I'm using a 24 pin IR controller from Aliexpress. IR button locations are defined as follows:

- letters correspond to rows
- numbers corresponding to columns

For example, A1 = top left, A4 = top right, and F4 = bottom right.

Serial (below) is the string being sent over the Tx line to the Arduino performing the display.

See the table below to see what strings are sent serially. Review aalight.ino to see what commands that those string perform.

*/

#include "IRLremote.h"

const int interruptIR = 0;                         // This is pin D2 on an UNO.

String command;                                    // We'll be sending strings and not single characters to Tx.
boolean understood = false;                        // Was the IR sequence received valid or not.


void setup() {
  Serial.begin(57600);
  IRLbegin<IR_ALL>(interruptIR);
} // setup()


void loop() {
  getirl();
} // loop()


void getirl() {
 if (IRLavailable()) {                             // Read the IR Receiver
    understood = true;

//    Serial.print("Command:");
//    Serial.println(IRLgetCommand());
    switch(IRLgetCommand()) {
                                                   // Button pressed
      case 65280:  command = "r2";   break;        // a1
      case 65025:  command = "r1";   break;        // a2
      case 64770:  command = "m1";   break;        // a3
      case 64515:  command = "m0";   break;        // a4

      case 64260:  command = "m6";   break;        // b1
      case 64005:  command = "m7";   break;        // b2
      case 63750:  command = "m8";   break;        // b3
      case 63495:  command = "m9";   break;        // b4

      case 63240:  command = "m10";  break;        // c1
      case 62985:  command = "e2";   break;        // c2
      case 62730:  command = "e1";   break;        // c3
      case 62475:  command = "m13";  break;        // c4

      case 62220:  command = "m14";  break;        // d1
      case 61965:  command = "o1";   break;        // d2
      case 61710:  command = "o2";   break;        // d3
      case 61455:  command = "m17";  break;        // d4

      case 61200:  command = "m18";  break;        // e1
      case 60945:  command = "n0";   break;        // e2
      case 60690:  command = "n1";   break;        // e3
      case 60435:  command = "m98";  break;        // e4

      case 60180:  command = "m22";  break;        // f1
      case 59925:  command = "u1";   break;        // f2
      case 59670:  command = "u2";   break;        // f3
      case 59415:  command = "m99";  break;        // f4

      default:        understood = false;          // We could do something by default
    } // switch
    if (understood) Serial.println(command);
    IRLreset();                                    // Receive the next value
  } // if irrecv
} // getirl()
