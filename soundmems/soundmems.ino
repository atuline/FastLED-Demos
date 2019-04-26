/* soundmems
 *  
 * By: Andrew Tuline
 *
 * Date: January, 2019
 *
 * Basic code to read and display values from various microphones.
 * 
 * If your microphone uses 3.3 wuth a 5V Arduino, then please read up on analogReference(EXTERNAL) in the setup().
 * 
 * You may also need to adjust the DC_OFFSET to be the value of the microphone when all is silent. Test and find out.
 *
 */

#define MIC_PIN      4                                   // Analog port for microphone
#define DC_OFFSET  509                                 // DC offset in mic signal. Should probably be about 512.
#define SQUELCH     16                                      // Background noise value

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(115200);                                 // Initialize serial port for debugging.
} // setup()


void loop() {
    sound_test();
} // loop()


void sound_test() {
  Serial.print(1024); Serial.print(" "); Serial.print(0); Serial.print(" ");
  int16_t micIn = analogRead(MIC_PIN) - DC_OFFSET;
  micIn = abs(micIn);
  micIn = (micIn <= SQUELCH) ? 0 : (micIn - SQUELCH);       // Remove noise/hum.
  Serial.println(micIn);  
} // sound_test()


