#ifndef SOUNDMEMS_H
#define SOUNDMEMS_H

void soundmems() {                                                              // Here's where we capture sound. It provides an average, a current sample as well as a peak trigger.
                                                                                // I tried some fancier math, but never came up with anything that really worked all that well. Must . . work. . harder.
// Local definitions
  #define sensitivity 100                                                       // Define maximum cutoff of potentiometer for cutting off sounds.
  #define DC_OFFSET  512                                                        // DC offset in mic signal. I subtract this value from the raw sample of a 'quiet room' test.
  #define NSAMPLES 32                                                           // Creating an array of lots of samples for decent averaging.
  #define QSAMPLES 4                                                            // Creating an array of quick samples, to even out any glitches.

// Persistent local variables
  static int16_t samplearray[NSAMPLES];                                         // Array of samples for general sampling average.
  static int16_t qsamplearray[QSAMPLES];                                        // Array of samples for quick sampling average.
  
  static uint16_t samplesum = 0;                                                // Sum of the last 64 samples. This had better be positive.
  static uint8_t samplecount = 0;                                               // A rollover counter to cycle through the array.

  static uint16_t qsamplesum = 0;
  static uint16_t qsamplecount = 0;

// Temporary local variables
  int16_t sample = 0;                                                           // Current sample. Starts with negative values, which is why it's signed.
  uint16_t potin;                                                               // Our scaled potentiometer value.


  samplepeak = 0;                                                               // Reset the global sample peak.

  sample = analogRead(MIC_PIN) - DC_OFFSET;                                     // Sample the microphone. Range will result in -512 to 512.
  sample = abs(sample);                                                         // Get the absolute value and DO NOT combine abs() into the previous line or this will break. Badly!

  if( sample > (sampleavg + 256)) sample = sampleavg;                           // Ignore any freaky spikes.
  
  if (sample < squelch) sample = 0;                                             // Squelch control for ambient noise, which is adjustable via the potentiometer.

  samplesum += sample - samplearray[samplecount];                               // Add the new sample and remove the oldest sample in the array. No 'for' loops required here for extra speed.
  sampleavg = samplesum / NSAMPLES;                                             // Get an average.
  samplearray[samplecount] = sample;                                            // Update oldest sample in the array with new sample. By Andrew Tuline.
  samplecount = (samplecount + 1) % NSAMPLES;                                   // Update the counter for the array and rollover if we hit the max.

  qsamplesum += sample - qsamplearray[qsamplecount];                            // Add the new sample and remove the oldest sample in the array. No 'for' loops required here for extra speed.
  qsampleavg = qsamplesum / QSAMPLES;                                           // Get an average.
  qsamplearray[qsamplecount] = sample;                                          // Update oldest sample in the array with new sample. By Andrew Tuline.
  qsamplecount = (qsamplecount + 1) % QSAMPLES;                                 // Update the counter for the array and rollover if we hit the max.

  if (sample > (sampleavg + squelch + 5)) samplepeak = 1;                       // We just peaked above average+noise+fudge.

  oldsample = sample;                                                           // We'll use oldsample globally as our 'current' sample.

  max_vol = sampleavg * 2 + 4;                                                  // What we think the maximum volume should be. Kind of a peak predictor.

 
/*  Serial.print(oldsample);
  Serial.print(" ");
  Serial.print(sampleavg);
  Serial.println(" ");
  Serial.print(qsampleavg);
  Serial.print(" ");
  Serial.print(max_vol);
  Serial.print(" ");
  Serial.print(0);
  Serial.print(" ");
  Serial.println(512);
*/

} // soundmems()

#endif
