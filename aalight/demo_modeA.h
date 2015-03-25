#ifndef DEMO_MODEA_H
#define DEMO_MODEA_H

// Leave the line above blank
void demo_modeA(){
  uint8_t smode;                                                // The mode we'll use.
  uint8_t secondHand = (millis() / 1000) % 220;                 // Change '60' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                                // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                               // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;

    switch(secondHand) {

  // matrix
      case  0: smode=0; bgclr = 0; bgbri = 10; thisdelay=30; thishue=95; break;
      case  5: smode=0; bgclr = 100; bgbri = 20; thisdelay=30; thisdir=1; break;
      case 10: smode=0; bgclr = 0; bgbri = 50; thisdelay=10; thishue=0; break;
      case 15: smode=0; thisdelay=20; thishue=random8(); huerot=1; break;

  // noise16
      case 25: smode=1; thisdelay=20; octaves=1; hue_octaves=2; hxy=6000; x=5000; xscale=3000; hue_scale=50; hue_speed=15; x_speed=100; break;
      case 30: smode=1; thisdelay=20; octaves=random16(1,3); hue_octaves=random16(1,5); hue_scale=random16(10, 50);  x=random16(); xscale=random16(); hxy= random16(); hue_time=random16(); hue_speed=random16(1,3); x_speed=random16(1,30); break;

  // one_sin
      case 35: smode=2; thisdelay=20; thisrot=1; thiscutoff=254; allfreq=8; break;                               // Both rotating hues
      case 40: smode=2; thisdelay=20; thisrot=0; bgclr=0; bgbri=10; break;                                                  // Just 1 rotating hue
      case 45: smode=2; thisdelay=20; thishue=255; bgclr=50;bgbri=5; break;                                                // No rotating hues, all red.
      case 50: smode=2; thisdelay=20; break;                                                  // Enough with the damn twinkles.
      case 55: smode=2; thisdelay=20; allfreq=16; break;                                                  // Time to make a wider bar.
      case 60: smode=2; thisdelay=20; thiscutoff=96; bgclr=100; bgbri=10; break;                                               // Change width of bars.
      case 65: smode=2; thisdelay=20; thiscutoff=96; thisrot=1;bgclr=150; break;                                  // Make those bars overlap, and rotate a hue
      case 70: smode=2; thisdelay=20; thisdir=1; break;                                                    // Change the direction.
      case 75: smode=2; thisdelay=20; thiscutoff=128; wavebright=64; bgclr=175; break;               // Yet more changes
      case 80: smode=2; thisdelay=20; wavebright=128; thisspeed=3; bgclr=200; break;                 // Now, we change speeds.
      case 85: smode=2; thisdelay=20; thisdir=0; break;                                      // Opposite directions

  // pop_fade
      case  90: smode=3; thisdelay=20; colours[0]=0xffffff; numcolours=1; boolcolours=0; maxbar=1; break;   // Just white twinkles
      case  95: smode=3; thisdelay=20; colours[1]=0xff0000; numcolours=2; boolcolours=0; maxbar=4; break;   // Add red and make bars
      case 100: smode=3; thisdelay=20; fadeval=192; break;                                                     // Speed up the fade
      case 105: smode=3; thisdelay=20; boolcolours=1; maxbar=1; break;                                          // A pile of colours, 1 pixel in length
      case 110: smode=3; thisdelay=20; fadeval=128; break;                                                      // Slow down the fade
      case 115: smode=3; thisdelay=20; colours[2]= 0x0000ff; boolcolours=0; numcolours=3; fadeval=192; maxbar=6; break;

  // rainbow_march
      case 120: smode=4; thisdelay=20; thisrot=1; deltahue=5; break;
      case 125: smode=4; thisdelay=20; thisdir=-1; deltahue=10; break;
      case 130: smode=4; thisdelay=20; thisrot=5; break;
      case 135: smode=4; thisdelay=20; thisrot=5; thisdir=-1; deltahue=20; break;
      case 140: smode=4; thisdelay=20; deltahue=30; break;
      case 145: smode=4; thisdelay=20; deltahue=2; thisrot=5; break;

  // three_sin
      case 150: smode=5; thisdelay=20;  mul1=20; mul2=25; mul3=22; break;
      case 155: smode=5; thisdelay=20; mul1=5; mul2=8; mul3=7; break;
      case 160: smode=5; thisdelay=20; lvl1=220; lvl2=220; lvl3=220; break;

  // two_sin
      case 165: smode=6; thisdelay=20; thisrot=1; thatrot=1; break;                                      // Both rotating hues
      case 170: smode=6; thisdelay=20; thisrot=0; break;                                                   // Just 1 rotating hue
      case 175: smode=6; thisdelay=20; thatrot=0; thishue=255; thathue=255; break;                    // No rotating hues, all red.
      case 180: smode=6; thisdelay=20; thatrot = 5; break;
      case 185: smode=6; thisdelay=20; thisrot = 4; allfreq=16; thathue=128; break;                                  // Time to make a wider bar.
      case 190: smode=6; thisdelay=20; thiscutoff=96; thatcutoff=240; break;                            // Change width of bars.
      case 195: smode=6; thisdelay=20; thiscutoff=96; thatcutoff=96; thisrot=1; break;                // Make those bars overlap, and rotate a hue
      case 200: smode=6; thisdelay=20; thisspeed=-4; thatspeed=-4; break;                                 // Change direction                                      // Change the direction.
      case 205: smode=6; thisdelay=20; thisspeed = 3; thiscutoff=128; thatcutoff=128; wavebright=64; break;   // Yet more changes
      case 210: smode=6; thisdelay=20; wavebright=128; thisspeed=5; thatspeed=3; break;                 // Now, we change speeds.
      case 215: smode=6; thisdelay=20; thisspeed=3; thatspeed=-3; break;                     // Opposite directions
    }
  } // if lastSecond


  switch (smode) {
    case 0: matrix(); break;
    case 1: noise16(); break;
    case 2: one_sin(); break;
    case 3: pop_fade(); break;
    case 4: rainbow_march(); break;
    case 5: three_sin(); break;
    case 6: two_sin(); break;
  }

} // demo_modeA()

#endif