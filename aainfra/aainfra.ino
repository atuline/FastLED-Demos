
/*

AAINFRA FOR AALIGHT LED EFFECTS

       By: Andrew Tuline
      URL: www.tuline.com
    Email: atuline@gmail.com
   GitHub: https://github.com/atuline
  Youtube: https://www.youtube.com/user/atuline/videos
 Pastebin: http://pastebin.com/u/atuline


Introduction

This program is for an Arduino that receives IR signals, decodes them and serially transmits a command to an Arduino
that is controlling a WS2812B LED strip with FastLED. This is a MUCH more reliable method of using IR functionality than
with a single Arduino running FastLED.


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

#include <IRremote.h>

int RECV_PIN = 9;                                     // Standard IR pin and data setup
IRrecv irrecv(RECV_PIN);
decode_results results;
String command;
boolean understood = false;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();                                // Start the receiver
}

void loop() {

  getir();

} // loop()


void getir() {

 if (irrecv.decode(&results)) {                      // Read the IR Receiver
    understood = true;
    switch (results.value) {
                                                      // Button pressed
      case 16187647:  command = "r2";   break;        // a1
      case 16220287:  command = "r1";   break;        // a2
      case 16203967:  command = "m1";   break;        // a3
      case 16236607:  command = "m0";   break;        // a4

      case 16195807:  command = "m6";   break;        // b1
      case 16228447:  command = "m7";   break;        // b2
      case 16212127:  command = "m8";   break;        // b3
      case 16244767:  command = "m9";   break;        // b4

      case 16191727:  command = "m10";  break;        // c1
      case 16224367:  command = "e2";   break;        // c2
      case 16208047:  command = "e1";   break;        // c3
      case 16240687:  command = "m13";  break;        // c4

      case 16199887:  command = "m14";  break;        // d1
      case 16232527:  command = "o1";   break;        // d2
      case 16216207:  command = "o2";   break;        // d3
      case 16248847:  command = "m17";  break;        // d4

      case 16189687:  command = "m18";  break;        // e1
      case 16222327:  command = "n0";   break;        // e2
      case 16206007:  command = "n1";   break;        // e3
      case 16238647:  command = "m888"; break;        // e4

      case 16197847:  command = "m22";  break;        // f1
      case 16230487:  command = "u1";   break;        // f2
      case 16214167:  command = "u2";   break;        // f3
      case 16246807:  command = "m889"; break;        // f4

      default:        understood = false;             // We could do something by default
    } // switch

    if (understood) Serial.println(command);
    irrecv.resume();                                  // Receive the next value

  } // if irrecv
} // getir()