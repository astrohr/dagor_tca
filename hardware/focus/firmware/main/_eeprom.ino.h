#include <Arduino.h>
#include "modules.h"

#include "_eeprom.h"


#ifdef EEPROM_INO_H
#error The .ino.h file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define EEPROM_INO_H


#ifdef MOTOR_H
#include "_motor.h"
#endif


void Eeprom::setup()
{
}  // void Eeprom::loop()


void Eeprom::loop()
{
  if (!ready) {
    EEPROM.get(position_loc, get.position);
    motor->set.position_value = get.position;
    motor->set.position = true;
    ready = true;
  }

  #ifdef MOTOR_H
  if (set.write_position == true) {
    EEPROM.put(position_loc, set.position);
    get.position = set.position;
    set.position = 0;
    set.write_position = false;
  }
  #endif

}  // void Eeprom::loop()
