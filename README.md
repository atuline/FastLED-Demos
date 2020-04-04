# FastLED-Demos


**By:** Andrew Tuline

**Email:** atuline@gmail.com

**Date:** May, 2019

**Update: April 2020. I am stopping work on my 'notasound/notamesh' large demos in favour of supporting WLED (at wled.me).

Here's a big pile of FastLED demos.


!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!

Before attempting to use these demos, make sure you can compile and run the examples found with the latest version of the FastLED library.

If this is your first foray into addressable LED's, then start off with a small strip of less than 20 LED's. Long strips require careful power management design and implementation, so start off small and then work up to longer strips.

Spend some time analyzing these demos, make changes to them and then make your own. Don't start off as a beginner trying to compile and run the advanced demos. Build up to it.

Finally, before looking for some support, have a look at fastled_support_faq.txt. Key things are to:

- Check everything several times.
- Break it down into simple components.
- Write small test programs so that you understand concepts.
- Learn the basics before trying the advanced routines.


## Introduction

Here are several display sequences for FastLED for a single strand of addressable RGB LED's, such as NeoPixels/WS2812, WS2801 or DotStars/APA102. Each should compile and run separately and all have been tested on an Arduino Nano with appropriate inputs as required.

Almost all of these sequences are designed to be loop/non-blocking delay friendly. They perform some simple calculations, update the LED array, return back to the loop to SHOW the LED's and then continue on to the next iteration.

In addition to several standalone demos, I have included comprehensive demos called notamesh and notasound (NOT for beginners), which supports multiple display sequences, multiple Arduino's, has a demo mode, EEPROM IR remote and sound reactive support. There are also a couple of separate button controlled demos with EEPROM support.

In addition, I have included a few other sound reactive sequences, most notably fht_log_ripple, which uses the ADMP401 MEMS microphone and the FHT (Fast Hartley Transform) library to respond to sound in the frequency domain. Tricky to get going, but very cool.


## Notable Files

**black.ino** - Turns off your LED's, so you can program in peace without having to unplug your strand.

**button_demoReel100** - Use button to control the mode. Press button for > 1 second to save current mode to EEPROM (as startup mode). This required a 3rd party button library.

**notamesh.ino** - Huge IR controlled demo display. Now works with ESP8266 and NOT for beginners.


## Introducing notamesh and notasound

Introducing two new display demos for Arduino and FastLED.

**notamesh** – Introduces the ability to synchronize and delay display sequences across multiple Arduino microcontrollers via IR control.


**Q. What is this ‘mesh’ all about?**
You can configure multiple Arduino microcontrollers with the same display sequence, but with a short delay, thus giving the effect of synchronization.


**Scenario:**
You have 5 IR controlled Arduino based displays, all within eyesight of each other, and all running notamesh. At compile time, each will be configured for the type of LED’s in use as well as a unique ID for each and with demo mode enabled.
Upon first time power-up, each strand will be configured with the length of the strand, as well as a delay. For instance, each Arduino has 40 LED’s connected to it. 

- Arduino A1 – 0ms delay
- Arduino A2 – 100ms delay
- Arduino A3 – 200ms delay
- and so on . . . .


Once they’re all programmed and running, you would then press the ‘Reset’ button on the IR controller in order to synchronize millis() and the Arduinos would begin displaying the demo mode, with each Arduino slightly out of sync with the others.


**These sketches:**
- Run on an Arduino UNO/Nano, etc.
- Use the FastLED display library.
- Use Nico Hood’s IR library.
- Include a LOT of display routines.

**Basic IR Functionality**
- Adjust overall brightness.
- Adjust overall speed.
- Reset (to sync multiple controllers).
- Enable/disable demo mode.
- Increase/decrease speed of animation.
- Adjust overall brightness.
- Change direction of some routines.
- Enable/disable glitter.
- Select previous/next display routine.
- Enable/disable palette rotation.
- Select previous/next palette.
- A 'lamp' mode.
- Select hue/saturation.
- Select a single hue based palette

**EEPROM Functionality**
- Select specific Arduino for EEPROM configuration.
- Adjust strand length.
- Adjust mesh delay.
- Change startup display mode.
- Program/select 2 favourite display modes.
- Added a bunch of other things as documented in notamesh and notasound.

**Sound functionality**
- Continuous sampling of sounds via ADMP401 microphone.
- Averaging of last 32 samples.
- Quick averaging of last 4 samples.
- Crude peak detection.
- Integration of sound with various display routines.
- IR control of sound squelch.
- No FFT or MSGEQ7 required.


## My FastLED Animation Philosophy

- Do NOT use delay statements in the loop as that breaks input routines (i.e. button).
- Do not use nested loops (for performance reasons).
- Use millis() as a real time counter unless a fixed counter is required.
- Spend a bit more time using high school math (ie trigonometry), rather than just count pixels.
- Keep the display routines as short as possible.
- Data typing as strict as possible i.e. why define an int when a uint8_t is all that is required. Oh, and floats are not used at all.
- Localize variables to each routine as much as possible.
- Break out the display routines into separate .h files for increased readability and modularity.
- Be generous with comments.


## Controls

- TSOP38238 IR receiver and a 24 button IR controller for IR control.


## More

Information about the awesome FastLED Library is available from: [fastled.io](http://fastled.io/)

Download FastLED from: [github.com/FastLED/FastLED](https://github.com/FastLED/FastLED)

FastLED Documentation is at: [http://fastled.io/docs/3.1/modules.html](http://fastled.io/docs/3.1/modules.html)

The FastLED community is at [https://www.reddit.com/r/fastled](https://www.reddit.com/r/fastled)


-----------------------------------------------------------------------------------

My Youtube channel is at:               https://www.youtube.com/user/atuline/videos
My Github account is at:                https://github.com/atuline
My Gists are at:                        https://gist.github.com/atuline
My Pastebins are at:                    https://pastebin.com/u/atuline
FastLED support forum:					https://www.reddit.com/r/fastled

