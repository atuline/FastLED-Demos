#ifndef DEMO_MODEA_H
#define DEMO_MODEA_H

// Leave the line above blank
void demo_modeA(){
  uint8_t smode;                                                // The mode we'll use.
  uint8_t secondHand = (millis() / 1000) % 220;                 // Change '60' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                                // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                               // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;

// matrix
    if (secondHand ==  0)  {smode=0; thisdelay=30; thishue=95;}
    if (secondHand ==  5)  {smode=0; thisdelay=30; thisdir=1;}
    if (secondHand == 10)  {smode=0; thisdelay=10; thishue=0;}
    if (secondHand == 15)  {smode=0; thisdelay=20; thishue=random8(); huerot=1;}

// noise16
    if (secondHand == 25)  {smode=1; thisdelay=20; octaves=1; hue_octaves=2; hxy=6000; x=5000; xscale=3000; hue_scale=50; hue_speed=15; x_speed=100;}
    if (secondHand == 30)  {smode=1; thisdelay=20; octaves=random16(1,3); hue_octaves=random16(1,5); hue_scale=random16(10, 50);  x=random16(); xscale=random16(); hxy= random16(); hue_time=random16(); hue_speed=random16(1,3); x_speed=random16(1,30);}

// one_sin
    if( secondHand ==  35)  {smode=2; thisdelay=20; thisrot=1; thiscutoff=254; allfreq=8;}                               // Both rotating hues
    if( secondHand == 40)  {smode=2; thisdelay=20; thisrot=0;}                                                          // Just 1 rotating hue
    if( secondHand == 45)  {smode=2; thisdelay=20; thishue=255;}                                                      // No rotating hues, all red.
    if( secondHand == 50)  {smode=2; thisdelay=20;}                                                        // Enough with the damn twinkles.
    if( secondHand == 55)  {smode=2; thisdelay=20; allfreq=16;}                                                        // Time to make a wider bar.
    if( secondHand == 60)  {smode=2; thisdelay=20; thiscutoff=96;}                                                     // Change width of bars.
    if( secondHand == 65)  {smode=2; thisdelay=20; thiscutoff=96; thisrot=1;}                                        // Make those bars overlap, and rotate a hue
    if( secondHand == 70)  {smode=2; thisdelay=20; thisdir=1;}                                                          // Change the direction.
    if( secondHand == 75)  {smode=2; thisdelay=20; thiscutoff=128; wavebright=64;}                     // Yet more changes
    if( secondHand == 80)  {smode=2; thisdelay=20; wavebright=128; thisspeed=3;}                       // Now, we change speeds.
    if( secondHand == 85)  {smode=2; thisdelay=20; thisdir=0;}                                            // Opposite directions

// pop_fade
    if (secondHand ==  90) {smode=3; thisdelay=20; colours[0]=0xffffff; numcolours=1; boolcolours=0; maxbar=1;}         // Just white twinkles
    if (secondHand ==  95) {smode=3; thisdelay=20; colours[1]=0xff0000; numcolours=2; boolcolours=0; maxbar=4;}         // Add red and make bars
    if (secondHand == 100) {smode=3; thisdelay=20; fadeval=192;}                                                           // Speed up the fade
    if (secondHand == 105) {smode=3; thisdelay=20; boolcolours=1; maxbar=1;}                                                // A pile of colours, 1 pixel in length
    if (secondHand == 110) {smode=3; thisdelay=20; fadeval=128;}                                                            // Slow down the fade
    if (secondHand == 115) {smode=3; thisdelay=20; colours[2]= 0x0000ff; boolcolours=0; numcolours=3; fadeval=192; maxbar=6;}

// rainbow_march
    if (secondHand == 120) {smode=4; thisdelay=20; thisrot=1; deltahue=5;}
    if (secondHand == 125) {smode=4; thisdelay=20; thisdir=-1; deltahue=10;}
    if (secondHand == 130) {smode=4; thisdelay=20; thisrot=5;}
    if (secondHand == 135) {smode=4; thisdelay=20; thisrot=5; thisdir=-1; deltahue=20;}
    if (secondHand == 140) {smode=4; thisdelay=20; deltahue=30;}
    if (secondHand == 145) {smode=4; thisdelay=20; deltahue=2; thisrot=5;}

// three_sin
    if( secondHand == 150) {smode=5; thisdelay=20;  mul1=20; mul2=25; mul3=22;}
    if( secondHand == 155) {smode=5; thisdelay=20; mul1=5; mul2=8; mul3=7;}
    if( secondHand == 160) {smode=5; thisdelay=20; lvl1=220; lvl2=220; lvl3=220;}

// two_sin
    if( secondHand == 165) {smode=6; thisdelay=20; thisrot=1; thatrot=1;}                                             // Both rotating hues
    if( secondHand == 170) {smode=6; thisdelay=20; thisrot=0;}                                                          // Just 1 rotating hue
    if( secondHand == 175) {smode=6; thisdelay=20; thatrot=0; thishue=255; thathue=255;}                           // No rotating hues, all red.
    if( secondHand == 180) {smode=6; thisdelay=20; thatrot = 5;}
    if( secondHand == 185) {smode=6; thisdelay=20; thisrot = 4; allfreq=16; thathue=128;}                                         // Time to make a wider bar.
    if( secondHand == 190) {smode=6; thisdelay=20; thiscutoff=96; thatcutoff=240;}                                   // Change width of bars.
    if( secondHand == 195) {smode=6; thisdelay=20; thiscutoff=96; thatcutoff=96; thisrot=1;}                       // Make those bars overlap, and rotate a hue
    if( secondHand == 200) {smode=6; thisdelay=20; thisspeed=-4; thatspeed=-4;}                                        // Change direction                                      // Change the direction.
    if( secondHand == 205) {smode=6; thisdelay=20; thisspeed = 3; thiscutoff=128; thatcutoff=128; wavebright=64;}   // Yet more changes
    if( secondHand == 210) {smode=6; thisdelay=20; wavebright=128; thisspeed=5; thatspeed=3;}                       // Now, we change speeds.
    if( secondHand == 215) {smode=6; thisdelay=20; thisspeed=3; thatspeed=-3;}                            // Opposite directions
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