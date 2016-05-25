#include <Arduino.h>
#include "modules.h"

#include "_buttons.h"

#ifdef BUTTONS_INO_H
#error The .ino.h file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define BUTTONS_INO_H



void Buttons::setup()
{
  pinMode(BUTTONS_UP, INPUT_PULLUP);
  pinMode(BUTTONS_DN, INPUT_PULLUP);
}


void Buttons::loop()
{
  // handling buttons separately, but in the same way

  bool _button_up = ! digitalRead(BUTTONS_UP);
  bool _button_dn = ! digitalRead(BUTTONS_DN);
  uint32_t millis_now = millis();

  // first, handle case when button not pressed (it's simpler):
  if (!_button_up) {
    get.button_up = false;
    get.button_up_holding = false;
    pressed_up = 0;
  }
  if (!_button_dn) {
    get.button_dn = false;
    get.button_dn_holding = false;
    pressed_dn = 0;
  }

  // handle just-pressed case:
  if (_button_up && ! get.button_up) {
    get.button_up = true;
    pressed_up = millis_now;
  }
  if (_button_dn && ! get.button_dn) {
    get.button_dn = true;
    pressed_dn = millis_now;
  }

  // handle case when button is still pressed:
  // (fall-trough from "just-pressed" case, but that's harmless)
  if (_button_up) {
    if (millis_now - pressed_up >= BUTTONS_HOLD_TIME) {
      get.button_up_holding = true;
    }
  }
  if (_button_dn) {
    if (millis_now - pressed_dn >= BUTTONS_HOLD_TIME) {
      get.button_dn_holding = true;
    }
  }

}  // void Buttons::loop()
