// Include Ardiono.h is needed for many custom IDEs, and doesn't hurt Arduino IDE:
#include <Arduino.h>


// Number of modules in your app:
#define MODULE_MAX_N 10
// (2 * MODULE_MAX_N) bytes of SRAM will be reserved

// Include code for working with modules:
#include "modules.hpp"

// Include the actual modules:
#include "_relays.ino.hpp"
#include "_fans.ino.hpp"
#include "_blink.ino.hpp"
#include "_power.ino.hpp"

#include "_protocol.ino.hpp"
#include "_serial_comm.ino.hpp"

// This include is all that is required to include a module in the code.
// If a module is removed from here, it will not be compiled.
// Order of includes matters.


void setup()
{
  Serial.begin(9600);
  // Call setup() for every module:
  unsigned int i;
  for (i = 0; i < ModulesRegistry::count; i++) {
    ModulesRegistry::modules[i]->setup();
  }
  Serial.print(F("\nready\n"));
}


void loop()
{
  // Call loop() for every module:
  unsigned int i;
  for (i = 0; i < ModulesRegistry::count; i++) {
    ModulesRegistry::modules[i]->loop();
  }
}
