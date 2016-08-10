/*
   Implementation of Relays module

*/

// Start every module with at least these two includes:
#include <Arduino.h>
#include "modules.hpp"
// Also include this module's header file:
#include "_relays.hpp"

// Unlike header file (.hpp), module implementation file (.ino.hpp) must not be included multiple times:
#ifdef RELAYS_IMPLEMENTATION
#error The .ino.hpp file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define RELAYS_IMPLEMENTATION


// Module's setup() function:
// (copied from default RelaysWithoutDelay.ino example)
void Relays::setup()
{
  relayPin1 = 2;
  relayPin2 = 3;
  relayPin3 = 4;
  relayPin4 = 5;

  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);

  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);
  digitalWrite(relayPin3, HIGH);
  digitalWrite(relayPin4, HIGH);

  interval = 1000;

  Serial.println("Ok ziv sam");
  Serial.flush();
}


// Module's loop() function:
// (copied from default RelaysWithoutDelay.ino example, with some comments added)
void Relays::loop()
{
  if (setRelay1 == RELAYS_ON){
    digitalWrite(relayPin1, LOW);
    setRelay1 = RELAYS_NOOP;
  }
  else if (setRelay1 == RELAYS_OFF) {
    digitalWrite(relayPin1, HIGH);
    setRelay1 = RELAYS_NOOP;
  }
  if (setRelay2 == RELAYS_ON){
    digitalWrite(relayPin2, LOW);
    setRelay2 = RELAYS_NOOP;
  }
  else if (setRelay2 == RELAYS_OFF) {
    digitalWrite(relayPin2, HIGH);
    setRelay2 = RELAYS_NOOP;
  }
  if (setRelay3 == RELAYS_ON){
    digitalWrite(relayPin3, LOW);
    setRelay3 = RELAYS_NOOP;
  }
  else if (setRelay3 == RELAYS_OFF) {
    digitalWrite(relayPin3, HIGH);
    setRelay3 = RELAYS_NOOP;
  }
  if (setRelay4 == RELAYS_ON){
    digitalWrite(relayPin4, LOW);
    setRelay4 = RELAYS_NOOP;
  }
  else if (setRelay4 == RELAYS_OFF) {
    digitalWrite(relayPin4, HIGH);
    setRelay4 = RELAYS_NOOP;
  }
}
