/** Fans module

    A variation on classic fans.ino example

*/

// Start every module with at least these two includes:
#include <Arduino.h>
#include "modules.hpp"

// Include any modules that depend on this one here:
// (fans module has no dependences)


// This guard enables module's header file to be included multiple times:
#ifndef FANS_HPP
#define FANS_HPP
#define FANS_NOOP -1
#define FANS_MAX 2

// Module:
class Fans: public Module
{
  private:  // members available only from this module
    int _fan1 = 0;
    int _fan2 = 0;
  public:  // members available from everywhere
    // public member variables:
    int setFan1 = FANS_NOOP;
    int setFan2 = FANS_NOOP;

    int fan1 = 0;
    int fan2 = 0;

    // public member functions:
    void setup();
    void loop();
};

// Instantiate the module class:
Fans* fans = (Fans*) ModulesRegistry::add(new Fans());


#endif  // #ifndef FANS_HPP
