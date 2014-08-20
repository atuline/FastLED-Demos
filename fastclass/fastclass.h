#ifndef fastclass_h
#define fastclass_h

#include "FastLED.h"
#include "Arduino.h"

class fastclass {
	public:
		fastclass(CRGB *leds, int numLeds);
    void run(CRGB *leds, int numLeds);
};

#endif
