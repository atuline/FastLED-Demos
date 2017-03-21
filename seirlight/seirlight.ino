/*
 * seirlight led lighting effects for FastLED.
 * 
 *       By: Andrew Tuline
 *    Date: March, 2017
 *      URL: www.tuline.com
 *    Email: atuline@gmail.com
 *   GitHub: https://github.com/atuline
 * Pastebin: http://pastebin.com/u/atuline
 *  Youtube: https://www.youtube.com/user/atuline/videos
 * 
 * 
 * CAUTION ************************************************************************************************************************************************
 * 
 * Before attempting to compile this routine, make sure you are already comfortable modifying Arduino Code and FastLED code in particular. In addition, you
 * should already be able to download, install and use 3rd party libraries. If you are a beginner, this is NOT the code you're looking for.
 * 
 * ********************************************************************************************************************************************************
 * 
 * 
 * Introduction
 * 
 * This is a significant re-write of my previous aalight program and provides the following:
 * 
 * - Uses an Arduino microcontroller.
 * - Uses the FastLED display library.
 * - Supports multiple display sequences, most with support for multiple settings.
 * - Supports IR (Infra Red) communications for control of the display sequences.
 * - Supports keyboard communications for control of the display sequences.
 * - Supports a button for 3 functions.
 * - Can support APA102 and WS2801 with IR communications.
 * - Can support WS2812 if NOT using keyboard or IR communications.
 * - Can save information in EEPROM.
 * 
 * 
 * Most of these effects assign colours via palettes as opposed to direct CRGB or CHSV assignments. This allows you to swap out colours without having to modify the
 * underlying effects code. In addition, each of the effects contain very little code (the longest is about 12 lines) and do not employ nested loops or blocking delays.
 * 
 * 
 * 
 * 1.02 Update
 * 
 * - Modify 'Set Strand ACTIVE/INACTIVE so that it doesn't halt the display of any of the Arduino's.
 * 
 * 1.01 Update
 * 
 * - Small updates/changes.
 * - Fix StrandID code.
 * 
 * 
 * 1.00 Updates/changes from aalight:
 * 
 * - Improve the brightness controls.
 * - Removed most of the pixel counting or lengthy routines.
 * - Converted all but the rainbow march sequences for palette support.
 * - Removed push button and keyboard support. Just focusing on IR support (for 4 pin strands only).
 * - Removing some IR keypresses, but adding some cool new ones.
 * - Combine mc demo modes and demo_modeA into a single set of demos.
 * - Fixed mode rotation along with maximum number of modes for the demo.
 * - Add eeprom starting mode support.
 * - Add support for multiple Arduino's, each driving a strand of LED's. This is done with a strandID variable.
 * - Add eeprom length of strand support (for each strand).
 * - Add glitter with an IR button press.
 * - Add palette transitioning.
 * - Cleanup/localize the variables as much as reasonably possible.
 * - Review/fix timing of each routine.
 * - Add ALL required variables for each display mode.
 * - Add ability to activate ALL of your strands at once (B4, B4).
 * - Program the random/semi-random palette selection and support fix/on the fly changes.
 * - Add the real routines with palettes.
 * - Fix demo mode timing, and increase from 5 to 10 seconds.
 * - Fixed routine timing issues with EVERY_N_MILLIS.
 * - Review routines for possible inclusion/removal.
 * - Final QA test, with an emphasis of "I'm so done with this".
 * 
 * 
 * Possible Future upgrades:
 * 
 * - Set a common timebase for multiple Arduino's so they can be 'in sync'.
 * 
 * 
 * 
 * 
 * Normal operation:
 * 
 * - Configure and compile seirlight for your type of strand, and pins used (WS2812's don't work with IR and are very unreliable with Keyboard).
 * - Use the keyboard or IR controller to modify and save the strand length (up to 100) to support your actual strand.
 * - Set the demorun variable to 0 and compile seirlight, so that the strand does not startup in demo mode.
 * - As a security measure, set strandActive to 0 and recompile the sketch. Your strand is now inactive until selected with the buttons below, i.e. B1, then A1 are the defaults.
 * - If you want to support multiple strands, set the strandId for each to match the output of the various buttons on the IR controller (except for B1).
 * 
 * You'll need to select your strand before it will display anything, which is a nice security feature if someone steals your display.
 * 
 * 
 * 
 * Hardware Setup
 * 
 * - The LED data line is connected to pin 12 (changeable).
 * - The LED clock line is connected to pin 11 (changeable).
 * - The button currently uses pin 6.
 * - For IR functionality, connect the data output of a TSOP34838 IR receiver to pin D2 (other TSOP pins are 5V and Gnd).
 * - If you use WS2812 LED's, then the IR will NOT work while the keyboard is unreliable. See https://github.com/FastLED/FastLED/wiki/Interrupt-problems
 * - This supports up to 23 strands with up to 100 LED's each currently. You can increase this value by changing MAX_LEDS.
 * 
 * 
 * EEPROM Functionality
 * 
 * - We can save the startup display mode in EEPROM.
 * - We can save the strand length in EEPROM, however the MAXIMUM strand length is pre-defined. We just use lesser value for the array.
 * 
 * 
 * Multiple Arduino/strand support
 * 
 * - We can support multiple strands with a strandID.
 * - We do not save the demo flag in EEPROM. That's modified at compile time.
 * - Only the IR control supports multiple strands. Keyboard does not.

 * 
 * Libraries Required
 * 
 * - FastLED library from https://github.com/FastLED/FastLED
 * - Nico Hood's IRL library from https://github.com/NicoHood/IRLremote
 * 
 * 
 * Button Operation
 * 
 * Click          Stop demo mode and advance to the next display mode.
 * Double-click   Stop demo mode and set to mode 0.
 * Long hold      Stop demo mode and write current mode to EEPROM as startup mode.
 * 
 * 
 * IR Operation
 * 
 * I'm using a 24 button IR remote like the ones provided with 5V LED strands from China. If you use a different one, you'll need to map
 * the codes to the modes in the irtest(); routine. I've provided irtest.ino to test this out with. The chart below summarizes the commands.
 * Button location uses characters as the row, and numbers are the columns, so C2 is the 3rd row, 2nd column.
 * 
 * 
 * Command                               IR Button location
 * --------                              ------------------
 * Increase brightness                   A1
 * Decrease brightness                   A2
 * Set mode 0 (black)                    A3  Disables the demo mode.
 * Toggle demo mode (millis based)       A4  It cycles through the routines based on the millis() counter.
 * 
 * Select Arduino                        B1  Then press A1 through F4. A1 is the first one. Press B4 twice to activate ALL Arduino's. Corresponds to strandId value. 
 * Decrease strand length                B2  The # of LED's programmed are white.
 * Increase strand length                B3  The # of LED's programmed are white.
 * Save strand length to EEPROM          B4  Press this once you have your strand length.
 * 
 * Enable/disable glitter                D1  Toggles glitter.
 * Decrease display mode                 D2  Also disables demo mode.
 * Increase display mode                 D3  Also disables demo mode.
 * Save Current mode to EEPROM           D4  This will be the startup mode.
 * 
 *                                       E1
 * Direction reverse                     E2
 * Direction forward                     E3
 *                                       E4
 * 
 * No palette change                     F1   Current palette stays fixed.
 * Random Palette 4 similar colours      F2   On the fly palette changes every 5 seconds.
 * Random Palette 4                      F3   On the fly palette changes every 5 seconds.
 * Random Palette 16                     F4   On the fly palette changes every 5 seconds.
 * 
 * 
 * 
 * Keyboard Operation
 * 
 * Once you have compiled the source code, you can open up the monitor and enter keyboard commands. Keyboard commands in the monitor mode include:
 * 
 * 
 * Key  Description                        Arguments                Notes
 * ---  -----------                        ---------                -----------------
 * a    Set all to one colour by hue       0-255
 * b    Set brightness                     0-255
 * c    clear strip (set mode 0)           n/a
 * d    Set delay variable                 0-255                    10 is a good value.
 * e    Set display mode previous/next     0/1                      Previous = 0, Next = 1.
 * f    Set fixed palette mode (0 to max)  0-255                    It's actually modded with gGradientPaletteCount
 * g    Glitter toggle                     n/a                      Toggles on/off.
 * h    Set hue variable                   0-255                    Doesn't do much as most routines no longer use HSV, but rather use palettes. It's here if you need it.
 * i    Similar palette hue                0-255                    Set a palette with colours similar to the colour selected.
 * l    Set strip length & write EEPROM    1-255
 * m    Set display mode                   0-255                    It's actually modded with maxMode
 * n    Direction toggle                   n/a                      Toggles direction for SOME of the routines, like Matrix and one_sin.
 * p    Play mode (fix, seq, shuf)         0-2
 * q    Return version number              n/a
 * s    Set saturation variable            0-255                    Doesn't do much as most routines no longer use HSV, but rather use palettes.
 * t    Select palette mode                0 - 3
 * u    Set sequence duration              1-255
 * w    Write current mode to EEPROM       n/a
 * 
 * Keyboard Examples:
 * 
 * m5              // Select mode 5 (with or without spaces separating the command and the argument)
 * h 80            // Select a hue of 80, which will change the hue of a few modes
 * 
 * 
 */


/*------------------------------------------------------------------------------------------
--------------------------------------- Start of variables ---------------------------------
------------------------------------------------------------------------------------------*/


#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.

#define SEIRLIGHT_VERSION 102


#define buttonPin 6                                           // input pin to use as a digital input
#include "jsbutton.h"                                         // Nice button routine by Jeff Saltzman and is already included here.

#include "FastLED.h"                                          // https://github.com/FastLED/FastLED
#include "IRLremote.h"                                        // https://github.com/NicoHood/IRLremote
#include "EEPROM.h"                                           // This is included with base install


#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// choose a valid PinInterrupt pin of your Arduino board
#define pinIR 2                                               // I'm using pin D2
#define IRL_BLOCKING true

// Serial definition
#define SERIAL_BAUDRATE 57600                                 // Or 115200.
#define SERIAL_TIMEOUT 5

// Serial variables
byte inbyte;                                                  // Serial input byte
int thisarg;                                                  // Serial input argument

// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for all strands
#define LED_CK 11                                             // Serial clock pin for WS2801 or APA102
#define COLOR_ORDER BGR                                       // It's GRB for WS2812
#define LED_TYPE APA102                                       // Alternatively WS2801, or WS2812
#define MAX_LEDS 100                                          // Maximum number of LED's defined (at compile time).


// Initialize changeable global variables.
uint8_t NUM_LEDS;                                             // Number of LED's we're actually using, and we can change this only the fly for the strand length.

uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[MAX_LEDS];                                   // Initialize our LED array.

CRGBPalette16 currentPalette;                                 // Use palettes instead of direct CHSV or CRGB assignments
CRGBPalette16 targetPalette;                                  // Also support smooth palette transitioning
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND


// EEPROM location definitions.
#define STARTMODE 0
#define STRANDLEN 1


const uint32_t STRANDID = 65280;                              // This is the same as button A1 and is the id of THIS strand. Change to a different button press as required.
bool strandActive = 0;                                        // 0=inactive, 1=active. Must be activated by button press of B1, then A1 (or the appropriate button).
bool strandFlag = 0;                                          // Flag to let us know if we're changing the active strand.

uint8_t ledMode;                                              // Starting mode is typically 0.
uint8_t demorun = 0;                                          // 0 = regular mode, 1 = demo mode, 2 = shuffle mode.
uint8_t maxMode = 38;                                         // Maximum number of modes.
uint8_t demotime = 10;                                        // Set the length of the demo timer.


// temporary variables to save latest IR input
uint8_t IRProtocol = 0;
uint16_t IRAddress = 0;
uint32_t IRCommand = 0;



// Generic/shared routine variables ----------------------------------------------------------------------
uint8_t allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
uint8_t bgclr = 0;                                            // Generic background colour
uint8_t bgbri = 0;                                            // Generic background brightness
bool    glitter = 0;                                          // Glitter flag
uint8_t palchg;                                               // 0=no change, 1=similar, 2=random
uint8_t startindex = 0;
uint8_t thisbeat;                                             // Standard beat
uint8_t thisbright = 0;                                       // Standard brightness
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
int thisdelay = 0;                                            // Standard delay
uint8_t thisdiff = 1;                                         // Standard palette jump
bool    thisdir = 0;                                          // Standard direction
uint8_t thisfade = 224;                                       // Standard fade rate
uint8_t thishue = 0;                                          // Standard hue
uint8_t thisindex = 0;                                        // Standard palette index
uint8_t thisinc = 1;                                          // Standard incrementer
int     thisphase = 0;                                        // Standard phase change
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t thissat = 255;                                        // Standard saturation
int8_t  thisspeed = 4;                                        // Standard speed change
uint8_t wavebright = 255;                                     // You can change the brightness of the waves/bars rolling across the screen.

uint8_t xd[MAX_LEDS];                                         // arrays for the 2d coordinates of any led
uint8_t yd[MAX_LEDS];

long summ=0;



extern const TProgmemRGBGradientPalettePtr gGradientPalettes[]; // These are for the fixed palettes in gradient_palettes.h
extern const uint8_t gGradientPaletteCount;                     // Total number of fixed palettes to display.
uint8_t gCurrentPaletteNumber = 0;                              // Current palette number from the 'playlist' of color palettes
uint8_t currentPatternIndex = 0;                                // Index number of which pattern is current



// Display functions -----------------------------------------------------------------------

// Support functions
#include "addglitter.h"
#include "make_palettes.h"

// Display routines
#include "circnoise_pal_1.h"
#include "circnoise_pal_2.h"
#include "circnoise_pal_3.h"
#include "circnoise_pal_4.h"
#include "confetti_pal.h"
#include "gradient_palettes.h"
#include "juggle_pal.h"
#include "matrix_pal.h"
#include "noise16_pal.h"
#include "noise8_pal.h"
#include "one_sin_pal.h"
#include "rainbow_march.h"
#include "serendipitous_pal.h"
#include "three_sin_pal.h"
#include "two_sin.h"





/*------------------------------------------------------------------------------------------
--------------------------------------- Start of code --------------------------------------
------------------------------------------------------------------------------------------*/


void setup() {


  Serial.begin(SERIAL_BAUDRATE);                              // SETUP HARDWARE SERIAL (USB)
  Serial.setTimeout(SERIAL_TIMEOUT);
  
  delay(1000);
  checkButton();
  delay(1000);                                                // Soft startup to ease the flow of electrons.

  attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_NEC>, CHANGE);    // IR definition
  
  LEDS.setBrightness(max_bright);                                                 // Set the generic maximum brightness value.
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER >(leds, MAX_LEDS);           // APA102 or WS2801 definition
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER >(leds, MAX_LEDS);                   // WS2812 definition
  
  set_max_power_in_volts_and_milliamps(5, 1000);                                  //5V, 1000mA

  random16_set_seed(4832);                                                        // Awesome randomizer of awesomeness
  random16_add_entropy(analogRead(2));
  int ranstart = random16();

  Serial.println("---SETUP COMPLETE---");
  
  ledMode = EEPROM.read(STARTMODE);                                               // Location 0 is the starting mode
  NUM_LEDS = EEPROM.read(STRANDLEN); if(NUM_LEDS >MAX_LEDS) NUM_LEDS = MAX_LEDS;  // Location 1 is the number of LED's
  //strandId = EEPROM.read(STRANDID);                                             // Which strand are we, anyways?

  currentPalette  = CRGBPalette16(CRGB::Black);
  targetPalette   = RainbowColors_p;
  currentBlending = LINEARBLEND;

  // This is for Stefan Petrick's Circular Noise routines
  for (uint8_t i = 0; i < NUM_LEDS; i++) {        // precalculate the lookup-tables:
    uint8_t angle = (i * 256) / NUM_LEDS;         // on which position on the circle is the led?
    xd[i] = cos8( angle );                         // corresponding x position in the matrix
    yd[i] = sin8( angle );                         // corresponding y position in the matrix
  }
  
  strobe_mode(ledMode, 1);                                                        // Initialize the first sequence
  
} // setup()



//------------------MAIN LOOP---------------------------------------------------------------
void loop() {
  
  getirl();                                                                 // Read a command from the IR LED and process command as required.
  readbutton();                                                             // Get button input.
  readkeyboard();                                                           // Get keyboard input.
  
  demo_check();                                                             // If we're in demo mode, check the timer to see if we need to increase the strobe_mode value.

  EVERY_N_MILLISECONDS(50) {                                                 // Smooth palette transitioning runs continuously.
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   
  }

  EVERY_N_SECONDS(5) {                                                        // If selected, change the target palette to a random one every 5 seconds.
    if (palchg == 1) SetupSimilar4Palette();
    if (palchg == 2) SetupRandom4Palette();
    if (palchg == 3) SetupRandom16Palette();
}

  EVERY_N_MILLIS_I(thistimer, thisdelay) {                                    // Sets the original delay time.
    thistimer.setPeriod(thisdelay);                                           // This is how you update the delay value on the fly.
    strobe_mode(ledMode, 0);                                                  // Strobe to display the current sequence, but don't initialize the variables, so mc=0;
  }

  if(glitter) addglitter(10);                                                 // If the glitter flag is set, let's add some.
  
  FastLED.show();                                                             // Power managed display of LED's.
  
} // loop()



//-------------------OTHER ROUTINES----------------------------------------------------------
void strobe_mode(uint8_t newMode, bool mc){                   // mc stands for 'Mode Change', where mc = 0 is strobe the routine, while mc = 1 is change the routine

  if(mc) {
    fill_solid(leds,NUM_LEDS,CRGB(0,0,0));                    // Clean up the array for the first time through. Don't show display though, so you may have a smooth transition.
    Serial.print("Mode: "); 
    Serial.println(ledMode);
  }

  switch (newMode) {                                          // First time through a new mode, so let's initialize the variables for a given display.

    case  0: if(mc) {fill_solid(leds,NUM_LEDS,CRGB(0,0,0));} break;                     // All off, not animated.
    case  1: if(mc) {fill_solid(leds, NUM_LEDS,CRGB(255,255,255));} break;              // All on, not animated.
    case  2: if(mc) {thisdelay=10; allfreq=2; thisspeed=1; thatspeed=1; thishue=0; thathue=128; thisdir=0; thisrot=1; thatrot=1; thiscutoff=128; thatcutoff=192;} two_sin(); break;
    case  3: if(mc) {thisdelay=20; targetPalette=RainbowColors_p; allfreq=4; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case  4: if(mc) {thisdelay=10; targetPalette = PartyColors_p; palchg=2;} noise8_pal(); break;
    case  5: if(mc) {thisdelay=10; allfreq=4; thisspeed=-1; thatspeed=0; thishue=64; thathue=192; thisdir=0; thisrot=0; thatrot=0; thiscutoff=64; thatcutoff=192;} two_sin(); break;
    case  6: if(mc) {thisdelay=20; targetPalette=RainbowColors_p; allfreq=10; bgclr=64; bgbri=4; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case  7: if(mc) {thisdelay=10; numdots=2; targetPalette=PartyColors_p; thisfade=16; thisbeat=8; thisbright=255; thisdiff=64;} juggle_pal(); break;
    case  8: if(mc) {thisdelay=40; targetPalette = LavaColors_p; thisindex=128; thisdir=1; thisrot=0; thisbright=255; bgclr=200; bgbri=6;} matrix_pal(); break;
    case  9: if(mc) {thisdelay=10; allfreq=6; thisspeed=2; thatspeed=3; thishue=96; thathue=224; thisdir=1; thisrot=0; thatrot=0; thiscutoff=64; thatcutoff=64;} two_sin(); break;
    case 10: if(mc) {thisdelay=20; targetPalette=RainbowColors_p; allfreq=16; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case 11: if(mc) {thisdelay=50; mul1=5; mul2=8; mul3=7;} three_sin_pal(); break;
    case 12: if(mc) {thisdelay=10; targetPalette=ForestColors_p;} serendipitous_pal(); break;
    case 13: if(mc) {thisdelay=20; targetPalette=LavaColors_p; allfreq=8; bgclr=0; bgbri=4; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case 14: if(mc) {thisdelay=10; allfreq=20; thisspeed=2; thatspeed=-1; thishue=24; thathue=180; thisdir=1; thisrot=0; thatrot=1; thiscutoff=64; thatcutoff=128;} two_sin(); break;
    case 15: if(mc) {thisdelay=50; targetPalette = PartyColors_p; thisindex=64; thisdir=0; thisrot=1; thisbright=255; bgclr=100; bgbri=10;} matrix_pal(); break;
    case 16: if(mc) {thisdelay=10; targetPalette = OceanColors_p; palchg=1;} noise8_pal(); break;
    case 17: if(mc) {thisdelay=10; targetPalette=PartyColors_p;} circnoise_pal_2(); break;
    case 18: if(mc) {thisdelay=20; allfreq=10; thisspeed=1; thatspeed=-2; thishue=48; thathue=160; thisdir=0; thisrot=1; thatrot=-1; thiscutoff=128; thatcutoff=192;} two_sin(); break;
    case 19: if(mc) {thisdelay=50; mul1=6; mul2=9; mul3=11;} three_sin_pal(); break;
    case 20: if(mc) {thisdelay=10; thisdir=1; thisrot=1; thisdiff=1;} rainbow_march(); break;
    case 21: if(mc) {thisdelay=10; thisdir=1; thisrot=2; thisdiff=10;} rainbow_march(); break;
    case 22: if(mc) {thisdelay=20; hxyinc = random16(1,15); octaves=random16(1,3); hue_octaves=random16(1,5); hue_scale=random16(10, 50);  x=random16(); xscale=random16(); hxy= random16(); hue_time=random16(); hue_speed=random16(1,3); x_speed=random16(1,30);} noise16_pal(); break;
    case 23: if(mc) {thisdelay=20; targetPalette=OceanColors_p; allfreq=6; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case 24: if(mc) {thisdelay=10; targetPalette=OceanColors_p;} circnoise_pal_4(); break;
    case 25: if(mc) {thisdelay=20; targetPalette = PartyColors_p; thisinc=1; thishue=192; thissat=255; thisfade=2; thisdiff=32; thisbright=255;} confetti_pal(); break;
    case 26: if(mc) {thisdelay=10; thisspeed=2; thatspeed=3; thishue=96; thathue=224; thisdir=1; thisrot=1; thatrot=2; thiscutoff=128; thatcutoff=64;} two_sin(); break;
    case 27: if(mc) {thisdelay=30; targetPalette = ForestColors_p; thisindex=192; thisdir=0; thisrot=0; thisbright=255; bgclr=50; bgbri=0;} matrix_pal(); break;
    case 28: if(mc) {thisdelay=20; targetPalette=RainbowColors_p; allfreq=20; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case 29: if(mc) {thisdelay=20; targetPalette = LavaColors_p; thisinc=2; thishue=128; thisfade=8; thisdiff=64; thisbright=255;} confetti_pal(); break;
    case 30: if(mc) {thisdelay=10; targetPalette=PartyColors_p;} circnoise_pal_3(); break;
    case 31: if(mc) {thisdelay=10; numdots=4; targetPalette=OceanColors_p; thisfade=32; thisbeat=12; thisbright=255; thisdiff=20;} juggle_pal(); break;
    case 32: if(mc) {thisdelay=30; SetupSimilar4Palette(); allfreq=4; bgclr=64; bgbri=4; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=128; thisrot=1; thisspeed=8; wavebright=255;} one_sin_pal(); break;
    case 33: if(mc) {thisdelay=50; mul1=3; mul2=4; mul3=5;} three_sin_pal(); break;
    case 34: if(mc) {thisdelay=10; thisdir=-1; thisrot=1; thisdiff=5;} rainbow_march(); break;
    case 35: if(mc) {thisdelay=10; targetPalette=PartyColors_p;} circnoise_pal_1(); break;
    case 36: if(mc) {thisdelay=20; targetPalette = ForestColors_p; thisinc=1; thishue=random8(255); thisfade=1; thisbright=255;} confetti_pal(); break;
    case 37: if(mc) {thisdelay=20; octaves=1; hue_octaves=2; hxy=6000; x=5000; xscale=3000; hue_scale=50; hue_speed=15; x_speed=100;} noise16_pal(); break;
    case 38: if(mc) {thisdelay=10; targetPalette = LavaColors_p; palchg=0;} noise8_pal(); break;

  } // switch newMode
  
} // strobe_mode()



void demo_check(){
  
  if(demorun) {                                                   // Is the demo flag set? If so, let's cycle through them.
    uint8_t secondHand = (millis() / 1000) % (maxMode*demotime);        // Adjust for total time of the loop, based on total number of available modes.
    static uint8_t lastSecond = 99;                               // Static variable, means it's only defined once. This is our 'debounce' variable.
    if (lastSecond != secondHand) {                               // Debounce to make sure we're not repeating an assignment.
      lastSecond = secondHand;
        if(secondHand%demotime==0) {                                     // Every 10 seconds.
          if(demorun == 2) ledMode = random8(0,maxMode); else {
            ledMode = secondHand/demotime;
          }
          strobe_mode(ledMode,1);                            // Does NOT reset to 0.
      } // if secondHand
    } // if lastSecond
  } // if demorun
  
} // demo_check()
// Turtles all the way down.



//----------------- IR Receiver, Keyboard and Button Command Processing ---------------------------------------------

void getirl() {                                                   // This is the IR function that gets the value and selects/performs command.
  
  if (IRProtocol) {

    if(IRCommand == 64260 || strandFlag == 1) {set_strand();}
    
    if (strandActive==1 || IRCommand == 63495) {
    
      Serial.print("Command: ");
      Serial.println(IRCommand);
      switch(IRCommand) {
        case 65280:  max_bright=min(max_bright*2,255); LEDS.setBrightness(max_bright); break;                                       //a1 - Increase max bright
        case 65025:  max_bright=max(max_bright/2,1); LEDS.setBrightness(max_bright); break;                                         //a2 - Decrease max bright
        case 64770:  demorun = 0; ledMode = 0; strobe_mode(ledMode,1); break;                                                                          //a3 - Change to mode 0
        case 64515:  demorun = !demorun; if(demorun) {Serial.println("Demo mode");} else {Serial.println("Not demo mode");} break;  //a4 - Toggle demo mode
  
//        case 64260:  set_strand(); break;                                                                                                                                                  //b1 - Write the current # of LED's
        case 64005:  demorun = 0; ledMode = 0; NUM_LEDS--; Serial.print("NUM_LEDS: "); Serial.println(NUM_LEDS); fill_solid(leds,MAX_LEDS,CRGB(0,0,0)); fill_solid(leds,NUM_LEDS,CRGB(255,255,255)); break;          //b2 - Decrease # of LED's
        case 63750:  demorun = 0; ledMode = 0; NUM_LEDS++; Serial.print("NUM_LEDS: "); Serial.println(NUM_LEDS);  fill_solid(leds,MAX_LEDS,CRGB(0,0,0)); fill_solid(leds,NUM_LEDS,CRGB(255,255,255)); break;         //b3 - Increase # of LED's
        case 63495:  EEPROM.write(STRANDLEN,NUM_LEDS); Serial.print("Writing IR: "); Serial.print(NUM_LEDS); Serial.println(" LEDs"); break;                                                  //b4 - Here is where we enable or disable a strand from receiving commands
  
        case 63240:  strobe_mode(9,1);    break;                //c1 - 
        case 62985:  thisdelay++;         break;                //c2 - thisdelay++;
        case 62730:  thisdelay--;         break;                //c3 - thisdelay--;
        case 62475:  strobe_mode(12,1);   break;                //c4
  
        case 62220:  glitter = !glitter; Serial.println("Glitter baby!");   break;                                    //d1 - Glitter
        case 61965:  demorun = 0; ledMode=(ledMode-1); if (ledMode==255) ledMode=maxMode; strobe_mode(ledMode,1); break;            //d2 - strobe_mode(ledMode--);
        case 61710:  demorun = 0; ledMode=(ledMode+1)%(maxMode+1); strobe_mode(ledMode,1); break;                                   //d3 - strobe_mode(ledMode++);
        case 61455:  EEPROM.write(STARTMODE,ledMode); Serial.print("Writing IR: "); Serial.println(ledMode);  break;     //d4 - Save startup mode
  
        case 61200:  strobe_mode(17,1);   break;                //e1 -
        case 60945:  thisdir = 1;         break;                //e2 - thisdir = 1;
        case 60690:  thisdir = 0;         break;                //e3 - thisdir = 0;
        case 60435:  strobe_mode(20,1);   break;                //e4
  
        case 60180:  palchg = 0; Serial.println("Fixed Palette");            break;                 //f1 - No palette change
        case 59925:  palchg = 1; Serial.println("Similar 4 Palette");        break;                 //f2 - Similar 4 palette
        case 59670:  palchg = 2; Serial.println("Random 4 Palette");         break;                 //f3 - Random 4 palette
        case 59415:  palchg = 3; Serial.println("Random 16 Palette");        break;                 //f4 - Random 16 palette
  
        default:                          break;                // We could do something by default
      } // switch
    } // strandActive
    IRProtocol = 0;                                             // reset IRProtocol variable to not read the same value twice.
  } // if IRLavailable()
  
} // getirl()



void IREvent(uint8_t protocol, uint16_t address, uint32_t command) {
  
  if (IRL_BLOCKING && !IRProtocol) {
    IRProtocol = protocol;                                        // update the values to the newest valid input
    IRAddress = address;
    IRCommand = command;
  }
  
} // IREvent()



void set_strand() {                                           // Setting the active strand.

  if(IRCommand == 64260) IRProtocol = 0;                      // Command is to set strand to let's clear the Protocol flag.

  strandFlag = 1;                                             // We need this state flag in order to be able to continue to run the routine while changing active/inactive.
  Serial.print("Strand is: ");

  if (IRProtocol) {                                           // We have a command and the strandFlag is 1 and it's not the Set Active flag command.
    Serial.println(IRCommand);
    strandFlag = 0;                                           // We know we're finally setting the strand to be ACTIVE/INACTIVE, so we'll clear that state flag.
    if (IRCommand == STRANDID)  {
      strandActive = 1; Serial.println("ACTIVE");
    } else {
      strandActive = 0; Serial.println("INACTIVE");
    }
    IRProtocol = 0;                                             // Let's clear the the IRProtocol flag and be ready for another command.
  }

} // set_strand()



void readkeyboard() {                                         // Process serial commands
  while (Serial.available() > 0) {
  
    inbyte = Serial.read();                                   // Read the command

    if (inbyte != 10) {                                       // Don't print out the separate carriage return.
      Serial.print("# ");
      Serial.print(char(inbyte));
      Serial.print(" ");
    }
    
    switch(inbyte) {

      case 97:                                                // "a" - SET ALL TO ONE colour BY hue = 0 - 255
        demorun = 0;
        ledMode = 0;
        thisarg = Serial.parseInt();
        thisarg = constrain(thisarg,0,255);
        Serial.println(thisarg);
        fill_solid(leds, NUM_LEDS, CHSV(thisarg, 255, 255));
        break;

      case 98:                                                // "b" - SET MAX BRIGHTNESS to #
        max_bright = Serial.parseInt();
        max_bright = constrain(max_bright,0,255);        
        Serial.println(max_bright);
        LEDS.setBrightness(max_bright);
        break;

      case 99:                                                // "c" - CLEAR STRIP
        Serial.println(" ");
        demorun = 0;
        ledMode = 0;
        strobe_mode(ledMode, 1);
        break;

      case 100:                                               // "d" - SET DELAY VAR to #
        thisarg = Serial.parseInt();
        thisdelay = constrain(thisarg,0,255);
        Serial.println(thisdelay);
        break;

      case 101:                                              // "e" - SET PREVIOUS / NEXT mode

        thisarg = Serial.parseInt();
        if (thisarg) {
          demorun = 0; ledMode=(ledMode+1)%(maxMode+1);
        } else {
         demorun = 0; ledMode=(ledMode-1); if (ledMode==255) ledMode=maxMode; 
        }
        strobe_mode(ledMode,1);
        break;

      case 102:                                               // "f - Set a fixed palette
        demorun = 0;
        palchg = 0;
        thisarg = Serial.parseInt();
        gCurrentPaletteNumber = thisarg % gGradientPaletteCount;
        targetPalette = gGradientPalettes[gCurrentPaletteNumber];
        Serial.println(gCurrentPaletteNumber);
        break;

      case 103:                                               // "g" - TOGGLE glitter
        glitter = !glitter;
        Serial.println(" ");
        break;

      case 104:                                               // "h" - SET HUE VAR to #
        thisarg = Serial.parseInt();
        thishue = constrain(thisarg,0,255);
        Serial.println(thishue);
        break;

      case 105:                                               // "i" - Set Similar Palette with hue selection
        palchg = 0;
        thisarg = Serial.parseInt();
        thishue = constrain(thisarg,0,255);
        Serial.println(thishue);
        SetupMySimilar4Palette();
        break;

      case 108:                                               // "l" - Set strip length & save
        thisarg = Serial.parseInt();
        NUM_LEDS = constrain(thisarg,1,MAX_LEDS);
        Serial.println(NUM_LEDS);
        EEPROM.write(STRANDLEN, NUM_LEDS);
        break;

      case 109:                                               // "m" - SET MODE to #
        demorun = 0;
        ledMode = Serial.parseInt();
        ledMode = constrain(ledMode,0,maxMode);
        Serial.println(ledMode);
        strobe_mode(ledMode, 1);
        break;

      case 110:                                               // "n"  - TOGGLE direction
        Serial.println(" ");
        thisdir = !thisdir;
        break;

      case 112:                                               // "p" - Play mode is either fixed, sequential or shuffle
        demorun = Serial.parseInt();
        demorun = constrain(demorun,0,2);
        Serial.println(demorun);        
        break;      
      
      case 113:                                               // "q" - Get version number
        Serial.println(SEIRLIGHT_VERSION);
        break;

      case 115:                                               // "s"  SET SATURATION VAR to #
        thisarg = Serial.parseInt();
        thissat = constrain(thisarg,0,255);
        Serial.println(thissat);
        break;

      case 116:                                               // "t" - Select Palette mode
        thisarg = Serial.parseInt();
        palchg = constrain(thisarg,0,3);
        Serial.println(palchg);
        break;

      case 117:                                               // "u" - Sequence duration
        thisarg = Serial.parseInt();
        demotime = constrain(thisarg,1,255);
        Serial.println(demotime);
        break;

      case 119:                                               // "w" - Write current mode to EEPROM
        EEPROM.write(STARTMODE,ledMode);
        Serial.print("Writing keyboard: ");
        Serial.println(ledMode);
        break;   
        
    } // switch inbyte
  } // while Serial.available
  
} // readkeyboard()



void readbutton() {                                           // Read the button and perform action

  uint8_t b = checkButton();

  if (b == 1) {                                               // Just a click event to advance the mode
    demorun = 0;
    ledMode=(ledMode+1)%(maxMode+1);
    strobe_mode(ledMode,1);
    Serial.print("Advance ");
    Serial.println(ledMode);
  }

  if (b == 2) {                                               // A double-click event to reset to 0
    demorun = 0;
    ledMode = 0;
    strobe_mode(ledMode, 1);
    Serial.println("Reset");
  }

  if (b == 3) {                                               // A hold event to write current mode to EEPROM
    demorun = 0;
    EEPROM.write(STARTMODE,ledMode);
    Serial.print("Writing Button: ");
    Serial.println(ledMode);
  }

} // readbutton()




