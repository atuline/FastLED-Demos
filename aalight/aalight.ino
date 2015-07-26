/*

aalight led lighting effects for FastLED.

      By: Andrew Tuline
     URL: www.tuline.com
   Email: atuline@gmail.com
  GitHub: https://github.com/atuline
Pastebin: http://pastebin.com/u/atuline
 Youtube: https://www.youtube.com/user/atuline/videos


Date: March, 2015


Introduction:

These effects/visualizations are designed for a single strand/loop of individually addressable LED's using the FastLED library.

Several modes of input are supported, such as local and remote IR support, a debounced button as well as keyboard. This also support for a demo mode.

This is built upon code from several sources, but mainly:

- FastLED library and some examples as found at http://fastled.io
- funkboxing LED for general layout as found at http://funkboxing.com/wordpress/?p=1366
- IR programmability as found at https://github.com/NicoHood/IRLremote


Hardware Setup

- 1 (or optionally 2) Arduino UNO or Nano 3.0.
- Pushbutton to pin 6 and gnd.
- WS2812B or (preferably) APA102 LED strip with data line connected to pin 12. 
- The clock line is connected to pin 11.
- Sparkfun INMP401 MEMS microphone (a mic+opamp) with power connected to Arduino 3.3V supply, output to A5 of Arduino.
- Connect Arduino 3.3V output to the AREF pin on the Arduino (for the 3.3V MEMS microphone).
- For IR functionality, connect the output of a TSOP34838 IR receiver to pin D2 on a Nano (other pins are 5V and Gnd)..


On a second Arduino (if using WS2812B's and want IR capability):

- TSOP34838 IR receiver with data connected to pin D2 (other pins are 5V and Gnd).
- Connect ground of the second Arduino to the first Arduino.
- Connect Tx of the second Arduino to Rx of first Arduino.
- Disconnect them when programming the Arduino's.

Microphone Note: If you use a different microphone, you will need to re-calculate the offset as well as amplitude variables. If
using a 5V microphone, then remove the AREF wire.

Compiling Note: When compiling or using the serial monitor, disconnect Tx/Rx between the Arduino's. Otherwise, you may damage one.



Power Supplies I've Used

- USB connection to computer for shorter strands.
- 7.4V battery pack (2 x 3.7V rechargeable 18650 batteries) for shorter strands connected to the power plug or Vin of the Arduino.
- USB based battery pack, again for shorter strands.
- 6V battery pack (4 x 'AA' batteries) for shorter strands connected to the power plug or Vin of the Arduino.
- 7.4 DC power supply connected to Arduino and DC-DC converter to provide 5V and >500mA to a medium length strip.
- Connect ALL grounds!!!!


Libraries Required

- FastLED library (get 3.1) from https://github.com/FastLED/FastLED
- Nico Hood's IRL library from https://github.com/NicoHood/IRLremote
- JChristensen's Button Library from https://github.com/JChristensen/Button



LED Strand Configuration

I'm currently using 20 APA102 LED's. You'll need to change the LED_TYPE, the NUM_LEDS's and LEDS.addLeds definition for different types of strands.
See the FastLED library examples with other types of strands.



IR Operation

You can use the IR either on the main Arduino if using APA102 or other 4 pin strips. In the case of WS2811 or WS2812's,
you will need to use a 2nd Arduino attached serially to the first.

Use of a second Arduino that receives IR input and serially transmits commands to the first Arduino is supported as follows:

- Select specific modes (such as mode 0, 1, 99)
- Change hue
- Change direction
- Change speed
- Select next/prev mode
- Change brightness

I'm using a 24 button IR remote like the ones provided with 5V LED strands from China. If you use a different one, you'll need
to map the codes to the modes in the getirl(); routine. I've provided getirl.ino to test this out with.



Push Button Operation

We are starting out in mode '99', or the demonstration mode. Pushing the button sets to mode '0' and will continue to step to the next mode.
A long push/release resets back to display mode 0.



If No Controls Are Available

If no controls are available, then the routine defaults to demonstration mode '99'.



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
m     Set mode (0-99)                     yes
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
- Add the IR receiver to the 2nd unit (on pin 9)
- Power up the Arduino's



IR Operation on 2nd Arduino

The following section provides a list of commands that are sent to the main Arduino.

I'm using a 24 pin IR controller from Aliexpress. IR button locations are defined letters corresponding to rows
and numbers corresponding to columns. A1 = top left, A4 = top right, and F4 = bottom right.

Serial is the string being sent over the Tx line to the Arduino performing the display.


Serial    Command                       IR Button location
------    --------                      ------------------
m98      Set mode 98                            F4
m99      Set mode 99                            E4
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



/*------------------------------------------------------------------------------------------
--------------------------------------- Start of variables ---------------------------------
------------------------------------------------------------------------------------------*/


#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.

#define AALIGHT_VERSION 100

#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.
#include "Button.h"                                           // Button library. Includes press, long press, double press detection.
#include "IRLremote.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// choose a valid PinInterrupt pin of your Arduino board
#define pinIR 2
#define IRL_BLOCKING true
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for WS2801, WS2811, WS2812B or APA102
#define LED_CK 11                                             // Serial clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102
// #define LED_TYPE WS2812B                                      // What kind of strip are you using?
#define NUM_LEDS 20                                            // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


int ledMode = 99;                                             // Starting mode is typically 0. Use 99 if no controls available. ###### CHANGE ME #########
int maxMode;                                                  // Maximum number of modes is set later.

// temporary variables to save latest IR input
uint8_t IRProtocol = 0;
uint16_t IRAddress = 0;
uint32_t IRCommand = 0;

// Pushbutton pin definition
const int buttonPin = 6;                                      // Digital pin used for debounced pushbutton

// int buttonState = 0;
// int lastButtonState = 0;
Button myBtn(buttonPin, true, true, 50);                      // Declare the button


// Microphone pin definition
#define MIC_PIN 5                                             // Analog (not digital) port for microphone
#define DC_OFFSET  32                                         // DC offset in mic signal - if unusure, leave 0


// Serial definition
#define SERIAL_BAUDRATE 57600                                 // Or 115200.
#define SERIAL_TIMEOUT 5

byte inbyte;                                                  // Serial input byte
int thisarg;                                                  // Serial input argument


// Generic variables
uint8_t thisdelay = 0;                                        // Standard delay
uint8_t thishue = 0;                                          // Standard hue
uint8_t thissat = 255;                                        // Standard saturation
int thisbright = 0;                                           // Standard brightness
uint8_t thisfade = 224;                                         // Standard fade rate
bool thisdir = 0;                                             // Standard direction


// Note that commented out variable are repeated.

// Matrix variables

bool huerot = 0;
uint8_t bgclr = 0;
uint8_t bgbri = 0;

// Two_sin variables-------------------------------------------------------------------------
uint8_t wavebright = 128;                                     // You can change the brightness of the waves/bars rolling across the screen. Best to make them not as bright as the sparkles.
//uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thathue = 140;                                        // You can change the starting hue for other wave.
uint8_t thisrot = 0;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t thatrot = 0;                                          // You can change how quickly the hue rotates for the other wave. Currently 0.
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
//bool thisdir = 0;                                             // A direction variable that will work with inputs.
int8_t thisspeed = 4;                                         // You can change the speed, and can use negative values.
int8_t thatspeed = 4;                                         // You can change the speed, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
int thisphase = 0;                                            // Phase change value gets calculated.
int thatphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
uint8_t thatcutoff = 192;                                     // You can change the cutoff value to display that wave. Lower value = longer wave.


// Twinkle variables -----------------------------------------------------------------------
int twinkrate = 100;


// One_sin variables -----------------------------------------------------------------------

// bool thisdir = 0;
// uint8_t bgclr = 0;
// uint8_t bgbri = 0;


// pop_fade variables ----------------------------------------------------------------------
int ranamount = 50;                                           // The higher the number, lowers the chance for a pixel to light up.
// uint8_t thisdelay = 50;                                       // Standard delay value.
bool boolcolours = 1;                                         // True = random, False = array of colours (up to 10)
uint8_t numcolours = 2;                                       // Number of colours in the array to display.
unsigned long colours[10] = {0xff0000, 0x00ff00, 0x0000ff, 0xffffff};  // Just assign the first 3 of these selectable colours.
uint8_t maxbar = 1;                                           // Maximum # of pixels to turn on in a row
uint8_t fadeval = 224;                                        // Fade rate


// three_sin variables ---------------------------------------------------------------------

int wave1=0;
int wave2=0;
int wave3=0;

uint8_t inc1 = 2;
uint8_t inc2 = 1;
uint8_t inc3 = -3;

uint8_t lvl1 = 80;
uint8_t lvl2 = 80;
uint8_t lvl3 = 80;

uint8_t mul1 = 20;
uint8_t mul2 = 25;
uint8_t mul3 = 22;

// rainbow_march variables -----------------------------------------------------------------

uint8_t deltahue = 1;                                         // Hue change between pixels.


// noise16 variables -----------------------------------------------------------------------

//uint8_t thisdelay = 20;                                       // A delay value for the sequence(s)
// bool thisdir = 0;                                          // We can reverse the direction.
uint32_t  x,hue_time;                                         // x & hue_time values
uint8_t octaves=2;       //2                                  // how many octaves to use for the brightness
uint8_t hue_octaves=3;   //3                                  // how many octaves to use for the hue
int xscale=57771;        //57771                              // the 'distance' between points on the x axis
uint32_t hxy = 43213;    //43213                              // not sure about this
int hue_scale=20;        //1                                  // the 'distance' between points for the hue noise
int hue_speed = 1;       //31                                 // how fast we move through hue noise
uint8_t x_speed = 0;     //331                                // adjust this value to move along the x axis between frames
int8_t hxyinc = 3;       //3

// uint8_t wavebright= 128;                                      // Usesd by qsub to set a fixed value to LED's depending on their current value

// Display functions

#include "matrix.h"
#include "noise16.h"
#include "one_sin.h"
#include "pop_fade.h"
#include "rainbow_march.h"
#include "three_sin.h"
#include "twinkle.h"
#include "two_sin.h"
#include "confetti.h"
#include "sinelon.h"
#include "juggle.h"
#include "demo_modeA.h"

/*------------------------------------------------------------------------------------------
--------------------------------------- Start of code --------------------------------------
------------------------------------------------------------------------------------------*/

void setup() {

  // This is used by the microphone and is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);                                  // MAKE SURE AREF IS CONNECTED TO 3.3V IF USING THE MEMS MICROPHONE.

  Serial.begin(SERIAL_BAUDRATE);                              // SETUP HARDWARE SERIAL (USB)
  Serial.setTimeout(SERIAL_TIMEOUT);

  attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_NEC>, CHANGE);    // IR definition
  
  LEDS.setBrightness(max_bright);                             // Set the generic maximum brightness value.

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER >(leds, NUM_LEDS); // WS2812B definition
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER >(leds, NUM_LEDS); // APA102 or WS2801 definition
  
  set_max_power_in_volts_and_milliamps(5, 500);               //5V, 500mA

  random16_set_seed(4832);                                    // Awesome randomizer
  random16_add_entropy(analogRead(2));
  int ranstart = random16();

  Serial.println("---SETUP COMPLETE---");
  change_mode(ledMode, 1);                                    // Initialize the first sequence
} // setup()



//------------------MAIN LOOP---------------------------------------------------------------
void loop() {
  getirl();
  readbutton();                                               // Button press increases the ledMode up to last contiguous mode and then starts over at 0.
  readkeyboard();                                             // Get keyboard input.
  change_mode(ledMode, 0);                                    // Strobe, don't set it.
  show_at_max_brightness_for_power();                         // Power managed display of LED's.
  delay_at_max_brightness_for_power(2.5*thisdelay);           // Power managed FastLED delay.
//  Serial.println(LEDS.getFPS());                            // Display frames per second in the serial monitor. Disable the delay in order to see how fast/efficient your sequence is.
} // loop()


void change_mode(int newMode, int mc){                        // mc stands for 'Mode Change', where mc = 0 is strobe the routine, while mc = 1 is change the routine

   maxMode = 41;
   if(mc) fill_solid(leds,NUM_LEDS,CRGB(0,0,0));              // Clean up the array for the first time through. Don't show display though, so you may have a smooth transition.

  switch (newMode) {                                          // First time through a new mode, so let's initialize the variables for a given display.

    case  0: if(mc) {fill_solid(leds,NUM_LEDS,CRGB(0,0,0)); LEDS.show();} LEDS.show(); break;              // All off, not animated.
    case  1: if(mc) {fill_solid(leds, NUM_LEDS,CRGB(255,255,255)); LEDS.show();} LEDS.show(); break;              // All on, not animated.
    case  2: if(mc) {thisdelay=20; twinkrate=NUM_LEDS; thishue=0; thissat=255; thisbright=255; thisfade=64; } twinkle(); break;
    case  3: if(mc) {thisdelay=10; thisrot=1; thatrot=1;} two_sin(); break;
    case  4: if(mc) {thisdelay=10; thisrot=0; thisdir=1;} two_sin(); break;
    case  5: if(mc) {thisdelay=10; thatrot=0; thishue=255; thathue=255;} two_sin(); break;
    case  6: if(mc) {thisdelay=10; allfreq=16; thathue=128;} two_sin(); break;
    case  7: if(mc) {thisdelay=10; thiscutoff=96; thishue=196; thatcutoff=240;} two_sin(); break;
    case  8: if(mc) {thisdelay=10; thiscutoff=96; thatcutoff=96; thisrot=1;} two_sin(); break;
    case  9: if(mc) {thisdelay=10; thisspeed= -4; thatspeed= -4;} two_sin(); break;
    case 10: if(mc) {thisdelay=10; thiscutoff=128; thatcutoff=128; wavebright=64;} two_sin(); break;
    case 11: if(mc) {thisdelay=10; wavebright=128; thisspeed=3;} two_sin(); break;
    case 12: if(mc) {thisdelay=10; thisspeed=3; thatspeed=-3;} two_sin(); break;
    case 13: if(mc) {thisdelay=30; thishue=95; bgclr=100; bgbri=10;} matrix(); break;
    case 14: if(mc) {thisdelay=30; thishue=40; thisdir=1; bgclr=75;} matrix(); break;
    case 15: if(mc) {thisdelay=30; thishue=random8(); huerot=1; bgbri=0;} matrix(); break;
    case 16: if(mc) {thisdelay=20; thisrot=1; thiscutoff=254; allfreq=8; bgclr=0; bgbri=10;} one_sin(); break;
    case 17: if(mc) {thisdelay=20; thisrot=0; bgclr=50;} one_sin(); break;
    case 18: if(mc) {thisdelay=20; thishue=255; bgclr=100; bgbri=40;} one_sin(); break;
    case 19: if(mc) {thisdelay=20; allfreq=16; bgbri=0;} one_sin(); break;
    case 20: if(mc) {thisdelay=20; thiscutoff=96; bgclr=200; bgbri=20;} one_sin(); break;
    case 21: if(mc) {thisdelay=20; thiscutoff=128; wavebright=64;} one_sin(); break;
    case 22: if(mc) {thisdelay=40; colours[0]=0xffffff; numcolours=1; boolcolours=0; maxbar=1;} pop_fade(); break;
    case 23: if(mc) {thisdelay=40; colours[1]=0xff0000; numcolours=2; boolcolours=0; maxbar=4;} pop_fade(); break;
    case 24: if(mc) {thisdelay=40; fadeval=192;} pop_fade(); break;
    case 25: if(mc) {thisdelay=40; boolcolours=1; maxbar=1;} pop_fade(); break;
    case 26: if(mc) {thisdelay=40; fadeval=128;} pop_fade(); break;
    case 27: if(mc) {thisdelay=40; colours[2]= 0x0000ff; boolcolours=0; numcolours=3; fadeval=192; maxbar=6;} pop_fade(); break;
    case 28: if(mc) {thisdelay=20; mul1=20; mul2=25; mul3=22;} three_sin(); break;
    case 29: if(mc) {thisdelay=20; mul1=5; mul2=8; mul3=7;} three_sin(); break;
    case 30: if(mc) {thisdelay=20; lvl1=220; lvl2=220; lvl3=220;} three_sin(); break;
    case 31: if(mc) {thisdelay=10; thisrot=1; deltahue=5;} rainbow_march(); break;
    case 32: if(mc) {thisdelay=10; thisdir=-1; deltahue=10;} rainbow_march(); break;
    case 33: if(mc) {thisdelay=10; thisrot=5;} rainbow_march(); break;
    case 34: if(mc) {thisdelay=10; thisrot=5; thisdir=-1; deltahue=20;} rainbow_march(); break;
    case 35: if(mc) {thisdelay=10; deltahue=30;} rainbow_march(); break;
    case 36: if(mc) {thisdelay=10; deltahue=2; thisrot=5;} rainbow_march(); break;
    case 37: if(mc) {thisdelay=20; octaves=1; hue_octaves=2; hxy=6000; x=5000; xscale=3000; hue_scale=50; hue_speed=15; x_speed=100;} noise16(); break;
    case 38: if(mc) {thisdelay=20; hxyinc = random16(1,15); octaves=random16(1,3); hue_octaves=random16(1,5); hue_scale=random16(10, 50);  x=random16(); xscale=random16(); hxy= random16(); hue_time=random16(); hue_speed=random16(1,3); x_speed=random16(1,30);} noise16(); break;
    case 39: if(mc) {thisdelay=20; thishue = 20;} confetti(); break;
    case 40: if(mc) {thisdelay=20; thishue = 50;} sinelon(); break;
    case 41: if(mc) {thisdelay=10;} juggle(); break;

    // DEMO MODE
    case 99: demo_modeA(); break;

  } // switch newMode

  ledMode = newMode;
} // change_mode()


//----------------- Hardware Support Functions ---------------------------------------------



void getirl() {                                               // This is the built-in IR function that just selects a mode.
  uint8_t oldSREG = SREG;
//  cli();
  if (IRProtocol) {                                              // This is the built-in IR function that just selects a mode.
    Serial.print("Command:");
    Serial.println(IRCommand);
    switch(IRCommand) {
      case 65280:  max_bright++;        break;                //a1 - max_bright++;
      case 65025:  max_bright--;        break;                //a2 - max_bright--;
      case 64770:  change_mode(1,1);    break;                //a3 - change_mode(1);
      case 64515:  change_mode(0,1);    break;                //a4 - change_mode(0);

      case 64260:  change_mode(5,1);    break;                //b1 - 
      case 64005:  change_mode(6,1);    break;                //b2 - 
      case 63750:  change_mode(7,1);    break;                //b3 - 
      case 63495:  change_mode(8,1);    break;                //b4 - 

      case 63240:  change_mode(9,1);    break;                //c1 - 
      case 62985:  thisdelay++;         break;                //c2 - thisdelay++;
      case 62730:  thisdelay--;         break;                //c3 - thisdelay--;
      case 62475:  change_mode(12,1);   break;                //c4

      case 62220:  change_mode(13,1);   break;                //d1
      case 61965:  ledMode--; change_mode(ledMode,1); break;  //d2 - change_mode(ledMode--);
      case 61710:  ledMode++; change_mode(ledMode,1); break;  //d3 - change_mode(ledMode++);
      case 61455:  change_mode(16,1);   break;                //d4

      case 61200:  change_mode(17,1);   break;                //e1 -
      case 60945:  thisdir = 1;         break;                //e2 - thisdir = 1;
      case 60690:  thisdir = 0;         break;                //e3 - thisdir = 0;
      case 60435:  change_mode(20,1);   break;                //e4

      case 60180:  change_mode(21,1);   break;                //f1
      case 59925:  thishue-=5;          break;                //f2 - thishue--;
      case 59670:  thishue+=5;          break;                //f3 - thishue++;
      case 59415:  change_mode(99,1);   break;                //f4

      default:                          break;                // We could do something by default
    } // switch
    // reset variable to not read the same value twice
    IRProtocol = 0;
  } // if IRLavailable()
  SREG = oldSREG;
} // getir()


void IREvent(uint8_t protocol, uint16_t address, uint32_t command) {
  // called when directly received a valid IR signal.
  // do not use Serial inside, it can crash your program!

  // dont update value if blocking is enabled
  if (IRL_BLOCKING && !IRProtocol) {
    // update the values to the newest valid input
    IRProtocol = protocol;
    IRAddress = address;
    IRCommand = command;
  }
}



void readkeyboard() {                                         // PROCESS HARDWARE SERIAL COMMANDS AND ARGS
  while (Serial.available() > 0) {
  
    inbyte = Serial.read();                                   // READ SINGLE BYTE COMMAND
    Serial.print("Serial read is: ");
    Serial.println(inbyte);
    switch(inbyte) {

      case 59: break;                                         // BREAK IF INBYTE = ';'

      case 97:                                                // "a" - SET ALL TO ONE COLOR BY HSV 0-255
        thisarg = Serial.parseInt();
        thissat = 255;
        thisbright = 255;
        fill_solid_HSV(thisarg, thissat, thisbright);
        break;

      case 98:                                                // "b" - SET MAX BRIGHTNESS to #
        max_bright = Serial.parseInt();
        LEDS.setBrightness(max_bright);
        break;

      case 99:                                                // "c" - CLEAR STRIP
        fill_solid(leds,NUM_LEDS,CRGB(0,0,0));
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

      case 109:                                               // "m" - SET MODE to #
        thisarg = Serial.parseInt();
        change_mode(thisarg, 1);
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
        change_mode(ledMode, 1);
        break;

      case 113:                                               // "q" - DISPLAY VERSION NUMBER
        Serial.print(AALIGHT_VERSION);
        break;

      case 114:                                               // "r1 or r2" - DECREASE OR INCREASE BRIGHTNESS by / or * 2
        thisarg = Serial.parseInt();
        if (thisarg == 1) max_bright=max_bright/2; else max_bright=max_bright*2;
        max_bright = constrain(max_bright, 1, 255);
        LEDS.setBrightness(max_bright);
        show_at_max_brightness_for_power();
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

      case 122:                                               // "z" - COMMAND TO 'SHOW' LEDS
        show_at_max_brightness_for_power();
        break;
    } // switch inbyte
  } // while Serial.available
} // readkeyboard()


void readbutton() {                                           // Read the button and increase the mode
  myBtn.read();
  if(myBtn.wasReleased()) {
    ledMode = ledMode > maxMode ? 0 : ledMode+1;              // Reset to 0 only during a mode change
    change_mode(ledMode, 1);
  }
  if(myBtn.pressedFor(1000)) {
    ledMode = 255;
    change_mode(ledMode, 1);
  }
} // readbutton()


//---------------------- LED Utility Functions ---------------------------------------------
 
int wrap(int step) {
  if(step < 0) return NUM_LEDS + step;
  if(step > NUM_LEDS - 1) return step - NUM_LEDS;
  return step;
} // wrap()



void fill_solid_HSV(uint8_t ahue, uint8_t asat, uint8_t abright) {  // Set all LED's to an HSV value.
  for(int i = 0 ; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(ahue, asat, abright);
  }
}  // fill_solid_HSV()
