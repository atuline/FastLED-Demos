/* button_demo
 * 
 * By: Jack Christensen
 * 
 * Modified by: Andrew Tuline
 * 
 * Date: January, 2017
 * 
 * This demonstration uses a button to set a 'mode' and to read/store it with the Arduino EEPROM. 
 * 
 * Run this sketch and open up the serial monitor.
 * Read the starting mode from EEPROM.
 * Press the button (attached to pin 6) quickly to change to the next mode.
 * Press the button for at least 1 second to write the current mode to EEPROM.
 *
 */

#include <Button.h>                                           // https://github.com/JChristensen/Button
#include "EEPROM.h"

#define BUTTON_PIN 6                                          // Connect a button or something similar from Arduino pin 6 to ground.
#define PULLUP true                                           // To keep things simple, we use the Arduino's internal pullup resistor.
#define INVERT true                                           // Since the pullup resistor will keep the pin high unless the
                                                              // switch is closed, this is negative logic, i.e. a high state
                                                              // means the button is NOT pressed. (Assuming a normally open switch.)
#define DEBOUNCE_MS 50                                        // A debounce time of 20 milliseconds usually works well for tactile button switches.


Button myBtn(BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);        // Declare the button.
boolean longpress;

uint8_t ledMode = 0;
uint8_t maxMode = 6;

int eepaddress = 0;                                           // Store the starting mode in EEPROM address 0.



void setup() {

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.

  ledMode = EEPROM.read(eepaddress);
  Serial.print("Starting ledMode: ");
  Serial.println(ledMode);

} // setup()



void loop() {

  readbutton();

} // loop()



void readbutton() {                                           // Read the button and increase the mode.
  
  myBtn.read();

  if(myBtn.wasReleased()) {
    if (longpress==1) {
      EEPROM.write(eepaddress,ledMode);
      Serial.print("Writing: ");
    } else {
      ledMode = ledMode > maxMode-1 ? 0 : ledMode+1;          // Reset to 0 only during a mode change.
    }
    longpress = 0;
    Serial.println(ledMode);
  }
  
  if(myBtn.pressedFor(1000)) {
    longpress = 1;
  }

} // readbutton()

