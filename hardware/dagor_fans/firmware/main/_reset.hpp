/** Reset module

    Self-reset by filling the 10u capacitor between reset and GND.
    Needs a 10u capacitor between reset and GND.
    Did I mention 10u capacitor between reset and GND.
    Look, this is in my clipboard: 10u capacitor between reset and GND.
*/

#include <Arduino.h>
#include "modules.hpp"

#ifndef RESET_HPP
#define RESET_HPP

#ifndef RESET_PIN
  // should be set to MISO pin on SPI header:
  #define RESET_PIN MISO
#endif


class Reset: public Module
{
  private:
    int pin = RESET_PIN;
  public:
    bool trigger = false;
    void setup();
    void loop();
};

Reset* reset = (Reset*) ModulesRegistry::add(new Reset());

#endif  // #ifndef RESET_HPP
