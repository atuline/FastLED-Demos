# FastLED-Demos


**By:** Andrew Tuline

**Email:** atuline@gmail.com

**Date:** January, 2017



!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!

Before attempting to use these demos, make sure you can compile and run the examples found with the latest version of the FastLED library.

If this is your first foray into addressable LED's, then start off with a small strip of less than 20 LED's. Long strips require careful power management design and implementation, so start off small and then work up to longer strips.

Spend some time analyzing these demos, make changes to them and then make your own. Don't start off as a beginner trying to compile and run the advanced demos such as seirlight. Build up to it.

Finally, before looking for some support, have a look at fastled_support_faq.txt. Key things are to:

- Check everything several times.
- Break it down into simple components.
- Understand the basics before trying the advanced routines.

If you're a beginner, learn to walk before you run.


## Introduction

Here are several display sequences for FastLED for a single strand of addressable RGB LED's, such as NeoPixels/WS2812, WS2801 or DotStars/APA102. Each should compile and run separately and all have been tested on an Arduino Nano with appropriate inputs as required.

Almost all of these sequences are designed to be loop/non-blocking delay friendly. They perform some simple calculations, update the LED array, return back to the loop to SHOW the LED's and then continue on to the next iteration.

In addition to several standalone demos, I have included a comprehensive demo called seirlight (NOT for beginners), which supports multiple display sequences, multiple Arduino's, has a demo mode, EEPROM and IR remote support. There are also a couple of button controlled demos with EEPROM support.

In addition, I have included a few sound reactive sequences, most notably fht_log_ripple, which uses the Sparkfun MEMS microphone and the FHT (Fast Hartley Transform) library to respond to sound in the frequency domain. Tricky to get going, but very cool.


## My LED Philosophy

I remember back in high school physics (in 1969) when my grade 9 physics teacher talked about wave particle duality where light can be thought of as either a wave or a series of particles.  When I first started programming addressable LED's, I made sure that each LED was lit to a specific value and at a specific location just like the particles I had learned about in grade 9. This was quite tedious and I have since come to think of and to program a strand of LED's as not only a wave, but one that's been abstracted by colour palettes. The advantage is that we can come up with some very cool sequences using basic trigonometry, which we learned in Grade 10 math.


## Notable Files

**black.ino** - Turns off your LED's, so you can program in peace without having to unplug your strand.

**button_demoReel100** - Use button to control the mode. Press button for > 1 second to save current mode to EEPROM (as startup mode).

**fht_log_ripple.ino** - Uses Fast Hartley Transform (from a 3rd party library) to translate input sounds into frequency domain output. This is NOT for the beginner.

**seirlight.ino** - Runs on an Arduino connected to an LED strip. Compile seirlight.ino, and it will include the other files in the directory. The overall structure for this code was originally based on funkboxing LED. In addition, to the FastLED library, it also requires another 3rd party library support IR remote control. This is NOT for the beginner. See seirlight.ino comment section for more information on how to use this.


## Controls

- Sparkfun MEMS microphone (with built-in pre-amp) for sound.
- TSOP38238 IR receiver an an el cheapo IR controller for IR control.
- 10K potentiometer for analog control.
- Any old button switch for push button control.


## Microphones

If you go out and buy one of those cheap sound sensors from aliexpress, you will be sorely disappointed. The Sparkfun MEMS microphone includes an amplifier (with a gain of 67db), which provides a line in signal that's adequate for the Arduino works fine. Those sound sensors don't. You have been warned.


## More

Information about the awesome FastLED Library is available from: [fastled.io](http://fastled.io/)

Download FastLED (select version 3.1) from: [github.com/FastLED/FastLED](https://github.com/FastLED/FastLED)

FastLED Documentation is at: [http://fastled.io/docs/3.1/modules.html](http://fastled.io/docs/3.1/modules.html)

The FastLED Google+ community is at [https://plus.google.com/communities/109127054924227823508](https://plus.google.com/communities/109127054924227823508)


-----------------------------------------------------------------------------------


My Youtube channel is at:               https://www.youtube.com/user/atuline/videos

My Github account is at:                https://github.com/atuline

My Gists are at:                        http://gist.github.com/atuline

My Pastebins are at:                    http://pastebin.com/u/atuline

