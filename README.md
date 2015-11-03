#FastLED-Demos


**By:** Andrew Tuline

**Email:** atuline@gmail.com

**Date:** August, 2015

!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!


If this is your first foray into addressable LED's, then start off with a small strip of less than 20 LED's. Long strips require careful power management planning, so start off small and then work up to longer strips.

Before attempting to use these demos, make sure you can compile and run the examples found with the latest development version of the FastLED library.

The FastLED Google+ community is at https://plus.google.com/communities/109127054924227823508

Finally, before looking for some support in the above community (or directly), have a look at fastled_support_faq.txt. Key things are to:

- Check everything several times.
- Break it down into simple components.
- Describe your environment thoroughly.


##Introduction


Here are several display sequences for FastLED for a single strand of RGB LED's, such as NeoPixels/WS2812, WS2801 or DotStars/APA102. Each should compile and run separately and all have been tested on an Arduino Nano.

These sequences are designed to be loop/non-blocking delay friendly. They perform some simple calculations, update the LED array, return back to the loop to SHOW the LED's and then continue on to the next iteration.

In addition to several standalone demos, I have included a display template as well as aalight and aainfra, which support multiple display sequences, a demo mode, keyboard, IR remote and updated pushbutton control.

I have also included a few sound reactive sequences, most notably fht_log, which uses the Sparkfun MEMS microphone and the FHT (Fast Hartley Transform) library to respond to sound in the frequency domain. Very cool.

##My LED Philosophy

I remember back in high school physics (in 1969) when my grade 9 physics teacher talked about wave particle duality where light can be thought of as either a wave or a series of particles.  When I first started programming addressable LED's, I made sure that each LED was lit to a specific value and at a specific location just like the particles I had learned about in grade 9. This was quite tedious and I have since come to think of and to program a strand of LED's as not only a wave, but one that's been abstracted by colour palettes. The advantage is that we can come up with some simple and cool mathematical sequences, however we don't necessarily know what each LED will contain.


##Files

**aatemplate.ino** - A standard display template which can be re-used.

**black.ino** - Turns off your LED's, so you can program in peace without having to unplug.

**aalight.ino** - Runs on an Arduino connected to an LED strip. Compile aalight.ino, and it will include the other files in the directory. The overall structure for this code is based on funkboxing LED. In addition, to the FastLED library, it also requires a couple of other 3rd party libraries to support pushbutton and IR remote control.

**aainfra.ino** - Runs on a 2nd Arduino with an IR sensor attached. Compile aainfra.ino and connect Tx of the 2nd Arduino to Rx of the first one. You will need to disconnect that in order to program the Arduinos.

See aalight.ino comment section for more information on how to use this. Also see aalight.png for a breadboard layout.


##More
Information about the awesome FastLED Library is available from: [fastled.io](http://fastled.io/)

Download FastLED (select version 3.1) from: [github.com/FastLED/FastLED](https://github.com/FastLED/FastLED)

FastLED Documentation is at: [http://fastled.io/docs/3.1/modules.html](http://fastled.io/docs/3.1/modules.html)

## Adding to the Library

In order for me to add sequences to the library, they should:

* Run quickly (no nested for loops with delay statements inside).
* Can be called with many different parameters.
* Treats LED's as waves and not particles (no pixel counting).
* Be short.
* Use 8 or 16 bit math. No floating point.
* Preference is given to time based rather than delay based code.

----------------------------------------------------------


My Youtube channel is at:               https://www.youtube.com/user/atuline/videos

My Github account is at:                https://github.com/atuline

My Gists are at:                        http://gist.github.com/atuline

My Pastebins are at:                    http://pastebin.com/u/atuline

