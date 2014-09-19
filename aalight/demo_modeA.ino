
// Leave the line above blank
void demo_modeA(){
  
  int r = 20;

  thisdelay = 20;
  for (int i=0; i<r*20; i++) {rainbow_fade();}

  thisdir = 0; thisdelay = 5;
  for (int i=0; i<r*60; i++) {rainbow_march();}

  thisdelay=20;
  for (int i=0; i<r*30; i++) {random_color_pop();}

  thisdir = 0; thisdelay = 40; thishue = 0;
  for (int i=0; i<r*20; i++) {ems_lightsONE();}

  thisdelay = 15; thishue = 0; thissat = 255;
  for (int i=0; i<r*40; i++) {pulse_one_color_all();}

  thishue = 50; thisdelay = 8;
  for (int i=0; i<r*60; i++) {twinkle();}

  thisdir = 0; thisdelay = 50; thisstep = 15;
  for (int i=0; i<r*20; i++) {rainbow_vertical();}

  thisdir = 0; thisdelay = 20;
  for (int i=0; i<r*40; i++) {mynoise();}

} // demo_modeA()

