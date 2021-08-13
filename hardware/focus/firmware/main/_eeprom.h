/** EEPROM module

    Store and read stuff

*/

#include <Arduino.h>
#include <EEPROM.h>
#include "modules.h"


#ifndef EEPROM_H
#define EEPROM_H


typedef struct {
  int32_t position;
} EepromGet;


typedef struct {
  int32_t position;
  bool write_position;
} EepromSet;



class Eeprom: public Module
{
  private:  // members available only from this module
    bool ready;
    int position_loc;
  public:  // members available from everywhere
    // public member variables:
    EepromGet get;
    EepromSet set;
    // public member functions:
    void setup();
    void loop();

    // Initialization of member variables (both private and public):
    Eeprom():
      ready{false},
      position_loc{0},
      get{
        0,  // position
      },
      set{
        0,  // position
        false,  // write_positio
      }
    {};
};

// Instantiate the module class:
Eeprom* eeprom = (Eeprom*) ModulesRegistry::add(new Eeprom());


#endif  // #ifndef EEPROM_H
