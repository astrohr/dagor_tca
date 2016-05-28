/*
   Implementation of RamUsage module

*/

// Start every module with at least these two includes:
#include <Arduino.h>
#include "modules.h"
// Also include this module's header file:
#include "_ram_usage.h"
#include "hardware_config.h"

// Unlike header file (.h), module implementation file (.ino.h) must not be included multiple times:
#ifdef RAM_USAGE_INO_H
#error The .ino.h file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define RAM_USAGE_INO_H


// Module's setup() function:
// (copied from default BlinkWithoutDelay.ino example)
void Ram::setup()
{
  for (step_i=0; step_i<RAM_MEASUREMENTS_N; step_i++) {
    measurements[step_i] = -1;
  }
  step_i = 0;
}


// Module's loop() function:
// (copied from default BlinkWithoutDelay.ino example, with some comments added)
void Ram::loop()
{
  step_i++;

  ram->measure(0);
  if (step_i % step == 0) {
    ram->print_mesaurements();
    step_i = 0;
  }
}
