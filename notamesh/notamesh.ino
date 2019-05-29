/*
 * notamesh - IR controlled 'synchronized' led lighting effects using FastLED across multiple Arduino AVR based controllers.
 * 
 *       By: Andrew Tuline
 *     Date: May, 2019 (latest update)
 *      URL: www.tuline.com
 *    Email: atuline@gmail.com
 *     Gist: https://gist.github.com/atuline
 *   GitHub: https://github.com/atuline
 * Pastebin: http://pastebin.com/u/atuline
 *  Youtube: https://www.youtube.com/user/atuline/videos
 * 
 *  FastLED Support Forums: https://www.reddit.com/r/FastLED/
 *  
 *  
 * This works best with Arduino Nano, a TSOP38238 IR receiver and APA102 LED's. It also works well with an ESP8266 and WS2812's.
 * 
 * It is also being tested with ESP8266 based WeMOS D1 Mini along with the TSOP38238 IR receiver and WS2812 LED's.
 * 
 * CAUTION ************************************************************************************************************************************************
 * 
 * Before attempting to compile this program, make sure you are already comfortable programming Arduino C and with FastLED, In addition, you
 * should already be able to download, install and USE 3rd party libraries. If you are a beginner, this is NOT the code you're looking for.
 * 
 * Effects in this program are programmed for up to 255 LED's. You will need to re-write it for longer strips.
 * 
 * These have been tested thoroughly on an Arduino Nano, but not other platforms/ On the Nano, IR control works great with APA102's, but is unreliable
 * with WS2812's. It works more reliably with WS2812's on the ESP8266.
 * 
 * 
 * INTRODUCTION *******************************************************************************************************************************************
 *  
 * Here's a pile of display routines for Arduino microcontrollers (tested on a Nano) and addressable LED strips using the FastLED display library.
 * Each Arduino is IR controlled using Nico Hood's IR control library.
 * 
 * 
 * My LED animation design philosophy is to:
 * 
 * Do NOT use delay statements in the loop or display routines as that breaks input routines (i.e. buttons, etc).
 * No nested loops (for performance reasons).
 * Use millis() as a realtime counter, unless a fixed counter is otherwise required.
 * Spend a bit more time on high school math, rather than the elementary school arithmetic method of counting pixels.
 * Keep the display routines as short as possible.
 * Keep data typing as strict as possible i.e. why define an int when a uint8_t is all that is required, and no floats at all.
 * Localize variables that are only used in individual routines, so don't define a global variable that's only used in a single function somewhere.
 * Global variables are used by multiple routines.
 * Break out the display routines into separate .h files for increased readability.
 * Be generous with comments and documentation.
 * 
 * 
 * Versions ***********************************************************************************************************************************************
 * 
 * TODO
 * 
 * Start focusing on ESP8266 and later ESP32 functionality. This is probably the end of the road for AVR's.
 * 
 * 
 * 1.09 Update
 * 
 * Add a couple of phase shifting routines.
 * Make Matrix simpler with a sawtooth wave instead of a line mover. . resulting in simpler and less code.
 * Added a monocolor palette that's selectable with the HUE/SATURATION buttons.
 * Remove routines that don't support palettes. Palettes are 'the standard'.
 * Remove _PAL references in file names and function call. After all, who uses CHSV these days? :D
 * 
 * 
 * 1.08 Update
 * 
 * Retrofitted with macro's to work with ESP8266 based WEMOS D1 Mini, specifically EEPROM functionality.
 * Also added a 'lamp' mode 0, which has adjustable hue and saturation.
 * Changed keypresses around to better support the new lamp mode.
 * 
 * 
 * 1.07 Update
 * 
 * I forgot to update some of the IR functionality for EEPROM with strand length and mesh delay. That's now fixed.
 * Also added more Serial.println statements for button presses.
 * Retrofit some updates that I'd made for notasound.
 * 
 * 
 * 1.06 Update *********************************
 * 
 * Add support for IRLRemote version 2.0.2, which now supports WS2812's (unreliably) with IRLRemote.
 * Add F("text") to Serial.print statements to save on RAM.
 * There have been a few other fixes added to notasound, which I haven't backported to this routine. That's what happens when I make $0 off of this.
 * 
 * 
 * 1.05 Update *********************************
 * 
 * Set/display two favourites . . . which has since been removed because the keypad is getting pretty busy.
 * 
 * 
 * 1.04 Update *********************************
 * 
 * Make programming length and delay to be unit selectable.
 * Change demo mode to start at ledMode 2.
 * Set default to non-demo mode.
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
 * HARDWARE SETUP for Arduino Nano **********************************************************************************************************************
 * 
 * This has been developed using an Arduino Nano and adapted for a WeMOS D1 Mini.
 * The LED data line is connected to pin 12.
 * The LED clock line is connected to pin 11.
 * For IR functionality, connect the data output of a TSOP34838 38kHz IR receiver to pin D2 (other TSOP pins are 5V and Gnd).
 * The IR NOW supports WS2812 LED's (but not very well).
 * This is currently configured to support up to 64 LED's. You can increase this by changing MAX_LEDS, up to 255 (not tested) with adequate memory.
 * Serial port has been configured to 115200 for debugging.
 * 
 * 
 * HARDWARE SETUP for WeMOS D1 Mini **********************************************************************************************************************
 * 
 * The LED data line is connected to pin D5
 * For IR functionality, connect the data output of a TSOP34838 38kHz IR receiver to pin D2 (other TSOP pins are 5V and Gnd).
 * The IR NOW supports WS2812 LED's.
 * This is currently configured to support up to 64 LED's. You can increase this by changing MAX_LEDS, up to 255 (not tested).
 * Serial port has been configured for 115200 for debugging.
 * 
 * You need to connect a pullup resistor on GPIO0 (or pin D3 on the WeMOS D1 Mini) to 3.3V and a pulldown resistor to GPIO15 (or pin D8 on the WeMOS D1 Mini)
 * 
 * Also, if you flash the ESP8266, you should cycle the power or press the restart button before ESP.restart() will work correctly. It's a known sporadic issue with ESP8266.
 * 
 * 
 * EEPROM Functionality ***********************************************************************************************************************************
 * 
 * Save the ID of the strand so that length and meshdelay can be programmed individually. 
 * Save the brightness in EEPROM, adjustable * or / 2.
 * Save the mesh delay in EEPROM by 100ms at a time. 
 * Save the direction in EEPROM, adjustable +/-1.
 * Save the startup favourite mode. Click on current display.
 * Save the glitter in EEPROM, adjustable toggle.
 * Save the strand length in EEPROM, however the MAXIMUM strand length is pre-defined. We just use a lesser value for the array.
 * Save the startup display mode in EEPROM.
 * Save the current palette, Adjustable +/-.
 * Save the current speed. Adjustable +/-.
 * Save the hue for lamp mode in EEPROM, adjustable +4.
 * Save the saturation for lamp mode in EEPROM, adjustable +4.
 * 
 * 
 * Libraries Required (to download) ***********************************************************************************************************************
 * 
 * FastLED library from https://github.com/FastLED/FastLED
 * Nico Hood's IRL library from https://github.com/NicoHood/IRLremote
 * 
 * Both libraries work with ESP8266 as well as UNO/Nano.
 *
 * You don't need to download the EEPROM.H library.
 * 
 *
 * Compile Time Configuration ******************************************************************************************************************************
 * 
 * Configure the define for ESP8266 or NANO/UNO (or whatever you're using) at the start of the program.
 * Configure and compile notamesh for your type of strand, microphone, IR and LED pins used.
 * Configure the STRANDID value for your Arduino (corresponding to a keypress value). Each Arduino should have a unique STRANDID, and you should write that down.
 * Set the demorun startup variable to 1 for demo mode or 0 for regular use. It's configured to 0 (non-demo mode) by default.
 *
 *
 * First Time Initialization ******************************************************************************************************************************
 * 
 * The first time notamesh runs on an Arduino, the setup program will initialize the EEPROM so that:
 * 
 * An Arduino initialization flag will be set.
 * The starting mode will be 0 (red).
 * The starting NUM_LEDS length will be 20 LED's.
 * The starting notamesh delay will be 0 ms (this only works with notamesh).
 * 
 * If you want to re-initialize the EEPROM values, then change the value of INITVAL at compile time. You can also reboot twice within 5 seconds.
 * 
 * 
 * Notamesh Initial Configuration ************************************************************************************************************************
 * 
 * 1) Set your strand length.
 * 
 * You'll need to 'activate' your strand for EEPROM programming. To do so:
 * 
 * - Press B1 to put the Arduino into 'Select strand' mode.
 * - Press the button equivalent to your STRANDID, i.e. C1 to 'activate' your Arduino for EEPROM programming.
 * - Press B2 to decrease the strand length.
 * - Press B3 to increase the strand length.
 * 
 * LED's will light up as 'white' to indicate the strand length. The strand length will be saved to EEPROM after each keypress.
 * Once done, press A3 to commit the changes and reset the Arduino.
 * 
 * 
 * 2) To increase/decrease the mesh delay (which works best with notamesh):
 * 
 * Again, you'll need to 'activate' your strand for EEPROM programming. To do so:
 * 
 * - Press B1 to put the Arduino into 'Select strand' mode.
 * - Press the button equivalent to your STRANDID, i.e. C1 to 'activate' your Arduino for EEPROM programming.
 * - Press E2 to decrease the amount of mesh delay by 100ms.
 * - Press E3 to increase the amount of mesh delay by 100ms.
 * 
 * LED's will light up as 'white' to indicate the mesh delay (1 led per 100ms). The mesh delay will be saved to EEPROM after each keypress.
 * Once done, press A3 to commit the changes and reset the Arduino.
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
 * Note: On an ESP8266, any changes to EEPROM require that the Arduino be rebooted once the changes have been made.
 * 
 * 
 * Command                          IR Button location & description
 * --------                         --------------------------------
 * Increase brightness              A1  Increase brightness
 * Decrease brightness              A2  Decrease brightness
 * Reboot and commit EEPROM changes A3  Reboots the Arduino in order to sync millis() if using notamesh. Factory reset if < 5 seconds. Also commits any changes to EEPROM.
 * Enable demo mode                 A4  Demo mode cycles through the routines based on the millis() counter. Not a toggle.
 * 
 * Select Arduino                   B1  This allows the EEPROM to be updated. Then press A1 through F4 as configured with STRANDID at compile time. (not A3 or B1 though). Press B1 again to get out of Select mode or A3 to reboot.
 * Decrease strand length           B2  The # of LED's programmed are white, only if strand is active (via B1 & STRANDID). This is saved in EEPROM. Press B1 again or A3 to reboot when done.
 * Increase strand length           B3  The # of LED's programmed are white, only if strand is active (via B1 & STRANDID). This is saved in EEPROM. Press B1 again or A3 to reboot when done.
 * Shorter mesh delay               B4  Decrease mesh delay by 100ms before starting (using white LED's), only if strand is active (with the Select Arduino command). This is saved in EEPROM. Press B1 again or A3 to reboot when done.
 * 
 * Longer mesh delay                C1 Increase mesh delay by 100ms before starting (using white LED's), only if strand is active (with the Select Arduino command). This is saved in EEPROM. Press B1 again or A3 to reboot when done. 
 * Slower animation                 C2  Increase value of EVERY_N_MILLISECONDS() for the current (fixed) display mode.
 * Faster animation                 C3  Decrease value of EVERY_N_MILLISECONDS() for the current (fixed) display mode.
 * Toggle direction                 C4 Toggle direction of some sequences.
 * 
 * // Enable/disable glitter           D1  Toggles glitter on some sequences. Removed for now.
 * 
 * Enable hue based palette         D1  Select palette that is hue/saturation changeable. 
 * Previous display mode            D2  Also disables demo mode.
 * Next display mode                D3  Also disables demo mode.
 * Save Current mode to EEPROM      D4  This will be the startup mode, and disables demo mode temporarily. This is saved in EEPROM.
 * 
 * Decrease hue colour              E1  Decrease the hue colour for lamp mode. This is saved in EEPROM.
 * Decrease hue saturation          E2  Decrease the hue saturation for lamp mode. This is saved in EEPROM.
 * Increase hue saturation          E3  Increase the hue saturation for lamp mode. This is saved in EEPROM.
 * Increase hue colour              E4  Increase the hue colour for lamp mode. This is saved in EEPROM.
 * 
 * Stop palette rotation            F1   Stop palette rotation at current palette. Save palette to EEPROM.
 * Select previous palette          F2   Stop palette rotation and select previous palette immediately. Save palette to EEPROM.
 * Select next palette              F3   Stop palette rotation and select next palette immediately. Save Palette to EEPROM.
 * Enable palette rotation          F4   Enable palette transitioning every 5 seconds.
 * 
 * 
 * 
 */


/*------------------------------------------------------------------------------------------
--------------------------------------- Start of variables ---------------------------------
------------------------------------------------------------------------------------------*/


//#define _ESP8266                                            // Uncomment this if using ESP8266. It's required for EEPROM changes between the platforms.
#define _NANO                                               // Uncomment this if using UNO/Nano or similar AVR.

#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.


#define NOTAMESH_VERSION 109                                  // Current version of this program.

#include "FastLED.h"                                          // https://github.com/FastLED/FastLED
#include "IRLremote.h"                                        // https://github.com/NicoHood/IRLremote
#include "EEPROM.h"                                           // This is included with base install. Uses a different one depending on the architecture.

#include "commands.h"                                         // The IR commands.

#if IRL_VERSION < 202
#error "Requires IRLRemote 2.0.2 or later. Check github for latest code."
#endif

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later. Check github for latest code."
#endif


#define pinIR 2                                               // Choose a valid pin of your Arduino board for IR operations. In this case, 2 on the Nano and D2 on the ESP8266.
#define IRL_BLOCKING true                                     // Never did test this.
uint16_t IRAddress;                                           // A 16 bit number that's the same for each controller. A value of 65535 is a generic release of button value.
uint8_t IRCommand;                                            // An 8 bit numeric value of the key press.

CNec IRLremote;                                               // Create our object with the NEC configuration and call it IRLremote.


// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for all strands. I use pin 12 on a Nano and D5 on the ESP8266.
#define LED_CK 11                                             // Serial clock pin for WS2801 or APA102
#define COLOR_ORDER GRB                                       // It's GRB for WS2812, BGR for APA102
#define LED_TYPE WS2812                                       // Alternatively WS2801, or WS2812
#define MAX_LEDS 64                                           // Maximum number of LED's defined (at compile time). 


// Initialize changeable global variables.
uint8_t NUM_LEDS = 40;                                        // Number of LED's we're actually using, and we can change this only the fly for the strand length.

uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[MAX_LEDS];                                   // Initialize our LED array. We'll be using less in operation.

 CRGBPalette16 CurrentPalette;                                // This uses palettes instead of direct CHSV or CRGB assignments for many routines.
 CRGBPalette16 TargetPalette;                                 // It also support smooth palette transitioning.

TBlendType currentBlending = LINEARBLEND;                     // NOBLEND or LINEARBLEND for palettes.

extern const TProgmemRGBGradientPalettePtr GradientPalettes[]; // These are for the fixed palettes in gradient_palettes.h.
extern const uint8_t GradientPaletteCount;                     // Total number of fixed palettes to display.
uint8_t CurrentPaletteNumber = 0;                              // Current palette number from the 'playlist' of color palettes.
uint8_t currentPatternIndex = 0;                               // Index number of which pattern is current.



// EEPROM location definitions.
#define ISINIT    0                                           // EEPROM location used to verify that this Arduino has been initialized.
#define STARTMODE 1                                           // EEPROM location for the starting mode.
#define STRANDLEN 2                                           // EEPROM location for the actual Length of the strand, which is < MAX_LEDS
#define STRANDEL  3                                           // EEPROM location for the mesh delay value.

#define STARTPAL  6                                           // EEPROM location of current palette. If !0, then rotation = 0.
#define GLIT      7                                           // EEPROM location for glitter.
#define BRIT      8                                           // EEPROM location is brightness.
#define SPED      9                                           // EEPROM location of speed.
#define DIRN      10                                          // EEPROM location of direction.
#define LHUE      11                                          // EEPROM location of lamp hue.
#define LSAT      12                                          // EEPROM location of lamp saturation.
#define DEMO      13                                          // EEPROM location of demo mode toggle.

#define INITVAL   0x57                                        // If this is the value in ISINIT, then the Arduino has been initialized. Change to completely reset your Arduino.

#define INITBRIT 128                                          // Initial max_bright.
#define INITDEL  0                                            // Starting mesh delay value of the strand in milliseconds.
#define INITDIRN 1                                            // Initial thisdir value.
#define INITGLIT 0                                            // Glitter is off by default.
#define INITLEN  20                                           // Start length is 20 LED's.
#define INITMODE 0                                            // Startmode is 0, which is black.
#define INITPAL  0                                            // Starting palette number.
#define INITSPED 0                                            // Initial thisdelay value.
#define INITLHUE 0                                            // Initial lamphue value.
#define INITLSAT 1                                            // Initial lampsat value.
#define INITDEMO 0                                            // Initial demo mode value.


const uint32_t STRANDID = IR_C1;                              // This is the ID button of the strand and should be unique for each strand in a series (if you want them to be different).
bool strandActive = false;                                    // 0=inactive, 1=active. Must be activated by button press of B1, then B3 (or the appropriate button).
bool strandFlag = false;                                      // Flag to let us know if we're changing the active strand.

uint16_t meshdelay = 0;                                       // Timer for the notamesh. Works with INITDEL.

uint8_t ledMode = 3;                                          // Starting mode is typically 0. Change INITMODE if you want a different starting mode.
uint8_t demorun = 0;                                          // 0 = regular mode, 1 = demo mode, 2 = shuffle mode.
uint8_t maxMode = 38;                                         // Maximum number of modes.
uint8_t demotime = 10;                                        // Set the length of the demo timer.


// Generic/shared routine variables ----------------------------------------------------------------------
uint8_t allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
uint8_t bgclr = 0;                                            // Generic background colour
uint8_t bgbri = 0;                                            // Generic background brightness
bool    glitter = false;                                      // Glitter flag
uint8_t numdots = 5;                                          // Number of LED's in the matrix or other similar routine.
uint8_t palchg = 3;                                           // Palette changes are: 0=no change, 1=previous, 2 = next, 3 = continuous change, 4 = hue/saturation palette
uint8_t startindex = 0;
uint8_t thisbeat;                                             // Standard beat
uint8_t thisbright = 255;                                     // Standard brightness
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
uint16_t thisdelay = 0;                                       // Standard delay
uint8_t thisdiff = 1;                                         // Standard palette jump
int8_t  thisdir = 1;                                          // Standard direction is either -1 or 1. Used as a multiplier rather than boolean.
uint8_t thisfade = 224;                                       // Standard fade rate
uint8_t thishue = 0;                                          // Standard hue
uint8_t thisindex = 0;                                        // Standard palette index
uint8_t thisinc = 1;                                          // Standard incrementer
uint8_t thisscale = 30;                                       // Scale for use with noise.
uint16_t thisdist = 1234;                                     // Distance for use with noise.
int     thisphase = 0;                                        // Standard phase change
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t thissat = 255;                                        // Standard saturation
int8_t  thisspeed = 4;                                        // Standard speed change
uint8_t wavebright = 255;                                     // You can change the brightness of the waves/bars rolling across the screen.
uint8_t lamphue = 0;                                          // Hue value of lamp mode.
uint8_t lampsat = 1;                                          // Saturation value of lamp mode.
uint16_t loops = 0;                                           // showfps counter.

uint8_t xd[MAX_LEDS];                                         // arrays for the 2d coordinates of any led
uint8_t yd[MAX_LEDS];

long summ=0;



// Display functions -----------------------------------------------------------------------

// Support functions
#include "support.h"
// #include "make_palettes.h"                                   // Removed from notamesh. We'll use gradient_palettes.h instead, so that we always know what palette we have at all times (unless otherwise changed).
#include "gradient_palettes.h"

// Display routines
#include "confetti.h"
#include "fadein.h"
#include "juggle.h"
#include "matrix_saw.h"
#include "one_sine_phase.h"
#include "noise8.h"
#include "one_sin.h"
#include "plasma.h"
#include "rainbow_march.h"
#include "ripple.h"
#include "serendipitous.h"
#include "three_sin.h"
#include "two_sin.h"
#include "two_sin_phase.h"



/*------------------------------------------------------------------------------------------
--------------------------------------- Start of code --------------------------------------
------------------------------------------------------------------------------------------*/


void setup() {

  Serial.begin(115200);                                                           // Setup serial baud rate
  delay(1000);                                                                    // Power up safety delay
  
  Serial.println(F(" ")); Serial.println(F("---SETTING UP notamesh---"));         // Let's get ready to rumble!

  if (!IRLremote.begin(pinIR))                                                    // Initializing IRLRemote by Nico Hood.
    Serial.println(F("You did not choose a valid pin."));
    
//  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER >(leds, MAX_LEDS);         // APA102 or WS2801 definition
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER >(leds, MAX_LEDS);                   // WS2812 definition
  
  set_max_power_in_volts_and_milliamps(5, 750);                                   // 5V, 750 mA maximum power draw.

  #if defined(_ESP8266)
  EEPROM.begin(32);                                                               // Not used by the UNO/Nano, but critical for ESP8266.
  #endif

  if (EEPROM.read(ISINIT) != INITVAL) {                                           // Check to see if Arduino has been initialized, and if not, do so. A cold reset as it were.
    EEPROM.write(ISINIT, INITVAL);                                                // Initialize the starting value (so we know it's initialized) to INITVAL.

    EEPROM.write(BRIT, INITBRIT);                                                 // Initialize the brightness value.
    EEPROM.write(STRANDEL, INITDEL);                                              // Initialize the notamesh delay to 0.
    EEPROM.write(DIRN, INITDIRN);                                                 // Initial thisdir value.
    EEPROM.write(GLIT, INITGLIT);                                                 // Initialize glitter value.
    EEPROM.write(STRANDLEN, INITLEN);                                             // Initialize the starting length to 20 LED's.
    EEPROM.write(STARTMODE, INITMODE);                                            // Initialize the starting mode to 0.
    EEPROM.write(STARTPAL, INITPAL);                                              // Initialize the palette to 0.
    EEPROM.write(SPED, INITSPED);                                                 // Initial thisdelay value.
    EEPROM.write(LHUE, INITLHUE);                                                 // Initial lamphue value.
    EEPROM.write(LSAT, INITLSAT);                                                 // Initial lampsat value.
    EEPROM.write(DEMO, INITDEMO);
    Serial.println(F("Cold reset."));
  }

  ledMode = EEPROM.read(STARTMODE);                                               // Location 0 is the starting mode.
  NUM_LEDS = EEPROM.read(STRANDLEN);                                              // Need to ensure NUM_LEDS < MAX_LEDS elsewhere.
  meshdelay = EEPROM.read(STRANDEL);                                              // This is our notamesh delay for cool delays across strands.
  glitter = EEPROM.read(GLIT);                                                    // notamesh glitter.
  max_bright = EEPROM.read(BRIT);                                                 // max_bright value.
  thisdelay = EEPROM.read(SPED);                                                  // thisdelay value.
  thisdir = EEPROM.read(DIRN);                                                    // thisdir value.
  lamphue = EEPROM.read(LHUE);                                                    // lamphue value.
  lampsat = EEPROM.read(LSAT);                                                    // lampsat value.
  demorun = EEPROM.read(DEMO);
 
  if (EEPROM.read(STARTPAL) != 0) {CurrentPaletteNumber = EEPROM.read(STARTPAL); palchg = 0;}

  Serial.println(F("---EEPROM COMPLETE---"));

  Serial.print(F("Initial mesh delay: ")); Serial.print(meshdelay*100); Serial.println(F("ms delay"));
  Serial.print(F("Initial strand length: ")); Serial.print(NUM_LEDS); Serial.println(F(" LEDs"));
  Serial.print(F("Strand ID: ")); Serial.println(STRANDID);
  Serial.print(F("Glitter: ")); Serial.println(glitter);
  Serial.print(F("Brightness: ")); Serial.println(max_bright);
  Serial.print(F("Delay: ")); Serial.println(thisdelay);
  Serial.print(F("Direction: ")); Serial.println(thisdir);
  Serial.print(F("Palette: ")); Serial.println(CurrentPaletteNumber);
  Serial.print(F("Demo: ")); Serial.println(demorun);

  LEDS.setBrightness(max_bright);                                                 // Set the generic maximum brightness value.
  
  CurrentPalette = CRGBPalette16(CRGB::Black);
  TargetPalette = (GradientPalettes[0]);

  strobe_mode(ledMode, 1);                                                        // Initialize the first sequence

  Serial.println(F("---SETUP COMPLETE---")); Serial.println(F(" "));

} // setup()



//------------------MAIN LOOP---------------------------------------------------------------
void loop() {
  
  getirl();                                                                   // Read a command from the IR LED and process command as required.
  
  demo_check();                                                               // If we're in demo mode, check the timer to see if we need to increase the strobe_mode value.

  EVERY_N_MILLISECONDS(50) {                                                  // Smooth palette transitioning runs continuously.
    if (palchg == 4) TargetPalette = CRGBPalette16(CHSV(lamphue, lampsat, 255), CHSV(lamphue, lampsat, 128), CHSV(lamphue, lampsat, 64), CHSV(lamphue, lampsat, 32));  // High speed palette change for hue/saturation palette.
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(CurrentPalette, TargetPalette, maxChanges);  // Our standard palette transitioning over 24 steps.
  }

  EVERY_N_SECONDS(5) {                                                        // If selected, change the target palette to a random one every 5 seconds.
    if (palchg==3) {
      CurrentPaletteNumber = addmod8(CurrentPaletteNumber, 1, GradientPaletteCount);
    }
    if (palchg != 4) {                                                        // Palchg = 4 is the one that's hue/saturation, and we don't want to do anything to that one.
      TargetPalette = GradientPalettes[CurrentPaletteNumber];                 // We're just ensuring that the TargetPalette WILL be assigned.
    }
  }
  
  EVERY_N_MILLIS_I(thistimer, thisdelay) {                                    // Sets the original delay time.
    thistimer.setPeriod(thisdelay);                                           // This is how you update the delay value on the fly.
    strobe_mode(ledMode, 0);                                                  // Strobe to display the current sequence, but don't initialize the variables, so mc=0;
  }

  if(glitter) addGlitter(10);                                                 // If the glitter flag is set, let's add some. This has been deprecated for now.
  
  if (!IRLremote.receiving()) {
    FastLED.show();                                                           // Power managed display of LED's.
  }
  
} // loop()



//-------------------OTHER ROUTINES----------------------------------------------------------
void strobe_mode(uint8_t newMode, bool mc){                   // mc stands for 'Mode Change', where mc = 0 is strobe the routine, while mc = 1 is change the routine

  if(mc) {
    fill_solid(leds,NUM_LEDS,CRGB(0,0,0));                    // Clean up the array for the first time through. Don't show display though, so you may have a smooth transition.
    Serial.print(F("Mode: ")); 
    Serial.println(newMode);
  }

  if (!strandActive) {                                          // Stops the display sequence if we're updating the EEPROM in ACTIVE mode.
    switch (newMode) {                                          // First time through a new mode, so let's initialize the variables for a given display.
  
      case  0: if(mc) {fill_solid(leds,NUM_LEDS,CHSV(lamphue, lampsat, 255));} fill_solid(leds,NUM_LEDS,CHSV(lamphue, lampsat, 255)); break;                     // Lamp mode.
      case  1: if(mc) {thisdelay=20; thisspeed=-1; allfreq=32; thiscutoff=192; thisrot=16;} one_sine_phase(); break;
      case  2: if(mc) {thisdelay=10; allfreq=16; thisspeed=1; thatspeed=2; thishue=0; thathue=128; thisrot=1; thatrot=1; thiscutoff=128; thatcutoff=192;} two_sin(); break;
      case  3: if(mc) {thisdelay=20; allfreq=4; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin(); break;
      case  4: if(mc) {thisdelay=10; thisscale=30; } noise8(); break;
      case  5: if(mc) {thisdelay=10; allfreq=4; thisspeed=-1; thatspeed=0; thishue=64; thathue=192; thisrot=0; thatrot=0; thiscutoff=64; thatcutoff=192;} two_sin(); break;
      case  6: if(mc) {thisdelay=20; allfreq=10; bgclr=64; bgbri=4; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin(); break;
      case  7: if(mc) {thisdelay=10; numdots=2; thisfade=16; thisbeat=8; thisbright=255; thisdiff=64;} juggle(); break;
      case  8: if(mc) {thisdelay=20; thisscale=30; thisdir=-1; thisfade=16; numdots=5; thisbright=255; } matrix_saw(); break;
      case  9: if(mc) {thisdelay=10; allfreq=6; thisspeed=2; thatspeed=3; thishue=96; thathue=224; thisrot=0; thatrot=0; thiscutoff=64; thatcutoff=64;} two_sin(); break;
      case 10: if(mc) {thisdelay=40;} ripple(); break;
      case 11: if(mc) {thisdelay=50; mul1=5; mul2=8; mul3=7;} three_sin(); break;
      case 12: if(mc) {thisdelay=10;} serendipitous(); break;
      case 13: if(mc) {thisdelay=0;} fadein(); break;
      case 14: if(mc) {thisdelay=10; allfreq=20; thisspeed=2; thatspeed=-1; thishue=24; thathue=180; thisrot=0; thatrot=1; thiscutoff=64; thatcutoff=128;} two_sin(); break;
      case 15: if(mc) {thisdelay=20; thisscale=20; thisdir=1; thisfade=24; numdots=5; thisbright=255; } matrix_saw(); break;
      case 16: if(mc) {thisdelay=10; thisscale=40;} noise8(); break; // By: Andrew Tuline
      case 17: if(mc) {thisdelay=10;} plasma(11,23,4,18); break;
      case 18: if(mc) {thisdelay=20; allfreq=10; thisspeed=1; thatspeed=-2; thishue=48; thathue=160; thisrot=1; thatrot=-1; thiscutoff=128; thatcutoff=192;} two_sin(); break;
      case 19: if(mc) {thisdelay=50; mul1=6; mul2=9; mul3=11;} three_sin(); break;
      case 20: if(mc) {thisdelay=10; thisrot=1; thisdiff=1;} rainbow_march(); break;
      case 21: if(mc) {thisdelay=10; thisrot=2; thisdiff=10;} rainbow_march(); break;
      case 22: if(mc) {thisdelay=40; thisspeed=2; allfreq = 16; thiscutoff=192; thisrot=16;} one_sine_phase(); break;
      case 23: if(mc) {thisdelay=20; allfreq=6; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin(); break;
      case 24: if(mc) {thisdelay=10;} plasma(23,15,6,7); break;
      case 25: if(mc) {thisdelay=20; thisinc=1; thishue=192; thissat=255; thisfade=2; thisdiff=32; thisbright=255;} confetti(); break;
      case 26: if(mc) {thisdelay=10; thisspeed=2; thatspeed=3; thishue=96; thathue=224; thisrot=1; thatrot=2; thiscutoff=128; thatcutoff=64;} two_sin(); break;
      case 27: if(mc) {thisdelay=20; thisscale=5; thisdir=-1; thisfade=32; numdots=5; thisbright=255; } matrix_saw(); break;
      case 28: if(mc) {thisdelay=20; allfreq=20; bgclr=0; bgbri=0; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=224; thisrot=0; thisspeed=4; wavebright=255;} one_sin(); break;
      case 29: if(mc) {thisdelay=20; thisinc=2; thishue=128; thisfade=8; thisdiff=64; thisbright=255;} confetti(); break;
      case 30: if(mc) {thisdelay=10;} plasma(8,7,9,13); break;
      case 31: if(mc) {thisdelay=10; numdots=4; thisfade=32; thisbeat=12; thisbright=255; thisdiff=20;} juggle(); break;
      case 32: if(mc) {thisdelay=30; allfreq=4; bgclr=64; bgbri=4; thisbright=255; startindex=64; thisinc=2; thiscutoff=224; thisphase=0; thiscutoff=128; thisrot=1; thisspeed=8; wavebright=255;} one_sin(); break;
      case 33: if(mc) {thisdelay=50; mul1=3; mul2=4; mul3=5;} three_sin(); break;
      case 34: if(mc) {thisdelay=10; thisrot=1; thisdiff=5;} rainbow_march(); break;
      case 35: if(mc) {thisdelay=10;} plasma(11,17,20,23); break;
      case 36: if(mc) {thisdelay=20; thisinc=1; thishue=random8(255); thisfade=1; thisbright=255;} confetti(); break;
      case 37: if(mc) {thisdelay=10; thisspeed=2; allfreq=8; thiscutoff=224; thisrot=8;} one_sine_phase(); break;
      case 38: if(mc) {thisdelay=10; thisscale=20; } noise8(); break;
  
    } // switch newMode
  } // !strandActive  
  
} // strobe_mode()



void demo_check(){                                                // Are we in demo mode or not, and if so, change the routine every 'demotime' seconds.
  
  if(demorun) {                                                   // Is the demo flag set? If so, let's cycle through them.
    uint8_t secondHand = (millis() / 1000) % (maxMode*demotime);  // Adjust for total time of the loop, based on total number of available modes.
    static uint8_t lastSecond = 99;                               // Static variable, means it's only defined once. This is our 'debounce' variable.
    if (lastSecond != secondHand) {                               // Debounce to make sure we're not repeating an assignment.
      lastSecond = secondHand;
        if(secondHand%demotime==0) {                              // Every 10 seconds.
          if(demorun == 2) ledMode = random8(0,maxMode); else {   // Shuffle mode, which is not used.
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

  if (IRLremote.available()) {

    auto irdata = IRLremote.read();                               // Get the new data from the remote
    IRAddress = irdata.address;
    IRCommand = irdata.command;    

    if(strandFlag == 1) set_strand();                             // Set the strand length

//    Serial.print(F("Address: "));                               // Print the protocol data. Note that there's also 65535, which we don't use.
//    Serial.println(irdata.address);
//    Serial.print(F("Command: "));
//    Serial.println(irdata.command);
//    Serial.println(IRCommand);
//    Serial.println();

    if (IRAddress == IR_ADD) {
      switch(IRCommand) {
        
        case IR_A1:  max_bright=min(max_bright*2,255); EEPROM.write(BRIT, max_bright); Serial.print(F("Bright: ")); Serial.println(max_bright); LEDS.setBrightness(max_bright); break;  //a1 - Increase max bright
        case IR_A2:  max_bright=max(max_bright/2,1); EEPROM.write(BRIT, max_bright); Serial.print(F("Bright: ")); Serial.println(max_bright); LEDS.setBrightness(max_bright); break;    //a2 - Decrease max bright
        case IR_A3:  fill_solid(leds,NUM_LEDS,CRGB(0,0,0)); FastLED.show(); Serial.println(F("Rebooting . . ")); FastLED.delay(100); bootme(); break;                                   //a3 - Change to mode 0, display and reboot
        case IR_A4:  demorun = 1; EEPROM.write(DEMO, demorun); Serial.println(F("Demo mode")); break;                                                                                   //a4 - Enable demo mode
  
        case IR_B1:  Serial.println(F("Activate request")); set_strand(); break;     //b1 - Set Strand Active or Inactive for EEPROM programming.
        case IR_B2:  if (strandActive==1) set_strandlen(); break;                    //b2 - Decrease # of LED's and write to EEPROM
        case IR_B3:  if (strandActive==1) set_strandlen(); break;                    //b3 - Increase # of LED's and write to EEPROM
        case IR_B4:  if (strandActive==1) set_meshdel(); break;                      //b4 - Shorter mesh delay by 100ms
  
        case IR_C1:  if (strandActive==1) set_meshdel(); break;                                                                                                         //c1 - Longer mesh delay by 100ms
        case IR_C2:  thisdelay++; EEPROM.write(SPED, thisdelay); Serial.print(F("Delay: ")); Serial.println(thisdelay); break;                                          //c2 - Slow down the sequence as much as you want.
        case IR_C3:  thisdelay--; if(thisdelay >30000) thisdelay = 0; EEPROM.write(SPED, thisdelay); Serial.print(F("Delay: ")); Serial.println(thisdelay); break;      //c3 - Speed up the sequence, but don't go too far.
        case IR_C4:  thisdir = thisdir*-1; EEPROM.write(DIRN, thisdir); Serial.print(F("thisdir = ")); Serial.println(thisdir);  break;                                 //c4 - Change the direction of the LEDs
 
//        case IR_D1:  glitter = !glitter; EEPROM.write(GLIT, glitter); Serial.print(F("Glitter is: ")); Serial.println(glitter);   break;                              //d1 - Toggle glitter.

        case IR_D1:  palchg = 4; Serial.println(F("Hue/Palette based palette change.")); EEPROM.write(STARTPAL, 4); break;                                              //d1 - Hue/saturation palette.  
        case IR_D2:  demorun = 0;  EEPROM.write(DEMO, demorun); ledMode=(ledMode-1); if (ledMode==255) ledMode=maxMode; meshwait(); strobe_mode(ledMode,1); break;      //d2 - strobe_mode(ledMode--);
        case IR_D3:  demorun = 0;  EEPROM.write(DEMO, demorun); ledMode=(ledMode+1)%(maxMode+1); meshwait(); strobe_mode(ledMode,1); break;                             //d3 - strobe_mode(ledMode++);
        case IR_D4:  EEPROM.write(STARTMODE,ledMode); Serial.print(F("Writing startup mode: ")); Serial.println(ledMode);  break;                                       //d4 - Save startup mode

        case IR_E1:  lamphue -=4; EEPROM.write(LHUE,lamphue); Serial.print(F("Lamphue: ")); Serial.println(lamphue); break;                                             //e1 - Change lamphue
        case IR_E2:  lampsat -=4; EEPROM.write(LSAT, lampsat); Serial.print(F("Lampsat: ")); Serial.println(lampsat); break;                                            //e2 - Shorter mesh delay by 100ms
        case IR_E3:  lampsat +=4; EEPROM.write(LSAT, lampsat); Serial.print(F("Lampsat: ")); Serial.println(lampsat); break;                                            //e3 - Change lampsaturation
        case IR_E4:  lamphue +=4; EEPROM.write(LHUE,lamphue); Serial.print(F("Lamphue: ")); Serial.println(lamphue); break;                                             //e4 - Change lamphue

        case IR_F1:  palchg = 0; Serial.print(F("Stop and select current Palette: ")); EEPROM.write(STARTPAL, CurrentPaletteNumber); Serial.println(CurrentPaletteNumber); break;                                                                                               //f1 - Stop and select current Palette
        case IR_F2:  palchg = 1; Serial.print(F("Stop and select previous Palette: ")); CurrentPaletteNumber -= 1; if(CurrentPaletteNumber == 255) CurrentPaletteNumber = GradientPaletteCount; EEPROM.write(STARTPAL, CurrentPaletteNumber); Serial.println(CurrentPaletteNumber); CurrentPalette = (GradientPalettes[CurrentPaletteNumber]); break;    //f2 - Stop and select previous Palette
        case IR_F3:  palchg = 2; Serial.print(F("Stop and select next Palette: ")); CurrentPaletteNumber = addmod8( CurrentPaletteNumber, 1, GradientPaletteCount);  EEPROM.write(STARTPAL, CurrentPaletteNumber); Serial.println(CurrentPaletteNumber);  CurrentPalette = (GradientPalettes[CurrentPaletteNumber]); break;             //f3 - Stop and select next Palette
        case IR_F4:  palchg = 3; Serial.print(F("Continuous palette change: "));  Serial.println(CurrentPaletteNumber); EEPROM.write(STARTPAL, 0); break;                                                                                                    //f4 - Continuous palette change

        default:     break;                                         // We could do something by default, but hey, let's not.

      } // switch irdata.command
    } // if IR_ADD
  } // if IRLRemote

} // getirl()



void bootme() {                                                     // This is used to reset all the Arduinos so that their millis() counters are all in sync.

  if (millis() < 5000) {
    Serial.println(F("Factory Reset."));                            // If we reset within 5 seconds of startup, then it means factory reset.
    EEPROM.write(ISINIT, 0);
    delay(200);
  }

  #if defined(_ESP8266)    
  EEPROM.commit();                                                  // We need to actually write to the EEPROM on the ESP8266. Am not sprinkling this throughout the existing code, so here it sits.
  ESP.restart();
  #endif

  #if defined(_NANO)
  asm volatile("jmp 0");
  #endif
  
} // bootme()



void meshwait() {                                                   // After we press a mode key, we need to wait a bit for the sequence to start.

//  Serial.print(F("Mesh delay: ")); Serial.print(meshdelay*100); Serial.println(F("ms delay."));
  FastLED.delay(meshdelay*100);                                     // Here's our notamesh wait upon keypress. Oh god I'm so sorry there's a delay statement here. At least it's only used upon mode change keypress. Makes life a LOT simpler.

} // meshwait()



void set_strand() {                                                 // Setting the active strand. This logic is not great, so we have to reboot when done.

  if (IRAddress == IR_ADD && IRCommand == IR_B1) {
    strandFlag = !strandFlag;
    if (strandFlag == false) {IRCommand = 255; strandActive = 0; Serial.println(F("INACTIVE"));}
  }

  if (strandFlag == true && IRAddress == IR_ADD && IRCommand == STRANDID) {
    strandActive = 1; Serial.println(F("ACTIVE"));
  }

} // set_strand()



void set_strandlen() {                                                  // Setting our strand length with button presses.

  Serial.println(F("Setting strand length."));
  
  if(strandActive == 1) {                                               // Only do this if the strand is active.
    demorun = 0;                                                        // First we disable the demo mode.
    fill_solid(leds,MAX_LEDS, CRGB::Black);                             // Let's make it black manually.
    
    if (IRCommand == IR_B2) {
      NUM_LEDS--; if (NUM_LEDS >255) NUM_LEDS=0;                        // Don't be stupid with our strand length selection.
    } else {
     NUM_LEDS++;  if (NUM_LEDS >= MAX_LEDS) NUM_LEDS=MAX_LEDS-1;        // Again, don't be stupid with our strand length selection.
    }
    fill_solid(leds,NUM_LEDS,CRGB(64,64,64));                           // Turn on the number of LEDs that we have selected as our length.
    EEPROM.write(STRANDLEN,NUM_LEDS);                                   // Write that value to EEPROM.
    Serial.print(F("Writing IR: ")); Serial.print(NUM_LEDS); Serial.println(F(" LEDs"));
  }
} // set_strandlen()



void set_meshdel() {                                                    // Setting our notamesh delay for whatever strands are powered up.

  if(strandActive == 1) {                                               // Only do this if the strand is active.
    demorun = 0;                                                        // First we disable the demo mode.
    ledMode = 0;                                                        // And set to mode 0 (black).
    fill_solid(leds,MAX_LEDS, CRGB::Black);                             // Let's make it black manually.
    
    if (IRCommand == IR_B4) {                                           // Which button did we press (either B4 or C1).
      meshdelay = meshdelay - 1;
      if (meshdelay >10000) meshdelay = 0;                              // Again, don't be stupid with our buttons.
    } else {
     meshdelay = meshdelay + 1;                                         // Increase the delay as much as you want. . .
    }
    
    fill_solid(leds,meshdelay,CRGB(64,64,64));                          // Turn on the number of LED's that we have selected (100ms is 1 LED)
    EEPROM.write(STRANDEL,meshdelay);                                   // Write out the delay to EEPROM.
    Serial.print(F("Writing IR: ")); Serial.print(meshdelay*100); Serial.println(F("ms delay."));
  } // if strandActive
  
} // set_meshdel()
