/*
 * notamesh - IR controlled 'synchronized' led lighting effects using FastLED across multiple Arduino controllers.
 * 
 *       By: Andrew Tuline
 *     Date: October, 2018
 *      URL: www.tuline.com
 *    Email: atuline@gmail.com
 *     Gist: https://gist.github.com/atuline
 *   GitHub: https://github.com/atuline
 * Pastebin: http://pastebin.com/u/atuline
 *  Youtube: https://www.youtube.com/user/atuline/videos
 * 
 * 
 * CAUTION ************************************************************************************************************************************************
 * 
 * Before attempting to compile this program, make sure you are already comfortable programming Arduino C and with FastLED, In addition, you
 * should already be able to download, install and USE 3rd party libraries. If you are a beginner, this is NOT the code you're looking for.
 * 
 * ********************************************************************************************************************************************************
 *
 *
 * WARNING ************************************************************************************************************************************************
 * 
 * 1) Although you can run 3 pin based WS2812 LED's with notamesh, you can ONLY do so in demo mode using an Arduino UNO/Nano. The IR command 
 * functionality will NOT work. You will need to use 4 pin strips, such as the APA102's to use the full functionality of this program. Reference:
 * 
 * https://github.com/FastLED/FastLED/wiki/Interrupt-problems
 * 
 * 2) Effects in this program are programmed for up to 255 LED's (if memory holds out). You will need to re-write it for longer strips.
 * 
 **********************************************************************************************************************************************************
 * 
 
 * 
 * Introduction ******************************
 * 
 * 1.05 Update
 * 
 * Set/display a favourite
 * 
 * 
 * 1.04 Update
 * 
 * Make programming length and delay to be unit selectable.
 * Change demo mode to start at ledMode 2.
 * Set default to non-demo mode.
 * 
 * 
 * This is a modification of my seirlight and previously aalight programs and provides the following:
 * 
 * - Uses an Arduino microcontroller.
 * - Uses the FastLED display library.
 * - Supports multiple display sequences, most with support for multiple parameters.
 * - Supports IR (Infra Red) communications for control of the display sequences. 
 * - Selectable individual display sequences or use a demo mode.
 * - Can support APA102 and WS2801 with IR communications.
 * - Can support WS2812 if NOT using IR communications (on an 8 bit AVR).
 * - Can save startup information to EEPROM including # of LED's, strand length, starting mode and mesh delay time.
 * - Add plasma, remove circnoise routines.
 * - NEW FUNCTIONALITY ---> Supports delayed sequences across Arduinos, which you might see in a mesh network. But it's 'notamesh' network.
 * - NEW FUNCTIONALITY ---> Use paletteknife palettes (for notamesh palette consistency across multiple strands) rather than the previous random generated ones.
 * 
 * 
 * 
 * 1.03 Update *********************************
 *
 * *** - Convert 'thisdir' direction variable from boolean to -1, 1 and update all directional based routines.
 * *** - Reduce direction IR button press to a toggle.
 * *** - Fix IR button pressed modes that don't animate.
 * *** - Move IR codes to commands.h and update the values to the 24 button pad I have.
 * *** - Remove keyboard and button functionality. I don't use it.
 * *** - Remove STRANDID and strandactive security features, as well as select individual Arduino. I may bring this back.
 * *** - Save strandlength after every change to B2/B3 instead of a separate 'save' command.
 * *** - Add EEPROM initialization check and save initial values.
 * *** - Add notamesh functionality which configures a delay. It displays 1 white LED per 100ms delay.
 * 
 * 
 * 
 * Hardware Setup *******************************
 * 
 * - The LED data line is connected to pin 12 (changeable).
 * - The LED clock line is connected to pin 11 (changeable).
 * - For IR functionality, connect the data output of a TSOP34838 IR receiver to pin D2 (other TSOP pins are 5V and Gnd).
 * - If you use WS2812 LED's, then the IR will NOT work. See https://github.com/FastLED/FastLED/wiki/Interrupt-problems
 * - This supports 64 LED's currently. You can increase this value by changing MAX_LEDS, up to 255 (not tested) with adequate memory.
 * 
 * 
 * 
 * EEPROM Functionality **********************
 * 
 * - We can save the startup display mode in EEPROM.
 * - We can save the strand length in EEPROM, however the MAXIMUM strand length is pre-defined. We just use a lesser value for the array.
 * - We can save the mesh delay in EEPROM by 100ms at a time.
 * 
 * 
 * Libraries Required (to download) **************
 * 
 * - FastLED library from https://github.com/FastLED/FastLED
 * - Nico Hood's IRL library from https://github.com/NicoHood/IRLremote
 * 
 * 
 * 
 * First Time Initialization ***************
 * 
 * The first time notamesh runs on an Arduino, the setup program will initialize the EEPROM so that:
 * 
 * - An Arduino initialization value will be saved.
 * - The starting mode will be 0 (black).
 * - The starting NUM_LEDS length will be 20 LED's.
 * - The starting notamesh delay will be 0 ms.
 * 
 * 
 *
 * Compile Time Configuration ********************
 * 
 * - Configure and compile notamesh for your type of strand, and pins used.
 * - Set the demorun startup variable to 1 for demo mode or 0 for regular use. It's configured to 0 by default.
 *
 *
 *
 * Standalone Initial Configuration ************
 * 
 * Power up an Arduino/strip and:
 * 
 * - Press B2 to decrease the strand length
 * - Press B3 to increase the strand length
 * 
 * LED's will light up as 'white' to indicate the strand length. The strand length will
 * be saved to EEPROM after each keypress. Once done, press A3 to reset the Arduino.
 * 
 * 
 * 
 * Notamesh Initial Configuration **************
 * 
 * You'll need to 'activate' your strand for EEPROM programming. To do so:
 * 
 * - Press B1 to put the Arduino into 'Select' mode.
 * - Press the button equivalent to your strandID, i.e. B4 to 'activate' your Arduino for EEPROM programming
 * 
 * To increase/decrease the mesh delay for the currently active Arduino:
 * 
 * - Press E2 to decrease the amount of mesh delay by 100ms.
 * - Press E3 to decrease the amount of mesh delay by 100ms.
 * 
 * LED's will light up as 'white' to indicate the mesh delay (1 led per 100ms). The mesh delay
 * will be saved to EEPROM after each keypress. Once done, press button 'A3' to reset the Arduino.
 * 
 * To increase/decrease the length of your strand for the currently active Arduino: 
 * 
 * - Press B2 to decrease the length of the strand.
 * - Press B3 to increase the length of the strand.
 * 
 * LED's will light up as 'white' to indicate the strand length which will be saved
 * to EEPROM after each keypress. Once done, press button 'A3' to reset the Arduino.
 * 
 * 
 * Setting Favourites Configuration
 * 
 * - Press the D2 | D3 buttons to find a mode you really like.
 * - Press B1 to put the Arduino into 'Select' mode.
 * - Press the button equivalent to your strandID, i.e. B4 to 'activate' your Arduino for EEPROM programming
 * - Press E1 to set Favourite 1 mode or press E4 to set Favourite 2 mode.
 * - Once done, press button 'A3' to reset the Arduino.
 * - You should then be able to press E1 or E4 to select the programmed favourite 1 or 2 modes.
 * 
 * 
 * 
 * Notamesh Operation ******************************
 * 
 * - The notamesh initial configuration is important so that there's a different delay between each LED strip.
 * - Press A3 once all notamesh Arduinos are running in order to synchronize them with the same millis() value.
 * - From there, you should be able to select demo mode or individual sequences below.
 * - The routines should run and you should get a cool synchronized display, combined with a delay so that it looks like they're communicating.
 * 
 * 
 * 
 * IR Keys and Operation ************************************
 * 
 * I'm using a 24 button IR remote like the ones provided with 5V LED strands from China. If you use a different one, you'll need to map
 * the codes to the modes in the irtest(); routine. I've provided irtest.ino to test this out with. You can also open up the serial monitor
 * and view the keypress entries. The chart below summarizes the commands. Button location uses characters as the row, and numbers are the
 * columns, so C2 is the 3rd row, 2nd column. See commands.h for the mapping.
 * 
 * 
 * Command                          IR Button location & description
 * --------                         --------------------------------
 * Increase brightness              A1  Increase brightness
 * Decrease brightness              A2  Decrease brightness
 * Reset and set mode 0             A3  Reboots the Arduino in order to sync millis() if using notamesh.
 * Enable demo mode                 A4  Demo mode cycles through the routines based on the millis() counter.
 * 
 * Select Arduino                   B1  This allows the EEPROM to be updated. Then press A1 through F4. A1 is the first one. Set the StrandID value related to a keypress at compile time.
 * Decrease strand length           B2  The # of LED's programmed are white, only if strand is active (with the Select Arduino command.)
 * Increase strand length           B3  The # of LED's programmed are white, only if strand is active (with the Select Arduino command.)
 *                                  B4
 * 
 *                                  C1
 * Slower animation                 C2 Increase value of EVERY_N_MILLISECONDS()
 * Faster animation                 C3 Decrease value of EVERY_N_MILLISECONDS()
 * Toggle direction                 C4 Toggle direction of some sequences.
 * 
 * Enable/disable glitter           D1  Toggles glitter on some sequences.
 * Previous display mode            D2  Also disables demo mode.
 * Next display mode                D3  Also disables demo mode.
 * Save Current mode to EEPROM      D4  This will be the startup mode, and disables demo mode temporarily. Disable startup demo mode at compile time.
 * 
 * 
 * Set/display favourite 1          E1  Put in select mode to set current mode as favourite 1.
 * Shorter mesh delay               E2  Decrease delay by 100ms before starting (using white LED's), only if strand is active (with the Select Arduino command.)
 * Longer mesh delay                E3  Increase delay by 100ms before starting (using white LED's), only if strand is active (with the Select Arduino command.)
 * Set/display favourite 2          E4  Put in select mode to set current mode as favourite 2.
 * 
 * 
 * Stop palette rotation            F1   Stop palette rotation at current palette.
 * Select previous palette          F2   Stop palette rotation and select previous palette immediately.
 * Select next palette              F3   Stop palette rotation and select next palette immediately.
 * Enable palette rotation          F4   Enable palette transitioning every 5 seconds.
 * 
 * 
 * 
 */


/*------------------------------------------------------------------------------------------
--------------------------------------- Start of variables ---------------------------------
------------------------------------------------------------------------------------------*/


#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.

#define NOTAMESH_VERSION 105                                  // Just a continuation of seirlight and previously aalight.

#include "FastLED.h"                                          // https://github.com/FastLED/FastLED
#include "IRLremote.h"                                        // https://github.com/NicoHood/IRLremote
#include "EEPROM.h"                                           // This is included with base install

#include "commands.h"                                         // The IR commands.


#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif


#define pinIR 2                                               // Choose a valid PinInterrupt pin of your Arduino board for IR operations. In this case, D2.
#define IRL_BLOCKING true

// Serial definition
#define SERIAL_BAUDRATE 57600                                 // Or 115200.


// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for all strands
#define LED_CK 11                                             // Serial clock pin for WS2801 or APA102
#define COLOR_ORDER BGR                                       // It's GRB for WS2812
#define LED_TYPE APA102                                       // Alternatively WS2801, or WS2812
#define MAX_LEDS 64                                           // Maximum number of LED's defined (at compile time).


// Initialize changeable global variables.
uint8_t NUM_LEDS;                                             // Number of LED's we're actually using, and we can change this only the fly for the strand length.

uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[MAX_LEDS];                                   // Initialize our LED array. We'll be using less in operation.

 CRGBPalette16 gCurrentPalette;                               // Use palettes instead of direct CHSV or CRGB assignments
 CRGBPalette16 gTargetPalette;                                // Also support smooth palette transitioning


extern const TProgmemRGBGradientPalettePtr gGradientPalettes[]; // These are for the fixed palettes in gradient_palettes.h
extern const uint8_t gGradientPaletteCount;                     // Total number of fixed palettes to display.
uint8_t gCurrentPaletteNumber = 0;                              // Current palette number from the 'playlist' of color palettes
uint8_t currentPatternIndex = 0;                                // Index number of which pattern is current


TBlendType currentBlending = LINEARBLEND;                     // NOBLEND or LINEARBLEND


// EEPROM location definitions.
#define ISINIT    0                                           // EEPROM location used to verify that this Arduino has been initialized
#define STARTMODE 1                                           // EEPROM location for the starting mode.
#define STRANDLEN 2                                           // EEPROM location for the actual Length of the strand, which is < MAX_LEDS
#define STRANDEL  3                                           // EEPROM location for the mesh delay value.
#define FAV1      4                                           // EEPROM location of first favourite routine.
#define FAV2      5                                           // EEPROM location of second favourite routine.

#define INITVAL   0x55                                        // If this is the value in ISINIT, then the Arduino has been initialized. Startmode should be 0 and strandlength should be 
#define INITMODE  0                                           // Startmode is 0, which is black.
#define INITLEN   20                                          // Start length is 20 LED's.
#define INITDEL   0                                           // Starting mesh delay value of the strand in milliseconds.
#define INITFAV   0                                           // Starting favourite modes.

// These are deprecated values for use with seirlight. I may bring them back in the future. Don't delete them.
const uint32_t STRANDID = IR_B4;                              // This is the ID button of the strand and should be unique for each strand in a series (if you want them to be different).
bool strandActive = 0;                                        // 0=inactive, 1=active. Must be activated by button press of B1, then B3 (or the appropriate button).
bool strandFlag = 0;                                          // Flag to let us know if we're changing the active strand.

uint16_t meshdelay;                                           // Timer for the notamesh. Works with INITDEL.

uint8_t ledMode = 0;                                          // Starting mode is typically 0. Change INITMODE if you want a different starting mode.
uint8_t demorun = 0;                                          // 0 = regular mode, 1 = demo mode, 2 = shuffle mode.
uint8_t maxMode = 38;                                         // Maximum number of modes.
uint8_t demotime = 10;                                        // Set the length of the demo timer.

uint8_t IRProtocol = 0;                                       // Temporary variables to save latest IR input
uint16_t IRAddress = 0;
uint32_t IRCommand = 0;



// Generic/shared routine variables ----------------------------------------------------------------------
uint8_t allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
uint8_t bgclr = 0;                                            // Generic background colour
uint8_t bgbri = 0;                                            // Generic background brightness
bool    glitter = 0;                                          // Glitter flag
uint8_t palchg = 3;                                           // 0=no change, 1=similar, 2=random
uint8_t startindex = 0;
uint8_t thisbeat;                                             // Standard beat
uint8_t thisbright = 0;                                       // Standard brightness
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
uint16_t thisdelay = 0;                                       // Standard delay
uint8_t thisdiff = 1;                                         // Standard palette jump
int8_t  thisdir = 1;                                          // Standard direction is either -1 or 1. Used as a multiplier rather than boolean.
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



// Display functions -----------------------------------------------------------------------

// Support functions
#include "addglitter.h"
// #include "make_palettes.h"                                   // Removed from notamesh. We'll use gradient_palettes instead.

// Display routines
#include "confetti_pal.h"
#include "gradient_palettes.h"
#include "juggle_pal.h"
#include "matrix_pal.h"
#include "noise16_pal.h"
#include "noise8_pal.h"
#include "one_sin_pal.h"
#include "plasma.h"
#include "rainbow_march.h"
#include "serendipitous_pal.h"
#include "three_sin_pal.h"
#include "two_sin.h"



/*------------------------------------------------------------------------------------------
--------------------------------------- Start of code --------------------------------------
------------------------------------------------------------------------------------------*/


void setup() {

  Serial.begin(SERIAL_BAUDRATE);                                                  // Setup serial baud rate

  Serial.println(" "); Serial.println("---SETTING UP---");

  delay(1000);                                                                    // Slow startup so we can re-upload in the case of errors.

  attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_NEC>, CHANGE);    // IR definition
  
  LEDS.setBrightness(max_bright);                                                 // Set the generic maximum brightness value.

  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER >(leds, MAX_LEDS);           // APA102 or WS2801 definition
//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER >(leds, MAX_LEDS);                 // WS2812 definition
  
  set_max_power_in_volts_and_milliamps(5, 1000);                                   //5V, 1A

  random16_set_seed(4832);                                                        // Awesome randomizer of awesomeness
  random16_add_entropy(analogRead(2));
  int ranstart = random16();

  if (EEPROM.read(ISINIT) != INITVAL) {                                           // Check to see if Arduino has been initialized, and if not, do so.
    EEPROM.write(STARTMODE, INITMODE);                                            // Initialize the starting mode to 0.
    EEPROM.write(STRANDLEN, INITLEN);                                             // Initialize the starting length to 20 LED's.
    EEPROM.write(ISINIT, INITVAL);                                                // Initialize the starting value (so we know it's initialized) to INITVAL.
    EEPROM.write(STRANDEL, INITDEL);                                              // Initialize the notamesh delay to 0.
    EEPROM.write(FAV1, INITFAV);                                                  // Initialize first favourite mode to 0.
    EEPROM.write(FAV2, INITFAV);                                                  // Initialize second favourite mode to 0.
  }
 
  ledMode = EEPROM.read(STARTMODE);                                               // Location 0 is the starting mode.
  NUM_LEDS = EEPROM.read(STRANDLEN);                                              // Need to ensure NUM_LEDS < MAX_LEDS elsewhere.
  meshdelay = EEPROM.read(STRANDEL);                                              // This is our notamesh delay for cool delays across strands.

  Serial.print("Initial delay: "); Serial.print(meshdelay*100); Serial.println("ms delay.");
  Serial.print("Initial strand length: "); Serial.print(NUM_LEDS); Serial.println(" LEDs");

  gCurrentPalette = CRGBPalette16(CRGB::Black);
  gTargetPalette = (gGradientPalettes[0]);

  strobe_mode(ledMode, 1);                                                        // Initialize the first sequence

  Serial.println("---SETUP COMPLETE---");

} // setup()



//------------------MAIN LOOP---------------------------------------------------------------
void loop() {
  
  getirl();                                                                   // Read a command from the IR LED and process command as required.
  
  demo_check();                                                               // If we're in demo mode, check the timer to see if we need to increase the strobe_mode value.

  EVERY_N_MILLISECONDS(50) {                                                  // Smooth palette transitioning runs continuously.
    uint8_t maxChanges = 24; 
      nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette, maxChanges);  
  }

  EVERY_N_SECONDS(5) {                                                        // If selected, change the target palette to a random one every 5 seconds.
    if (palchg==3) {
      gCurrentPaletteNumber = addmod8(gCurrentPaletteNumber, 1, gGradientPaletteCount);
    }
    gTargetPalette = gGradientPalettes[gCurrentPaletteNumber];                // We're just ensuring that the gTargetPalette WILL be assigned.
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
    Serial.println(newMode);
    Serial.println(millis());
  }

  switch (newMode) {                                          // First time through a new mode, so let's initialize the variables for a given display.

    case  0: if(mc) {fill_solid(leds,NUM_LEDS,CRGB(1,0,0));} break;                     // All off, not animated.
    case  1: if(mc) {thisdelay=20; hxyinc = random16(1,15); octaves=random16(1,3); hue_octaves=random16(1,5); hue_scale=random16(10, 50);  x=random16(); xscale=random16(); hxy= random16(); hue_time=random16(); hue_speed=random16(1,3); x_speed=random16(1,30);} noise16_pal(); break;
    case  2: if(mc) {thisdelay=10; allfreq=2; thisspeed=1; thatspeed=2; thishue=0; thathue=128; thisdir=1; thisrot=1; thatrot=1; thiscutoff=128; thatcutoff=192;} two_sin(); break;
    case  3: if(mc) {thisdelay=20; allfreq=4; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case  4: if(mc) {thisdelay=10; palchg=2;} noise8_pal(); break;
    case  5: if(mc) {thisdelay=10; allfreq=4; thisspeed=-1; thatspeed=0; thishue=64; thathue=192; thisdir=1; thisrot=0; thatrot=0; thiscutoff=64; thatcutoff=192;} two_sin(); break;
    case  6: if(mc) {thisdelay=20; allfreq=10; bgclr=64; bgbri=4; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case  7: if(mc) {thisdelay=10; numdots=2; thisfade=16; thisbeat=8; thisbright=255; thisdiff=64;} juggle_pal(); break;
    case  8: if(mc) {thisdelay=40; thisindex=128; thisdir=1; thisrot=0; thisbright=255; bgclr=200; bgbri=6;} matrix_pal(); break;
    case  9: if(mc) {thisdelay=10; allfreq=6; thisspeed=2; thatspeed=3; thishue=96; thathue=224; thisdir=1; thisrot=0; thatrot=0; thiscutoff=64; thatcutoff=64;} two_sin(); break;
    case 10: if(mc) {thisdelay=20; allfreq=16; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case 11: if(mc) {thisdelay=50; mul1=5; mul2=8; mul3=7;} three_sin_pal(); break;
    case 12: if(mc) {thisdelay=10;} serendipitous_pal(); break;
    case 13: if(mc) {thisdelay=20; allfreq=8; bgclr=0; bgbri=4; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case 14: if(mc) {thisdelay=10; allfreq=20; thisspeed=2; thatspeed=-1; thishue=24; thathue=180; thisdir=1; thisrot=0; thatrot=1; thiscutoff=64; thatcutoff=128;} two_sin(); break;
    case 15: if(mc) {thisdelay=50; thisindex=64; thisdir=-1; thisrot=1; thisbright=255; bgclr=100; bgbri=10;} matrix_pal(); break;
    case 16: if(mc) {thisdelay=10; palchg=1;} noise8_pal(); break; // By: Andrew Tuline
    case 17: if(mc) {thisdelay=10;} plasma(11,23,4,18); break;
    case 18: if(mc) {thisdelay=20; allfreq=10; thisspeed=1; thatspeed=-2; thishue=48; thathue=160; thisdir=-1; thisrot=1; thatrot=-1; thiscutoff=128; thatcutoff=192;} two_sin(); break;
    case 19: if(mc) {thisdelay=50; mul1=6; mul2=9; mul3=11;} three_sin_pal(); break;
    case 20: if(mc) {thisdelay=10; thisdir=1; thisrot=1; thisdiff=1;} rainbow_march(); break;
    case 21: if(mc) {thisdelay=10; thisdir=1; thisrot=2; thisdiff=10;} rainbow_march(); break;
    case 22: if(mc) {thisdelay=20; hxyinc = random16(1,15); octaves=random16(1,3); hue_octaves=random16(1,5); hue_scale=random16(10, 50);  x=random16(); xscale=random16(); hxy= random16(); hue_time=random16(); hue_speed=random16(1,3); x_speed=random16(1,30);} noise16_pal(); break;
    case 23: if(mc) {thisdelay=20; allfreq=6; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case 24: if(mc) {thisdelay=10;} plasma(23,15,6,7); break;
    case 25: if(mc) {thisdelay=20; thisinc=1; thishue=192; thissat=255; thisfade=2; thisdiff=32; thisbright=255;} confetti_pal(); break;
    case 26: if(mc) {thisdelay=10; thisspeed=2; thatspeed=3; thishue=96; thathue=224; thisdir=1; thisrot=1; thatrot=2; thiscutoff=128; thatcutoff=64;} two_sin(); break;
    case 27: if(mc) {thisdelay=30; thisindex=192; thisdir=-1; thisrot=0; thisbright=255; bgclr=50; bgbri=0;} matrix_pal(); break;
    case 28: if(mc) {thisdelay=20; allfreq=20; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin_pal(); break;
    case 29: if(mc) {thisdelay=20; thisinc=2; thishue=128; thisfade=8; thisdiff=64; thisbright=255;} confetti_pal(); break;
    case 30: if(mc) {thisdelay=10;} plasma(8,7,9,13); break;
    case 31: if(mc) {thisdelay=10; numdots=4; thisfade=32; thisbeat=12; thisbright=255; thisdiff=20;} juggle_pal(); break;
    case 32: if(mc) {thisdelay=30; allfreq=4; bgclr=64; bgbri=4; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=128; thisrot=1; thisspeed=8; wavebright=255;} one_sin_pal(); break;
    case 33: if(mc) {thisdelay=50; mul1=3; mul2=4; mul3=5;} three_sin_pal(); break;
    case 34: if(mc) {thisdelay=10; thisdir=-1; thisrot=1; thisdiff=5;} rainbow_march(); break;
    case 35: if(mc) {thisdelay=10;} plasma(11,17,20,23); break;
    case 36: if(mc) {thisdelay=20; thisinc=1; thishue=random8(255); thisfade=1; thisbright=255;} confetti_pal(); break;
    case 37: if(mc) {thisdelay=20; octaves=1; hue_octaves=2; hxy=6000; x=5000; xscale=3000; hue_scale=50; hue_speed=15; x_speed=100;} noise16_pal(); break;
    case 38: if(mc) {thisdelay=10; palchg=0;} noise8_pal(); break;

  } // switch newMode
  
} // strobe_mode()



void demo_check(){
  
  if(demorun) {                                                   // Is the demo flag set? If so, let's cycle through them.
    uint8_t secondHand = (millis() / 1000) % (maxMode*demotime);  // Adjust for total time of the loop, based on total number of available modes.
    static uint8_t lastSecond = 99;                               // Static variable, means it's only defined once. This is our 'debounce' variable.
    if (lastSecond != secondHand) {                               // Debounce to make sure we're not repeating an assignment.
      lastSecond = secondHand;
        if(secondHand%demotime==0) {                              // Every 10 seconds.
          if(demorun == 2) ledMode = random8(0,maxMode); else {
            ledMode = secondHand/demotime;
          }
          meshwait();
          strobe_mode(ledMode,1);                                 // Does NOT reset to 0.
      } // if secondHand
    } // if lastSecond
  } // if demorun
  
} // demo_check()



//----------------- IR Receiver and Button Command Processing ---------------------------------------------

void getirl() {                                                   // This is the IR function that gets the value and selects/performs command.
  
  if (IRProtocol) {

    if(strandFlag == 1) set_strand();       // Set the strand length
    
      Serial.print("Command: ");
      Serial.println(IRCommand);
      switch(IRCommand) {
        case IR_A1:  max_bright=min(max_bright*2,255); LEDS.setBrightness(max_bright); break;                                       //a1 - Increase max bright
        case IR_A2:  max_bright=max(max_bright/2,1); LEDS.setBrightness(max_bright); break;                                         //a2 - Decrease max bright
        case IR_A3:  fill_solid(leds,NUM_LEDS,CRGB(0,0,0)); FastLED.show(); bootme(); break;                                          //a3 - Change to mode 0, display and reboot
        case IR_A4:  demorun = 1; if(demorun) {Serial.println("Demo mode"); meshwait();} else {Serial.println("Not demo mode");} break;  //a4 - Enable demo mode
  
        case IR_B1:  set_strand(); break;                                            //b1 - Set Strand Active or Inactive for EEPROM programming.
        case IR_B2:  if (strandActive==1) set_strandlen(); break;                    //b2 - Decrease # of LED's and write to EEPROM
        case IR_B3:  if (strandActive==1) set_strandlen(); break;                    //b3 - Increase # of LED's and write to EEPROM
        case IR_B4:  break;                                                          //b4 -
  
        case IR_C1:  break;                                                                                           //c1
        case IR_C2:  thisdelay++; break;                                                                              //c2 - Slow down the sequence as much as you want.
        case IR_C3:  thisdelay--; if(thisdelay >30000) thisdelay = 0; break;                                          //c3 - Speed up the sequence, but don't go too far.
        case IR_C4:  thisdir = thisdir*-1;Serial.print("thisdir = "); Serial.println(thisdir);  break;                //c4 - Change the direction of the LEDs
  
        case IR_D1:  glitter = !glitter; Serial.println("Glitter baby!");   break;                                                              //d1 - Glitter
        case IR_D2:  demorun = 0; ledMode=(ledMode-1); if (ledMode==255) ledMode=maxMode; meshwait(); strobe_mode(ledMode,1); break;            //d2 - strobe_mode(ledMode--);
        case IR_D3:  demorun = 0; ledMode=(ledMode+1)%(maxMode+1); meshwait(); strobe_mode(ledMode,1); break;                                   //d3 - strobe_mode(ledMode++);
        case IR_D4:  EEPROM.write(STARTMODE,ledMode); Serial.print("Writing startup mode: "); Serial.println(ledMode);  break;                  //d4 - Save startup mode

//        case IR_E1:  demorun = 0; ledMode = 3; meshwait(); strobe_mode(ledMode,1);   break;             //e1
        case IR_E1:  if (strandActive==1) {set_strandfav(); } else {demorun = 0; ledMode = EEPROM.read(FAV1);  meshwait(); strobe_mode(ledMode,1);}   break;    //e1
        case IR_E2:  if (strandActive==1) set_meshdel(); break;                                         //e2 - Shorter mesh delay by 100ms
        case IR_E3:  if (strandActive==1) set_meshdel(); break;                                         //e3 - Longer mesh delay by 100ms
//        case IR_E4:  demorun = 0; ledMode = 34; meshwait(); strobe_mode(ledMode,1);   break;            //e4
        case IR_E4:  if (strandActive==1) {set_strandfav(); } else {demorun = 0; ledMode = EEPROM.read(FAV2);  meshwait(); strobe_mode(ledMode,1);}   break;    //e1

        if (strandActive==1) {set_strandfav(); } else {demorun = 0; ledMode = EEPROM.read(FAV1);  meshwait(); strobe_mode(ledMode,1);}   break;             //e1



        case IR_F1:  palchg = 0; Serial.print("Stop and select current Palette "); Serial.println(gCurrentPaletteNumber); break;                                                                                               //f1 - Stop and select current Palette
        case IR_F2:  palchg = 1; Serial.print("Stop and select previous Palette "); gCurrentPaletteNumber -= 1; if(gCurrentPaletteNumber == 255) gCurrentPaletteNumber = gGradientPaletteCount; Serial.println(gCurrentPaletteNumber); gCurrentPalette = (gGradientPalettes[gCurrentPaletteNumber]); break;    //f2 - Stop and select previous Palette
        case IR_F3:  palchg = 2; Serial.print("Stop and select next Palette "); gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);  Serial.println(gCurrentPaletteNumber);  gCurrentPalette = (gGradientPalettes[gCurrentPaletteNumber]); break;             //f3 - Stop and select next Palette
        case IR_F4:  palchg = 3; Serial.print("Continuous palette change ");  Serial.println(gCurrentPaletteNumber); break;                                                                                                    //f4 - Continuous palette change

        default:     break;                // We could do something by default

      } // switch IRCommand

    IRProtocol = 0;                                             // Reset IRProtocol variable to not read the same value twice.

  } // if IRProtocol

} // getirl()



void bootme() {                                                 // This is used to reset all the Arduinos so that their millis() counters are all in sync.

  asm volatile("jmp 0");
  
} // bootme()



void meshwait() {                                                   // After we press a mode key, we need to wait a bit for the sequence to start.

  Serial.print("Mesh delay: "); Serial.print(meshdelay*100); Serial.println("ms delay.");

  FastLED.delay(meshdelay*100);                                     // Here's our notamesh wait upon keypress. Oh god I'm so sorry there's a delay statement here. At least it's only used upon mode change keypress.

} // meshwait()



void IREvent(uint8_t protocol, uint16_t address, uint32_t command) {
  
  if (IRL_BLOCKING && !IRProtocol) {
    IRProtocol = protocol;                                        // update the values to the newest valid input
    IRAddress = address;
    IRCommand = command;
  }
  
} // IREvent()



void set_strand() {                                             // Setting the active strand.

  if (IRCommand == IR_B1) {IRProtocol = 0; strandFlag = 1;}      // Command is to set strand to let's clear the Protocol flag.
                                                                // We need this state flag in order to be able to continue to run the routine while changing active/inactive.
  if (IRProtocol) {                                             // We have a command and the strandFlag is 1 and it's not the Set Active flag command.
    Serial.println(IRCommand);
    Serial.print("Strand is: ");
    strandFlag = 0;                                             // We know we're finally setting the strand to be ACTIVE/INACTIVE, so we'll clear that state flag.
    if (IRCommand == STRANDID)  {
      strandActive = 1; Serial.println("ACTIVE");
    } else {
      strandActive = 0; Serial.println("INACTIVE");
    }
    IRProtocol = 0;                                             // Let's clear the the IRProtocol flag and be ready for another command.
    Serial.println("Finished strand length.");
  }

} // set_strand()



void set_strandfav() {                                        // Set a couple of strand favourites.

  if(strandActive == 1) {
    if (IRCommand == IR_E1) EEPROM.write(FAV1, ledMode);
    if (IRCommand == IR_E4) EEPROM.write(FAV2, ledMode);
  }  // if strandActive
  IRProtocol = 0;                                             // Let's clear the the IRProtocol flag and be ready for another command.
} // set_strandfav()





void set_strandlen() {                                                // Setting our strand length with button presses.

  Serial.println("Setting strand length.");
  
  if(strandActive == 1) {                                               // Only do this if the strand is active.
    demorun = 0;                                                        // First we disable the demo mode.
    ledMode = 0;                                                        // And set to mode 0 (black).
    fill_solid(leds,MAX_LEDS, CRGB::Black);                             // Let's make it black manually.
    
    if (IRCommand == IR_B2) {
      NUM_LEDS--; if (NUM_LEDS >255) NUM_LEDS=0;                        // Don't be stupid with our strand length selection.
    } else {
     NUM_LEDS++;  if (NUM_LEDS >= MAX_LEDS) NUM_LEDS=MAX_LEDS-1;        // Again, don't be stupid with our strand length selection.
    }
    fill_solid(leds,NUM_LEDS,CRGB(255,255,255));                        // Turn on the number of LEDs that we have selected as our length.
    EEPROM.write(STRANDLEN,NUM_LEDS);                                   // Write that value to EEPROM.
    Serial.print("Writing IR: "); Serial.print(NUM_LEDS); Serial.println(" LEDs");
  }
} // set_strandlen()



void set_meshdel() {                                                    // Setting our notamesh delay for whatever strands are powered up.

  if(strandActive == 1) {                                               // Only do this if the strand is active.
    demorun = 0;                                                        // First we disable the demo mode.
    ledMode = 0;                                                        // And set to mode 0 (black).
    fill_solid(leds,MAX_LEDS, CRGB::Black);                             // Let's make it black manually.
    
    if (IRCommand == IR_E2) {                                           // Which button did we press (either E2 or E3).
      meshdelay = meshdelay - 1;
      if (meshdelay >10000) meshdelay = 0;                              // Again, don't be stupid with our buttons.
    } else {
     meshdelay = meshdelay + 1;                                         // Increase the delay as much as you want. . .
    }
    
    fill_solid(leds,meshdelay,CRGB(255,255,255));                       // Turn on the number of LED's that we have selected (100ms is 1 LED)
    EEPROM.write(STRANDEL,meshdelay);                                   // Write out the delay to EEPROM.
    Serial.print("Writing IR: "); Serial.print(meshdelay*100); Serial.println("ms delay.");
  } // if strandActive
  
} // set_meshdel()


