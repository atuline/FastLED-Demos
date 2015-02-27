#FastLED-Demos


**By:** Andrew Tuline

**Email:** atuline@gmail.com

**Date:** February, 2015

!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!

Before attempting to use these demos, you should compile and run the examples found with the latest version of the FastLED library, available at:

https://github.com/FastLED/FastLED

In some cases, you may need to use the latest development version of FastLED.



##Introduction

Here's several COMPLETELY re-written display sequences for FastLED for a single strand of RGB LED's, such as Neopixels, WS2812B, WS2801 or APA102. Each should compile and run separately and have been tested on an Arduino UNO and Nano 3.0.

In an older version, I had several demos that used nested for loops, required a lot of code, weren't highly customizable. Except for some really cool ones, most of those have now been replaced. For instance, two_sin() replaces several 'marching' type sequences and supports several variables to change colour, phase, direction, brightness and so on. Likewise, pop_fade_demo replaces several twinkling sequences.

The new sequences are now designed to be loop/non-blocking delay friendly. They perform some simple calculations, update the LED array and then then return back to the loop to SHOW the LED's and then continue on to the next iteration.

In addition to several standalone demos, I have included a display template as well as aalight and aainfra, which support multiple display sequences, a demo mode, keyboard, IR remote and updated pushbutton control.

I have also included fht_log, which uses the Sparkfun MEMS microphone and the FHT (Fast Hartley Transform) library to respond to sound in the frequency domain. Very cool.



**aatemplate.ino** - A standard display template which can be re-used.

**aalight.ino** - Runs on an Arduino connected to an LED strip. Compile aalight.ino, and it will include the other ino files in the directory. The structure for this code is based on funkboxing LED. In addition, to the FastLED library, it also requires a couple of other 3rd party libraries to support pushbutton and IR remote control.

**aainfra.ino** - Runs on a 2nd Arduino with an IR sensor attached. Compile aainfra.ino and connect Tx of the 2nd Arduino to Rx of the first one. You will need to disconnect that in order to program the Arduinos.

See aalight.ino comment section for more information on how to use this. Also see aalight.png for a breadboard layout.


The awesome FastLED Library is available from: [fastled.io](http://fastled.io/)

Download FastLED from: [github.com/FastLED/FastLED](https://github.com/FastLED/FastLED)


## Update

I've removed some clunky routines and replaced them some simple ones.

----------------------------------------------------------


An LED links page on my web site:       http://www.tuline.com/dru/content/led-products-libraries-and-effects

My Youtube channel is at:               https://www.youtube.com/user/atuline/videos

My future LED site will be at:          http://www.twinkleds.com  (already registered with an 'under construction' page)

My Github account is at:                https://github.com/atuline

My Pastebins are at:                    http://pastebin.com/u/atuline

My personal web site is at:             http://www.tuline.com