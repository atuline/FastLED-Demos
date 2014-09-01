FastLED-Demos
=============

By: Andrew Tuline

Email: atuline@gmail.com

Date: August, 2014


Here's some demo sequences for FastLED 2.1 (and above) for a single strand of RGB LED's, such as WS2811 or WS2812B. Some I made,
others I've found on the Internet and modified. Each should compile and run separately. I've tested them on an Arduino UNO and
Nano 3.0, but not the Teensy.

If I haven't properly attributed some code, please let me know and I'd be happy to do so.

In the future, I hope to restructure and include sequences that:

* just run
* are sound responsive with just a microphone
* are sound responsive with msgeq7 spectrum analyzer chip
* support some form of input (button, IR)
* have multiple displays in one big file
* I may even investigate classes down the road


The template for the sequences is called aattemplate.


The awesome FastLED Library is available from: http://fastled.io/

Download FastLED 2.1 from: https://github.com/FastLED/FastLED/tree/FastLED2.1


An LED links page on my web site:       http://www.tuline.com/dru/content/led-products-libraries-and-effects

My Youtube channel is at:               https://www.youtube.com/user/atuline/videos

My future LED site will be at:          http://www.twinkleds.com  (already registered with an 'under construction' page)

My Github account is at:                https://github.com/atuline

My Pastebins are at:                    http://pastebin.com/u/atuline

My personal web site is at:             http://www.tuline.com



Disclaimer: I haven't gone through the code with a fine tooth comb, so it's not yet optimized with non-blocking delays and in many
cases, the most efficient declarations of variables. Am working on it slowly.