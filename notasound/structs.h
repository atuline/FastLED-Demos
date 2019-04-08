#ifndef STRUCTS_H
#define STRUCTS_H

// These structs are from Reko Merio.

struct bubble {

  uint8_t brightness;
  uint8_t color;
  uint16_t pos;
  int8_t velocity;
  uint8_t life;
  uint8_t maxLife;
  int8_t maxVelocity;
  bool exist;


  bubble() {
    
    Init();
    
  } // bubble()

  
  Move() {

    if (velocity > maxVelocity)
      velocity = maxVelocity;
    pos += velocity;
    life++;
    brightness -= 255 / maxLife;
    if (pos > NUM_LEDS - 1) {
      velocity *= -1;
      pos = NUM_LEDS - 1;
    }
    if (pos < 0) {
      velocity *= -1;
      pos = 0;
    }
    if (life > maxLife || brightness < 20)
      exist = false;
    if (!exist)
      Init();
      
  } // Move()


  NewColor() {
    
    color = millis();                             // hue + random(20);
    brightness = 255;
    
  } // NewColor()


  Init() {
    
    pos = random(0, NUM_LEDS);
    velocity = 1;                                 // Increase or decrease if needed
    life = 0;
    maxLife = 80;                                 //How many moves bubble can do
    exist = false;
    brightness = 255;
    color = millis();     // hue + random(30);
    maxVelocity = 2;
    
  } // Init()
  
};  // struct bubble

typedef struct bubble Bubble;


#endif

