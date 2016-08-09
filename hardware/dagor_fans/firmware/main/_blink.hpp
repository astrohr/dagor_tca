/** Blink module

    A variation on classic blink.ino example

*/

// Start every module with at least these two includes:
#include <Arduino.h>
#include "modules.hpp"

// Include any modules that depend on this one here:
// (blink module has no dependences)


// This guard enables module's header file to be included multiple times:
#ifndef BLINK_HPP
#define BLINK_HPP


// Module:
class Blink: public Module
{
  private:  // members available only from this module
    // (blink has none)
  public:  // members available from everywhere
    // public member variables:
  	int ledPin;
  	int ledState;
    unsigned long interval;
    unsigned long previousMillis;
    // public member functions:
    void setup();
    void loop();
};

// Instantiate the module class:
Blink* blink = (Blink*) ModulesRegistry::add(new Blink());


#endif  // #ifndef BLINK_HPP
