/** Relays module

    A variation on classic relays.ino example

*/

// Start every module with at least these two includes:
#include <Arduino.h>
#include "modules.hpp"

// Include any modules that depend on this one here:
// (relays module has no dependences)


// This guard enables module's header file to be included multiple times:
#ifndef RELAYS_HPP
#define RELAYS_HPP


#define RELAYS_NOOP 0
#define RELAYS_ON 1
#define RELAYS_OFF 2

// Module:
class Relays: public Module
{
  private:  // members available only from this module
    int relayPin1;
    int relayPin2;
    int relayPin3;
    int relayPin4;
  public:  // members available from everywhere
    // public member variables:
  	byte setRelay1 = RELAYS_NOOP;
    byte setRelay2 = RELAYS_NOOP;
    byte setRelay3 = RELAYS_NOOP;
    byte setRelay4 = RELAYS_NOOP;

    unsigned long interval;
    unsigned long previousMillis;
    // public member functions:
    void setup();
    void loop();
};

// Instantiate the module class:
Relays* relays = (Relays*) ModulesRegistry::add(new Relays());


#endif  // #ifndef RELAYS_HPP
