/*
   Implementation of Fans module

*/

// Start every module with at least these two includes:
#include <Arduino.h>
#include "modules.hpp"
// Also include this module's header file:
#include "_fans.hpp"
#include "_relays.hpp"

// Unlike header file (.hpp), module implementation file (.ino.hpp) must not be included multiple times:
#ifdef FANS_IMPLEMENTATION
#error The .ino.hpp file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define FANS_IMPLEMENTATION


// Module's setup() function:
// (copied from default FansWithoutDelay.ino example)
void Fans::setup()
{
  setFan1 = 0;
  setFan2 = 0;
}


// Module's loop() function:
// (copied from default FansWithoutDelay.ino example, with some comments added)
void Fans::loop()
{
  if (setFan1 != FANS_NOOP && setFan1 != _fan1) {
    relays->setRelay1 = RELAYS_OFF;
    relays->setRelay2 = RELAYS_OFF;
    if (setFan1 > 0) {
      relays->setRelay1 = RELAYS_ON;
    }
    if (setFan1 == 2) {
      relays->setRelay2 = RELAYS_ON;
    }
    _fan1 = setFan1;
  }
  setFan1 = FANS_NOOP;
  fan1 = _fan1;

  if (setFan2 != FANS_NOOP && setFan2 != _fan2) {
    relays->setRelay3 = RELAYS_OFF;
    relays->setRelay4 = RELAYS_OFF;
    if (setFan2 > 0) {
      relays->setRelay3 = RELAYS_ON;
    }
    if (setFan2 == 2) {
      relays->setRelay4 = RELAYS_ON;
    }
    _fan2 = setFan2;
  }
  setFan2 = FANS_NOOP;
  fan2 = _fan2;
}
