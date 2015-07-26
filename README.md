#FastLED-Demos


**By:** Andrew Tuline

**Email:** atuline@gmail.com

**Date:** March, 2015

!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!


If this is your first foray into addressable LED's, then start off with a small strip of < 20 LED's. Long strips require careful power management planning, so start off small and then work up to longer strips.

Before attempting to use these demos, make sure you can compile and run the examples found with the latest development version of the FastLED library, currently 3.1.

The FastLED Google+ community is at https://plus.google.com/communities/109127054924227823508

Finally, before looking for some support, have a look at my FastLED FAQ at http://www.tuline.com/dru/content/my-fastled-support-faq




##Introduction

Here's several display sequences for FastLED for a single strand of RGB LED's, such as Neopixels/WS2812, WS2801 or DotStars/APA102. Each should compile and run separately and have been tested on an Arduino Nano.

These sequences are designed to be loop/non-blocking delay friendly. They perform some simple calculations, update the LED array, return back to the loop to SHOW the LED's and then continue on to the next iteration.

In addition to several standalone demos, I have included a display template as well as aalight and aainfra, which support multiple display sequences, a demo mode, keyboard, IR remote and updated pushbutton control.

I have also included a few sound reactive sequences, most notably fht_log, which uses the Sparkfun MEMS microphone and the FHT (Fast Hartley Transform) library to respond to sound in the frequency domain. Very cool.



**aatemplate.ino** - A standard display template which can be re-used.

**aalight.ino** - Runs on an Arduino connected to an LED strip. Compile aalight.ino, and it will include the other files in the directory. The overall structure for this code is based on funkboxing LED. In addition, to the FastLED library, it also requires a couple of other 3rd party libraries to support pushbutton and IR remote control.

**aainfra.ino** - Runs on a 2nd Arduino with an IR sensor attached. Compile aainfra.ino and connect Tx of the 2nd Arduino to Rx of the first one. You will need to disconnect that in order to program the Arduinos.

See aalight.ino comment section for more information on how to use this. Also see aalight.png for a breadboard layout.


Information about the awesome FastLED Library is available from: [fastled.io](http://fastled.io/)

Download FastLED (select version 3.1) from: [github.com/FastLED/FastLED](https://github.com/FastLED/FastLED)


## Adding to the Library

In order for me to add sequences to the library, my expectation is that they:

* Run quickly (no nested for loops with delay statements).
* Can be called with many different parameters.
* Be short.
* Use 8 or 16 bit math. No floating point.
* Preference is given to time based rather than delay based code.


----------------------------------------------------------


An LED links page on my web site:       http://www.tuline.com/dru/content/led-products-libraries-and-effects

My Youtube channel is at:               https://www.youtube.com/user/atuline/videos

My future LED site will be at:          http://www.twinkleds.com  (already registered with an 'under construction' page)

My Github account is at:                https://github.com/atuline

My Pastebins are at:                    http://pastebin.com/u/atuline

My personal web site is at:             http://www.tuline.com

My FastLED FAQ is at:                   http://www.tuline.com/dru/content/my-fastled-support-faq
