/*

AALIGHT LED EFFECTS

      By: Andrew Tuline
     URL: www.tuline.com
   Email: atuline@gmail.com
  GitHub: https://github.com/atuline
Pastebin: http://pastebin.com/u/atuline
 Youtube: https://www.youtube.com/user/atuline/videos


Date: September, 2014


Introduction:

These limited number of effects/visualizations are designed for a single strand/loop of individually addressable WS2801 or WS2812 LED's using the
FastLED library. The framework is based on the funkboxing LED project.

Several modes of input are supported, such as local and remote IR support, a debounced button as well as keyboard. This also support for a demo mode,
however it could be improved by converting it to be time based, rather than loop based.

This is built upon code from several sources, but mainly:

- funkboxing LED effects for FastLED (the main structure of this program + a few effects)
- http://industriumvita.com/arduino-ir-remote-controled-ws2811-addressable-leds/



Hardware Setup

- 1 (or optionally 2) Arduino UNO or Nano 3.0 (is what I've been using).
- For a hardware debounced pushbutton, see the next few lines.
- Connect pin '0' of the pushbutton to pin 6 of UNO.
- One side of 10K resistor connected to pin '0' of pushbutton. Other side connected to 5V.
- Positive side of 1uf capacitor connected to pin '0' of pushbutton. Other side connected to gnd.
- Pushbutton pin '1' connected to gnd.
- (Code has been commented out for this). TSOP34838 IR receiver with data connected to pin 9 (other pins are 5V and Gnd).
- WS2812B (or WS2801) LED strip with data line connected to pin 13 (other pins are connected to Vin and Gnd of the Arduino).
- WS2801 clock would be on pin 11.
- Sparkfun INMP401 MEMS microphone (a mic+opamp) with power connected to Arduino 3.3V supply, output to A5 of Arduino.
- Connect Arduino 3.3V output to the AREF pin on the Arduino (for the 3.3V MEMS microphone).

On a second Arduino:

- TSOP34838 IR receiver with data connected to pin 9 (other pins are 5V and Gnd).
- Connect ground of the second Arduino to the first Arduino.
- Connect Tx of the second Arduino to Rx of first Arduino.

Microphone Note: If you use a different microphone, you will need to re-calculate the offset as well as amplitude variables. If
it's a 5V microphone, then remove the AREF wire.

Compiling Note: When compiling or using the serial monitor, disconnect Tx/Rx between the Arduino's.



Power Supplies Tested

- USB connection to computer (for <24 LED strands)
- 6VDC adapter. Be careful, as some are noisy and may damage the LED strip (been there, done that).
- 7.4V battery pack (2 x 3.7V rechargeable 18650 batteries)
- 6V battery pack (4 x 'AA' batteries)

Note: If the LED strip is plugged into Vin (or directly to the power supply), I wouldn't recommend going above 7.4V. I have blown up several
LED's by providing too high of a voltage with noisy regulators.



Libraries Required

- FastLED library (v2.1) from https://github.com/FastLED/FastLED/tree/FastLED2.1
- Ken Shiriff's IR library from https://github.com/shirriff/Arduino-IRremote



LED Strand Configuration

This is currently configured for 24 LED's. Change the definition of NUM_LEDS to match the length of your strand.

I'm currently using WS2812B LED's. You'll need to change the LEDS.addLeds definition for different types of strands. See the FastLED
library examples with other types of strands.



IR Operation

Use of a second Arduino that receives IR input and serially transmits commands to the first Arduino is supported. In addition, it also
provides a lot more functionality, such as:

- Select specific modes (such as mode 0, 1, 888)
- Change hue
- Change direction
- Change speed
- Select next/prev mode
- Change brightness

I'm using a 24 button IR remote like the ones provided with 5V LED strands from China. If you use a different one, you'll need
to map the codes to the modes in the getir(); routine.



Push Button Operation

If connected, the physical button should always work and selects the next display mode, up to the last contiguous display mode and then
loops back to 0. It does not support test, sound responsive or demo modes.



If No Controls Are Available

If no controls are available, then it is recommended that you set the starting mode to the demo mode '888', which is the standard one.



Keyboard Operation

Once you have compiled the source code, you can open up the monitor and enter keyboard commands. Keyboard commands in the monitor mode include:


Key   Command                         Do I use this
---   -------                         -------------
;     Break                               no
a     Set all to one color by hsv (0-255) yes
b     Set maximum brightness (0-255)      yes
c     clear strip                         no
d     Set delay variable (0-??)           yes
e     Increase/decrease delay (1/2)       yes
h     Set hue variable (0-255)            yes
l     Set single LED value rgb            no  (commented out)
m     Set mode (0-??)                     yes
n     Reverse/forward direction (1/2)     yes
o     Decrease/increase mode (1/2)        yes
Q     Return version number               no
r     Decrease/increase brightness (1/2)  yes
s     Set step variable (0-?)             yes
t     Set saturation variable (0-?)       yes
u     Decrease/increase hue (1/2)         yes
v     Set single LED value hsv (??)       no (commented out)
y     set of LED's in display and save    Not yet implemented
z     Show LED's                          no



Examples:

m5              // Select mode 5 (with or without spaces separating the command and the argument)
h 80            // Select a hue of 80, which will change the hue of some of the modes



Second Arduino Support

Since 3 pin strands using FastLED do not work IR functionality (when animating), a second Arduino is used to pass
serial commands to the main unit as if they were keyboard commands (see above). To use this functionality:

- Compile aainfra.ino on the 2nd unit (disconnect Tx and Rx between the Arduino's before doing so)
- Connect the grounds together
- Connect Tx of the 2nd unit to Rx of the main unit
- Remove the IR receiver from the main unit
- Add the IR receiver to the 2nd unit (on pin 9)
- Power up the Arduino's



IR Operation on 2nd Arduino

The following section provides a list of commands that are sent to the main Arduino.

I'm using a 24 pin IR controller from Aliexpress. IR button locations are defined letters corresponding to rows
and numbers corresponding to columns. A1 = top left, and F4 = bottom right.

Serial is the string being sent over the Tx line to the Arduino performing the display.


Serial    Command                       IR Button location
------    --------                      ------------------
m889      Set mode 889                          F4
m888      Set mode 888                          E4
m1        Set mode 1                            A3
m0        Set mode 0                            A4
e1, e2    Increase/Decrease delay               C2, C3
o1, o2    Decrease/Increase mode                D2, D3
r1, r2    Increase/Decrease brightness          A1, A2
u1, u2    Decrease/Increase hue                 F2, F3

n0, n1    Direction reverse/forward             E2, E3
??        Increase/decrease of LED's            B1, with A1, A2        // Not yet implemented
y         Save of LED's to flash                B1                     // Not yet implemented

*/


/*
---------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------- Start of code --------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------
*/

#include <FastLED.h>                                            // FastLED library

#define VERSION_NUMBER 1.0


// PUSHBUTTON SETUP STUFF
const int buttonPin = 6;    					// Digital pin used for debounced pushbutton
int buttonState = 0;
int lastButtonState = 0;


// MICROPHONE SETUP STUFF
#define MIC_PIN 5           					// Analog (not digital) port for microphone

#define DC_OFFSET  32                 // DC offset in mic signal - if unusure, leave 0



// SERIAL SETUP STUFF
#define SERIAL_BAUDRATE 9600  				// Or 57600 or 115200. I'm just old school.
#define SERIAL_TIMEOUT 5

byte inbyte;                  				// Serial input byte
int thisarg;                  				// Serial input argument


// LED SETUP STUFF
#define NUM_LEDS 24           				// Number of LED's ###################################################### CHANGE ME #########
#define LED_DT 13              				// Serial data pin for WS2812B or WS2801
#define LED_CK 11              				// Serial clock pin for WS2801 only
#define COLOR_ORDER GRB       				// Are they RGB, GRB or what??
#define LED_TYPE WS2811               // What kind of strip are you using?

int ledMode = 0;                      // Starting mode is typically 0. Use 888 if no controls available. ###### CHANGE ME #########

struct CRGB leds[NUM_LEDS];    				// Main definition of the LED array

int BOTTOM_INDEX = 0;         				// These are used by the funkboxing code for emslightsONE
int TOP_INDEX = int(NUM_LEDS/2);
int EVENODD = NUM_LEDS%2;


// Funkboxing variables. You can change some values via the serial monitor.
int thisdelay = 0;                    // FX LOOPS DELAY VAR
int thisstep = 10;                    // FX LOOPS DELAY VAR
int thishue = 0;                      // FX LOOPS COLOR VAR
int thissat = 255;                    // FX LOOPS COLOR VAR
int thisbright = 0;                   // FX LOOPS COLOR VAR
bool thisdir = 0;                     // FX LOOPS DIRECTION VAR
int max_bright = 128;                 // SET MAX BRIGHTNESS TO 1/4

int thisindex = 0;                    // SET SINGLE LED VAR
int thisRED = 0;
int thisGRN = 0;
int thisBLU = 0;
int idex = 0;                         // LED INDEX (0 to NUM_LEDS-1)

int thisbounce = 0;                  // Bounce direction variable.


//Noise Variables
uint32_t x,hue_time, hxy;
int xscale, hue_scale, hue_speed;
uint8_t x_speed, octaves, hue_octaves;



// -----------------SETUP------------------
void setup() {

  // This is used by the microphone and is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);             											// MAKE SURE AREF IS CONNECTED TO 3.3V IF USING THE MEMS MICROPHONE.

  Serial.begin(SERIAL_BAUDRATE);        											// SETUP HARDWARE SERIAL (USB)
  Serial.setTimeout(SERIAL_TIMEOUT);

  pinMode(buttonPin, INPUT);            											// Pushbutton (used to select next mode)

  LEDS.setBrightness(max_bright);       											// Set the generic maximum brightness value.

//  LEDS.addLeds<WS2801, LED_CK, LED_DT, BGR, DATA_RATE_MHZ(1)>(leds, NUM_LEDS); 	 // WS2801 definition
	LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER >(leds, NUM_LEDS); // WS2811 definition

  set_max_power_in_volts_and_milliamps(5, 2000); 						//5v 500mA

  Serial.println("---SETUP COMPLETE---");

  change_mode(ledMode);                  											// Initialize the first sequence
  show_at_max_brightness_for_power();

} // setup()


//------------------MAIN LOOP------------------
void loop() {

  readbutton();                  // Button press increases the ledMode up to last contiguous mode and then starts over at 0.

  switch (ledMode) {              // Looping through (and not initializing) the current mode. Don't initialize variables here, as we are just going through loop again.
    case 999: break;
    case   0: break;              // ALL OFF
    case   1: break;              // ALL ON
    case   2: rainbow_fade(); break;
    case   3: rainbow_march(); break;
    case   4: random_color_pop(); break;
    case   5: ems_lightsONE(); break;
    case   6: pulse_one_color_all(); break;
    case   7: twinkle();break;
    case   8: mynoise(); break;
    case   9: rainbow_vertical(); break;                      

    // TEST MODES

    // SOUND RESPONSIVE MODES
    case  20: memsmic(); break;

    // DEMO MODES
    case 888: demo_modeA(); break;                            // This is the standard demo mode.

  } // switch


  // PROCESS HARDWARE SERIAL COMMANDS AND ARGS
  while (Serial.available() > 0) {
  
    inbyte = Serial.read();                                   // READ SINGLE BYTE COMMAND
    Serial.print("Serial read is: ");
    Serial.println(inbyte);
    switch(inbyte) {

      case 59: break;                                         // BREAK IF INBYTE = ';'

      case 97:                                                // "a" - SET ALL TO ONE COLOR BY HSV 0-255
        thisarg = Serial.parseInt();
        thisbright = 255;
        one_color_allHSV(thisarg, thisbright);
        break;

      case 98:                                                // "b" - SET MAX BRIGHTNESS to #
        max_bright = Serial.parseInt();
        LEDS.setBrightness(max_bright);
        break;

      case 99:                                                // "c" - CLEAR STRIP
        one_color_all(0,0,0);
        break;

      case 100:                                               // "d" - SET DELAY VAR to #
        thisarg = Serial.parseInt();
        thisdelay = thisarg;
        break;

      case 101:                                               // "e1 or e2" - DECREASE OR INCREASE delay by 1
        thisarg = Serial.parseInt();
        if (thisarg == 1) thisdelay--; else thisdelay++;
        if (thisdelay < 0 ) thisdelay = 0;
        break;

      case 104:                                               // "h" - SET HUE VAR to #
        thisarg = Serial.parseInt();
        thishue = thisarg;
        break;

/*      case 108:                                               // "l" - SET SINGLE LED VALUE RGB
        thisindex = Serial.parseInt();
        thisRED = Serial.parseInt();
        thisGRN = Serial.parseInt();
        thisBLU = Serial.parseInt();
        if (ledMode != 999) {
          ledMode = 999;
          one_color_all(0,0,0);}
          leds[thisindex].setRGB( thisRED, thisGRN, thisBLU);
        break;
*/

      case 109:                                               // "m" - SET MODE to #
        thisarg = Serial.parseInt();
        change_mode(thisarg);
        break;

      case 110:                                               // "n0 or n1" - DIRECTION is 0 or 1
        thisarg = Serial.parseInt();
        if (thisarg <=1 && thisarg >=0 ) thisdir = thisarg;
        break;

      case 111:                                               // "o1 or o2" - DECREASE OR INCREASE MODE by 1
        thisarg = Serial.parseInt();
        if (thisarg == 1) ledMode--;
        if (thisarg == 2) ledMode++;
        if (ledMode < 0) ledMode = 0;
        change_mode(ledMode);
        break;

      case 113:                                               // "q" - DISPLAY VERSION NUMBER
        Serial.print(VERSION_NUMBER);
        break;

      case 114:                                               // "r1 or r2" - DECREASE OR INCREASE BRIGHTNESS by / or * 2
        thisarg = Serial.parseInt();
        if (thisarg == 1) max_bright=max_bright/2; else max_bright=max_bright*2;
        max_bright = constrain(max_bright, 1, 255);
/*      if (max_bright == 0) max_bright=1;
        if (max_bright > 255) max_bright=255; */
        LEDS.setBrightness(max_bright);
        show_at_max_brightness_for_power();
        break;

      case 115:                                               // "s" - SET STEP VAR to #
        thisarg = Serial.parseInt();
        thisstep = thisarg;
        break;

      case 116:                                               // "t" - SET SATURATION VAR to #
        thisarg = Serial.parseInt();
        thissat = thisarg;
        break;

      case 117:                                               // "u1 or u2" - DECREASE OR INCREASE HUE by 8
        thisarg = Serial.parseInt();                          // Only works in some CHSV modes
        if (thisarg == 1) thishue=thishue-8; else thishue=thishue+8;
        show_at_max_brightness_for_power();
        break;

/*      case 118:                                               // "v" - SET SINGLE LED VALUE HSV
        thisindex = Serial.parseInt();
        thishue = Serial.parseInt();
        thissat = Serial.parseInt();
        //thisVAL = Serial.parseInt();
        if (ledMode != 999) {
          ledMode = 999;
          one_color_all(0,0,0);
        }
        leds[thisindex] = CHSV(thishue, thissat, 255);
        break;
*/
      case 122:                                               // "z" - COMMAND TO 'SHOW' LEDS
        show_at_max_brightness_for_power();
        break;

    } // switch inbyte
  } // while Serial.available
} // loop()


void change_mode(int newmode){
  one_color_all(0,0,0);                          // Clean up the array for the first time through. Don't show display though, so you may have a smooth transition.

  switch (newmode) {                             // First time through a new mode, so let's initialize the variables for a given display.

    case  0: one_color_all(0,0,0); LEDS.show(); break;              // ALL OFF, NOT ANIMATED
    case  1: one_color_all(255,255,255); LEDS.show(); break;        // ALL ON, NOT ANIMATED
    case  2: thisdelay=20; break;                                   // STRIP RAINBOW FADE
    case  3: thisdir=0; thisdelay=5; break;                         // RAINBOW MARCH
    case  4: thisdelay=20; break;                                   // RANDOM COLOR POP
    case  5: thisdir=0; thisdelay=40; thishue=0; break;             // EMS LIGHTS ONE
    case  6: thisbounce=0; thisdelay=5; thishue=90; break;          // PULSE COLOR BRIGHTNESS
    case  7: thishue=50; thisdelay=8; break;                        // TWINKLE
    case  8: thisdir=0; thisdelay=20; hxy=4630; octaves=4; hue_octaves=2; xscale=30; hue_scale=50; hue_speed=5; x_speed=5; break;       // NOISY
    case  9: thisdelay=50; thisstep=15; break;                      // RAINBOW VERTICAL

    // TEST MODES

    // SOUND ENABLED MODES
    case 20: break;                                                 // MEMSMIC

    // DEMO MODES
    case 888: break;                                                // Standard demos (no sound)

  } // switch

  ledMode = newmode;
  Serial.print("Mode is: ");
  Serial.println(ledMode);
} // change_mode()




//----------------- Hardware Support Functions -----------------------------

void readbutton() {                            								// Read the button and increase the mode
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      ledMode = ledMode > 8 ? 0 : ledMode+1;        					// Reset to 0 only during a mode change
      change_mode(ledMode);
    }
  }
  lastButtonState = buttonState;
}

//---------------------- LED Utility Functions -----------------


// FIND INDEX OF HORIZONAL OPPOSITE LED
int horizontal_index(int i) {
  // ONLY WORKS WITH INDEX < TOPINDEX
  if (i == BOTTOM_INDEX) {return BOTTOM_INDEX;}
  if (i == TOP_INDEX && EVENODD == 1) {return TOP_INDEX + 1;}
  if (i == TOP_INDEX && EVENODD == 0) {return TOP_INDEX;}
  return NUM_LEDS - i;
}

// FIND INDEX OF ANTIPODAL OPPOSITE LED
  int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {iN = ( i + TOP_INDEX ) % NUM_LEDS; }
  return iN;
}


void drawIntegerBar( int intpos, int width, uint8_t hue)
{
  int i = intpos; // start drawing at "I"
  for( int n = 0; n < width; n++) {
    leds[i] += CHSV( hue, 255, 255);
    i++;
    if( i == NUM_LEDS) i = 0; // wrap around
  }
}

 
int wrap(int step) {
  if(step < 0) return NUM_LEDS + step;
  if(step > NUM_LEDS - 1) return step - NUM_LEDS;
  return step;
}
