/** Power module

    Checking the power state of the relay inputs

*/

// Start every module with at least these two includes:
#include <Arduino.h>
#include "modules.hpp"

// Include any modules that depend on this one here:
// (power module has no dependences)

// This guard enables module's header file to be included multiple times:
#ifndef POWER_HPP
#define POWER_HPP

// Module:
class Power: public Module
{
  private:  // members available only from this module
    // (power has none)
    int powerPin = A0;
    int pinStatus;
  public:  // members available from everywhere
    // public member variables:
  	int powerState;
    // public member functions:
    void setup();
    void loop();
};

// Instantiate the module class:
Power* power = (Power*) ModulesRegistry::add(new Power());


#endif  // #ifndef POWER_HPP
