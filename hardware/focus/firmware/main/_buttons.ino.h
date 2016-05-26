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

  if (millis_now - last_millis < BUTTONS_INTERVAL) {
    return;
  }
  last_millis = millis_now;

  // first, handle case when button not pressed (it's simpler):
  if (!_button_up) {
    // maybe button was pressed and released within BUTTONS_HOLD_TIME:
    if (pending_up) {
      get.button_up_click = true;
      click_up = millis_now;
      pending_up = false;
    }
    // in any case, it's not held:
    get.button_up_hold = false;
    pressed_up = 0;
  }
  if (!_button_dn) {
    // maybe button was pressed and released within BUTTONS_HOLD_TIME:
    if (pending_dn){
      get.button_dn_click = true;
      click_dn = millis_now;
      pending_dn = false;
    }
    // in any case, it's not held:
    get.button_dn_hold = false;
    pressed_dn = 0;
  }
  // timeout clicks if not yet cleared:
  if (get.button_up_click && millis_now - click_up >= BUTTONS_CLICK_TIMEOUT) {
    get.button_up_click = false;
  }
  if (get.button_dn_click && millis_now - click_dn >= BUTTONS_CLICK_TIMEOUT) {
    get.button_dn_click = false;
  }

  // handle just-pressed:
  if (_button_up && ! pending_up && ! get.button_up_hold) {
    pending_up = true;
    pressed_up = millis_now;
  }
  if (_button_dn && ! pending_dn && ! get.button_dn_hold) {
    pending_dn = true;
    pressed_dn = millis_now;
  }

  // handle case when button is still pressed:
  // (fall-trough from "just-pressed" case, but that's harmless)
  if (_button_up) {
    if (millis_now - pressed_up >= BUTTONS_HOLD_TIME) {
      get.button_up_hold = true;
      get.button_up_click = false;
      pending_up = false;
      pressed_up = 0;
    }
  }
  if (_button_dn) {
    if (millis_now - pressed_dn >= BUTTONS_HOLD_TIME) {
      get.button_dn_hold = true;
      get.button_dn_click = false;
      pending_dn = false;
      pressed_dn = 0;
    }
  }

  get.button_up = get.button_up_click || get.button_up_hold;
  get.button_dn = get.button_dn_click || get.button_dn_hold;

}  // void Buttons::loop()
