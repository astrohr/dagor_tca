#include <Arduino.h>
#include "modules.h"

#include "_status.h"


#ifdef STATUS_INO_H
#error The .ino.h file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define STATUS_INO_H


#include "_motor.h"
#include "_buttons.h"


void Status::setup()
{
  pinMode(STATUS_LIMIT_UP, INPUT_PULLUP);
  pinMode(STATUS_LIMIT_DN, INPUT_PULLUP);
}


void Status::loop()
{

  // don't check status too often, it's bad for you...
  uint32_t millis_now = millis();
  if (millis_now - last_millis < STATUS_INTERVAL) {
    return;
  }
  last_millis = millis_now;

  // limit switches:
  bool _limited_up = digitalRead(STATUS_LIMIT_UP);
  bool _limited_dn = digitalRead(STATUS_LIMIT_DN);
  get.can_go_up = ! _limited_up;
  get.can_go_dn = ! _limited_dn;

  bool _future_flying_up = false;
  bool _future_flying_dn = false;
  // movement for motor:
  int32_t move_by = 0;


  // by default, do nothing:
  get.action = STATUS_COMMAND_NOOP;

  // read command from buttons:
  #ifdef BUTTONS_H
  if (buttons->get.button_up && ! buttons->get.button_dn) {
    get.buttons_direction = STATUS_COMMAND_UP;
  } else if (! buttons->get.button_up && buttons->get.button_dn) {
    get.buttons_direction = STATUS_COMMAND_DN;
  } else {
    // both buttons or no buttons:
    get.buttons_direction = STATUS_COMMAND_NOOP;
  }

  // now figure out what action should be taken:
  if (get.buttons_direction == STATUS_COMMAND_UP && get.can_go_up) {
    get.action = STATUS_COMMAND_UP;

    if (buttons->get.button_up_holding) {
      move_by = STATUS_LARGE_MOVE_DISTANCE;
      _future_flying_up = true;

    } else {  // single click:
      _future_flying_up = false;
      move_by = STATUS_MANUAL_MOVE_DISTANCE;
    }

  // ... same for down:
  } else if (get.buttons_direction == STATUS_COMMAND_DN && get.can_go_dn) {
    get.action = STATUS_COMMAND_DN;

    if (buttons->get.button_dn_holding) {
      move_by = -1 * STATUS_LARGE_MOVE_DISTANCE;
      _future_flying_dn = true;

    } else {  // single click:
      _future_flying_dn = false;
      move_by = -1 * STATUS_MANUAL_MOVE_DISTANCE;
    }


  }

  #endif  // #ifdef BUTTONS_H


  // action from serial:

  if (get.action == STATUS_COMMAND_NOOP) {
    // TOOD
    // also set move_by!
  }



  // if flying just ended (e.g. a button is no longer held):
  if (flying_up && !get.action) {
    move_by = STATUS_RAMP_MOVE_DISTANCE;
    get.action = STATUS_COMMAND_UP;
  }
  if (flying_dn && !get.action) {
    move_by = -1 * STATUS_RAMP_MOVE_DISTANCE;
    get.action = STATUS_COMMAND_DN;
  }

  // limit switch reached:

  if (_limited_up) {
    if (moving_up) {
      get.action = STATUS_COMMAND_STOP;
      move_by = 0;
    } else if (get.action == STATUS_COMMAND_UP) {
      get.action = STATUS_COMMAND_NOOP;
      move_by = 0;
    }
  }
  if (_limited_dn) {
    if (moving_dn) {
      get.action = STATUS_COMMAND_STOP;
Serial.println("STOP");
      move_by = 0;
    } else if (get.action == STATUS_COMMAND_DN) {
      get.action = STATUS_COMMAND_NOOP;
      move_by = 0;
    }
  }

  // issue movement command to motor:
  if (get.action) {
    if (get.action == STATUS_COMMAND_STOP) {
      motor->set.stop = true;
      move_by = 0;
    }
    motor->set.move_by = move_by;
  }

  if (move_by != 0) {
    moving_up = (bool) (move_by > 0);
    moving_dn = (bool) (move_by < 0);
  } else {
    if (motor->get.idle) {
      moving_up = false;
      moving_dn = false;
    }
  }

  flying_up = _future_flying_up;
  flying_dn = _future_flying_dn;

}
