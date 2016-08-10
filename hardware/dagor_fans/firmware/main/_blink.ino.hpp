/*
   Implementation of Blink module

*/

// Start every module with at least these two includes:
#include <Arduino.h>
#include "modules.hpp"
// Also include this module's header file:
#include "_blink.hpp"

// Unlike header file (.hpp), module implementation file (.ino.hpp) must not be included multiple times:
#ifdef BLINK_IMPLEMENTATION
#error The .ino.hpp file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define BLINK_IMPLEMENTATION


// Module's setup() function:
// (copied from default BlinkWithoutDelay.ino example)
void Blink::setup()
{
  ledPin = 13;
  interval = 1000;
  pinMode(ledPin, OUTPUT);
}


// Module's loop() function:
// (copied from default BlinkWithoutDelay.ino example, with some comments added)
void Blink::loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.

  // this is a local variable, it exists only within while this function is running:
  unsigned long currentMillis = millis();

  // previousMillis and interval are module functions.
  // Completely equivalent way of writing that "if" statement:
  // if (currentMillis - blink->previousMillis >= blink->interval) ...

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED:
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}
