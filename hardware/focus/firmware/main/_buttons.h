#include <Arduino.h>
#include "modules.h"


#ifndef BUTTONS_H
#define BUTTONS_H

#define BUTTONS_INTERVAL 100

#define BUTTONS_HOLD_TIME 400
#define BUTTONS_CLICK_TIMEOUT 200

#define BUTTONS_UP 4
#define BUTTONS_DN 5  // "dn" means "down"

typedef struct {
  bool button_up_click;
  bool button_dn_click;
  bool button_up_hold;
  bool button_dn_hold;
  bool button_up;
  bool button_dn;
} ButtonsGet;


typedef struct {
  bool clear;
} ButtonsSet;


class Buttons : public Module
{
private:
  uint32_t last_millis;
  uint32_t pressed_up;
  uint32_t pressed_dn;
  bool pending_up;
  bool pending_dn;
  uint32_t click_up;
  uint32_t click_dn;
public:
  ButtonsGet get;
  ButtonsSet set;
  void setup();
  void loop();
//initializers:
  Buttons():
    last_millis{0},
    pressed_up{0},
    pressed_dn{0},
    pending_up{false},
    pending_dn{false},
    get{
      false,
      false,
      false,
      false,
      false,
      false,
    },
    set{
      false,
    }
  {
  };
};


Buttons* buttons = (Buttons*) ModulesRegistry::add(new Buttons());


#endif  // #ifndef BUTTONS_H
