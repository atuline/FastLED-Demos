
/* My FastLED Tips, Tricks and Traps

By: Andrew Tuline

Date: July, 2015

These notes are best viewed with an IDE such as Sublime Text. This is NOT a functional program, but a series of notes about using FastLED.

References:

https://github.com/FastLED/FastLED/wiki/Overview
http://fastled.io/docs/3.1/modules.html

Mark's additional demos:
https://gist.github.com/kriegsman

Other notable gists and pastebins:
https://gist.github.com/hsiboy
https://gist.github.com/jpro56
https://gist.github.com/jasoncoon
https://gist.github.com/StefanPetrick
https://gist.github.com/bonjurroughs

http://pastebin.com/u/atuline     (lots of junk in there)
*/



/* NOOB NOTES -----------------------------------------------------------------------------------------------------------

1) If you are trying to control LED's with a button or some other device, you will want the loop to run as fast as possible. Therefore:

- Try (really hard) to avoid using nested for loops.
- Try (really hard) to avoid using delay statement within any loops, and especially nested for loops.
- Try to avoid using the delay statement at all.
- If you MUST use a delay, try and keep it just within your main loop.
- Also, use this instead, as it doesn't stop the program from continuing to execute:

 EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    mydemo();
    FastLED.show();
  }

2) If you want to ensure you don't overload your battery, you might want to use power managed display. So, instead of:

    FastLED.show();

  Put the following in setup():

    set_max_power_in_volts_and_milliamps(5, 500);               // This is used by the power management functionality and is currently set at 5V, 500mA.

  Use the following to show the LED's in loop():
 
    show_at_max_brightness_for_power();
 

3) You can even use functions like beatsin88() mapped to NUM_LEDS to avoid using delays entirely.

4) Palettes. Learn to use them. Learn how to smoothly transition between them.

5) Use 8 and 16 bit math, and the FastLED math functions where possible. Floating point is s-l-o-w.

*/


// Basic Definition -----------------------------------------------------------------------------------------------------

  #include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

  #if FASTLED_VERSION < 3001000                                 // This guarantees the person will have to use FastLED 3.1
  #error "Requires FastLED 3.1 or later; check github for latest code."
  #endif
   
  // Fixed definitions cannot change on the fly.
  #define LED_DT 12                                             // Data pin to connect to the strip.
  #define LED_CK 11
  #define COLOR_ORDER BGR                                       // Use BGR for APA102 and GRB for WS2812
  #define LED_TYPE APA102                                       // Or WS2812. Don't forget to change the FastLED.addLeds line as well.
  #define NUM_LEDS 20                                           // Number of LED's.

  // Initialize changeable global variables.
  uint8_t max_bright = 64;                                      // Overall brightness definition. It can be changed on the fly, i.e. with a potentiometer.

  struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

  // VARIABLES - In general, use the smallest and most accurate variable definition you can, i.e. Localize them to your routine if you can.
  uint8_t myVar = 0;                                            // for a variable that ranges from 0 to 255.






// SETUP -------------------------------------------------------------------------------------------------------


  void setup () {

    delay(3000);                                                // If things go bad, you can shutdown before the LED's start drawing power.
    Serial.begin(57600);                                        // Get the serial port running for debugging

    FastLED.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);        // Use this for WS2812B
    FastLED.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
    FastLED.addLeds<LED_TYPE,LED_DT,LED_CK,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip).setDither(max_bright < 255);

    FastLED.setBrightness(max_bright);                          // You can change the overall brightness on the fly, i.e. with a potentiometer.
    set_max_power_in_volts_and_milliamps(5, 500);               // This is used by the power management functionality and is currently set at 5V, 500mA.


  // Optional randomization
    random16_set_seed(4832);                                    // Awesome randomizer
    random16_add_entropy(analogRead(2));
    int ranstart = random16();


  } // setup()



// LOOP -------------------------------------------------------------------------------------------------------

  void loop() {
    static int myvar = 5;                                       // The value is only initialized once. Very cool.
    int yourvar = 6;                                            // The value is initialied every time you call this routine.
    ChangeMe();                                                 // Used to change parameters of your program.

    EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
      twinkle();
    }
    show_at_max_brightness_for_power();                         // Run the FastLED.show() at full loop speed.
    Serial.println(LEDS.getFPS());                              // Display frames per second on the serial monitor.  
  } // loop()





// CHANGEME -----------------------------------------------------------------------------------------------------

  void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
    uint8_t secondHand = (millis() / 1000) % 15;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
    static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
    if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
      lastSecond = secondHand;
      switch(secondHand) {
        case 0: thisdelay = 10; break;
        case 5: thisdelay = 20; break;
        case 10: thisdelay = 30; break;
        case 15: break;
      }
    }
  } // ChangeMe()





// Non-power managed display -------------------------------
  FastLED.show();                                             // I don't use this anymore, I use the power managed display instead.
  FastLED.delay(thisdelay);


// Power managed display -----------------------------------

  set_max_power_in_volts_and_milliamps(5, 500);               // This is defined in setup and used by the power management functionality and is currently set at 5V, 500mA.
  show_at_max_brightness_for_power();                         // This is used in loop for power managed display of LED's.
  


// Palettes -----------------------------------------------------------------------------------------


Instructions for using Paletteknife (you need to use Chrome browser):


http://fastled.io/tools/paletteknife/

Put in your toolbar:
https://plus.googleapis.com/112916219338292742137/posts/FvLgYPF52Ma

Get Palettes here:
http://soliton.vm.bytemark.co.uk/pub/cpt-city/index.html

Find palette and click on above toolbar link.

Copy/paste the link to the top of your routine.



CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;        // NOBLEND or LINEARBLEND


// In setup
// RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p

  currentPalette  = CRGBPalette16(CRGB::Black);
  targetPalette   = RainbowColors_p;                            // Used for smooth transitioning.
  currentBlending = LINEARBLEND;

// In loop

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

// In loop for testing
  EVERY_N_MILLISECONDS(5000) {
      targetPalette = CRGBPalette16(CHSV(random8(), 255, 32), CHSV(random8(), random8(64)+192, 255), CHSV(random8(), 255, 32), CHSV(random8(), 255, 255)); 
  }



// In routine
  leds[i] = ColorFromPalette(currentPalette,thishue,thisbri, currentBlending);



// Creating a palette
void SetupRandomPalette() {
  int myHue = random8();
  targetPalette = CRGBPalette16(CHSV(myHue, 255, 32), CHSV(myHue, random8(64)+192, 255), CHSV(myHue, 255, 32), CHSV(myHue, random8(64)+192, 255)); 
}

void SetupRandomPalette() {
  for (int i = 0; i < 16; i++) {
    targetPalette[i] = CHSV(random8(), 255, 255);
  }
}

void SetupRandomPalette() {
  int myHue = random8();
  targetPalette = CRGBPalette16(CRGB::Green); 
}





// Timing ----------------------------------------------------------------------------------------------------------


  EVERY_N_MILLISECONDS(10) {run_function();}        // Whatever function call you want.




// Assigning Colors -------------------------------------------------------------------------------------------------

  leds[k] += CHSV(thathue, allsat, thatbright);
  leds[k] = CRGB::Red;





// colorutils.h is at http://fastled.io/docs/3.1/group___colorutils.html

  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.

  fill_solid(leds,NUM_LEDS, 0xff00ff);                        //A long RGB value
  fill_solid(leds, NUM_LEDS, CRGB(50,0,200));                 // 8 bit values
  fill_solid(leds, NUM_LEDS, CHSV(150,255,200));              // 8 bit values


  fill_gradient_RGB(leds, startpos, 0x000011, endpos, 0x110000);
  fill_gradient_RGB(leds, NUM_LEDS, CRGB(50,0,200), CRGB(80,200,240));    // up to 4 of these

//FORWARD_HUES, BACKWARD_HUES, SHORTEST_HUES, LONGEST_HUES
  fill_gradient(leds, startpos, CHSV(50, 255,255) , endpos, CHSV(150,255,255), SHORTEST_HUES);
  fill_gradient(leds, NUM_LEDS, CHSV(50, 255,255), CHSV(100,255,255), LONGEST_HUES);    // up to 4 of these



// Fade, Scale
  fadeToBlackBy(leds, NUM_LEDS, fadeval);                     // 8 bit, 1 = slow, 255 = fast
  nscale8(leds,NUM_LEDS,fadeval);                             // 8 bit, 1 = fast, 255 = slow
  leds[i].fadeToBlackBy(fadeval);

// Blend

  CRGB myclr;
  myclr = blend(CRGB::Red, CRGB::Blue, sin8(mysine));
  leds[i] = myclr;

//  or

  leds[i] = blend(CRGB::Red, CRGB::Blue, sin8(mysine));
  

// Randomization
  uint8_t myval = random8(0, 255);
  int myval = random16(NUM_LEDS);



// Beats

  int beat = beatsin16(BeatsPerMinute,0,NUM_LEDS);
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);




// Noise http://fastled.io/docs/3.1/group___noise.html
  fill_noise16(leds, NUM_LEDS, octaves, x, xscale, hue_octaves, hxy, hue_scale, hue_time);



// Math http://fastled.io/docs/3.1/lib8tion_8h_source.html

// Limiting math
  #define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
  #define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.
  uint8_t sum = qadd8( 200, 200);                               // --> saturated at 255
  sum = qsub8(i, j);                                            // Does not go below 0


  // Memory management

  memset(leds, 0, NUM_LEDS * 3);                                // Quick clearing of the LED's.
  memcpy8(temp, leds, sizeof(leds));                            // Copy values from temp to leds



// PRINTF capability for debugging
// If you want to use printf capability, put this in your code:
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

  int serial_putchar(char c, FILE* f) {
      if (c == '\n') serial_putchar('\r', f);
      return !Serial.write(c);
  } //serial_putchar()
  FILE serial_stdout;

  void setup() {
    Serial.begin(57600);
    fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE); // Set up stdout
    stdout = &serial_stdout;
  } // setup()

  void loop() {
    printf("My favorite number is %6d!\n", 12);                 // This is just an example
  } // loop()


// Other ---------------------------------------------------------------------------------------------------------------------------------

  // An add glitter function.
  void addGlitter( uint8_t chanceOfGlitter) {
    if(random8() < chanceOfGlitter) {
      leds[ random16(NUM_LEDS) ] += CRGB::White;
    }
  }


// Beats Information ---------------------------------------------------------------------------------------------------------------------


uint8_t wave = beatsin8(
  accum88 beats_per_minute,                                   // I'd use an int or uint8_t
  uint8_t lowest=0,
  uint8_t highest=255,
  uint32_t timebase=0,                                        // Set to millis() to zero out the beat
  uint8_t phase_offset=0)                                     // This is kind of cool

When you want to 'zero' out the beat:

  uint8_t wave = beatsin8(60, 0, 255, millis());


// Easing & lerping -------------------------------------------------------------------------------------------------------

easeOutVal = ease8InOutQuad(easeInVal);                       // Start with easeInVal at 0 and then go to 255 for the full easing.
ledNum = lerp8by8(0, NUM_LEDS, easeOutVal);                   // Map it to the number of LED's you have.




// CHSV to CRGB scaling -----------------------------------------------------------
/*
If you want any level of hue rotation, then keep CHSV brightness at 32

Table of CHSV brightness to CRGB value
0 == (0,0,0)
1 == (1,0,0)
16 == (2,0,0)
23 == (3,0,0)
28 == (4,0,0)
32 == (5,0,0)

*/