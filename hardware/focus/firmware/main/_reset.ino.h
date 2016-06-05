/*
   Implementation of Reset module

*/

#include <Arduino.h>
#include "modules.h"
#include "_reset.h"

#ifdef RESET_INO_H
#error The .ino.h file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define RESET_INO_H


// Module's setup() function:
// (copied from default BlinkWithoutDelay.ino example)
void Reset::setup()
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}


void Reset::loop()
{
  if (!trigger) return;
  digitalWrite(pin, HIGH);
  while(1){};
}
