#ifndef SOUNDMEMS_H
#define SOUNDMEMS_H

void soundmems() {                                                              // Here's where we capture sound. It provides an average, a current sample as well as a peak trigger.
                                                                                // I tried some fancier math, but never came up with anything that really worked all that well. Must . . work. . harder.
// Local definitions
  #define sensitivity 100                                                       // Define maximum cutoff of potentiometer for cutting off sounds.
  #define DC_OFFSET  512                                                        // DC offset in mic signal. I subtract this value from the raw sample of a 'quiet room' test.
  #define NSAMPLES 64                                                           // Creating an array of lots of samples for decent averaging.

// Persistent local variables (saved for next iteration)
  static int16_t samplearray[NSAMPLES];                                         // Array of samples.
  static uint16_t samplesum = 0;                                                // Sum of the last 64 samples. This had better be positive.
  static uint8_t samplecount = 0;                                               // A rollover counter to cycle through the array.

// Temporary local variables
  int16_t sample = 0;                                                           // Current sample. Starts with negative values, which is why it's signed.
  uint16_t potin;                                                               // Our scaled potentiometer value.

  samplepeak = 0;                                                               // Reset the global sample peak.

  potin = map(analogRead(POT_PIN), 0, 1023, 0, sensitivity);                    // Read the potentiometer and scale it to our sensitivity setting, which is scaled to our microphone readings.

  sample = analogRead(MIC_PIN) - DC_OFFSET;                                     // Sample the microphone. Range will result in -512 to 512.
  sample = abs(sample);                                                         // Get the absolute value and DO NOT combine abs() into the previous line or this will break. Badly!
  
  if (sample < potin) sample = 0;                                               // Filter ambient noise, which is adjustable via the potentiometer.

  samplesum += sample - samplearray[samplecount];                               // Add the new sample and remove the oldest sample in the array. No 'for' loops required here for extra speed.
  sampleavg = samplesum / NSAMPLES;                                             // Get an average.
  samplearray[samplecount] = sample;                                            // Update oldest sample in the array with new sample. By Andrew Tuline.
  samplecount = (samplecount + 1) % NSAMPLES;                                   // Update the counter for the array and rollover if we hit the max.

  if (sample > (sampleavg + potin) && (sample < oldsample)) samplepeak = 1;     // We're on the down swing, so we just peaked.
  
  oldsample = sample;                                                           // We'll use oldsample globally as our 'current' sample.

} // soundmems()



#endif
