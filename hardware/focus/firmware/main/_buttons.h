#include <Arduino.h>
#include "modules.h"


#ifndef BUTTONS_H
#define BUTTONS_H

#define BUTTONS_HOLD_TIME 400

#define BUTTONS_UP 4
#define BUTTONS_DN 5  // "dn" means "down"

typedef struct {
  bool button_up;
  bool button_dn;
  bool button_up_holding;
  bool button_dn_holding;
} ButtonsGet;


typedef struct {
  ; // nothing to set for buttons module
} ButtonsSet;


class Buttons : public Module
{
private:
  uint32_t pressed_up;
  uint32_t pressed_dn;
public:
  ButtonsGet get;
  ButtonsSet set;
  void setup();
  void loop();
//initializers:
  Buttons():
    pressed_up{0},
    pressed_dn{0},
    get{
      false,
      false,
      false,
      false,
    },
    set{
    }
  {
  };
};


Buttons* buttons = (Buttons*) ModulesRegistry::add(new Buttons());


#endif  // #ifndef BUTTONS_H
