/*
   Implementation of Power module

*/

// Start every module with at least these two includes:
#include <Arduino.h>
#include "modules.hpp"
// Also include this module's header file:
#include "_power.hpp"

// Unlike header file (.hpp), module implementation file (.ino.hpp) must not be included multiple times:
#ifdef POWER_IMPLEMENTATION
#error The .ino.hpp file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define POWER_IMPLEMENTATION


// Module's setup() function:
// (copied from default PowerWithoutDelay.ino example)
void Power::setup()
{
  pinMode(powerPin, INPUT);
}

// Module's loop() function:
void Power::loop()
{
  pinStatus = digitalRead(powerPin);
  if (pinStatus == HIGH) {
    powerState = 1;
  } else {
    powerState = 0;
  }
}
