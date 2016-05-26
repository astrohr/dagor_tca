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

  //print(&Serial);

  // by default, do nothing:
  int future_action = STATUS_COMMAND_NOOP;
  int future_buttons_direction = STATUS_COMMAND_NOOP;
  // movement for motor:
  int32_t move_by = 0;

  // don't bother with anyuthing else while stopping because of a limit switch:
  if (get.stopping_hard) {
    // ... only detect when stopping is finished:
    if (motor->get.idle) {
      get.stopping_hard = false;
    }
    return;
  }

  // set motion flags:
  if (motor->get.idle) {
    get.stopping_hard = false;
    get.stopping_soft = false;
    get.moving_up = false;
    get.moving_dn = false;
    // also, we don't intend anything at this point:
    get.intent_up = false;
    get.intent_dn = false;
    // and any action has been accomplished, so clear that as well:
    get.action = STATUS_COMMAND_NOOP;
    get.idle = true;

  } else {
    get.moving_up = motor->get.direction > 0;
    get.moving_dn = motor->get.direction < 0;
  }

  // set limit switches:
  get.can_go_up = ! digitalRead(STATUS_LIMIT_UP);
  get.can_go_dn = ! digitalRead(STATUS_LIMIT_DN);


  // handle hard stopping, i.e. limit switches:
  if (!get.can_go_up && get.moving_up) {
    get.stopping_hard = true;
    return;
  }
  if (!get.can_go_dn && get.moving_dn) {
    get.stopping_hard = true;
    return;
  }
  // hard stop can also be triggerd by setter:
  if (set.hard_stop) {
    get.stopping_hard = true;
    set.hard_stop = false;
    return;
  }


  // read command from buttons:
  #ifdef BUTTONS_H
  if (buttons->get.button_up && ! buttons->get.button_dn) {
    future_buttons_direction = STATUS_COMMAND_UP;
  } else if (! buttons->get.button_up && buttons->get.button_dn) {
    future_buttons_direction = STATUS_COMMAND_DN;
  } else {
    // both buttons or no buttons:
    future_buttons_direction = STATUS_COMMAND_NOOP;
  }

  // set action from button command:
  if (future_buttons_direction == STATUS_COMMAND_UP && get.can_go_up) {
    future_action = STATUS_COMMAND_UP;
    if (buttons->get.button_up_hold) {
      move_by = STATUS_LARGE_MOVE_DISTANCE;
    } else if (buttons->get.button_up_click) {
      move_by = STATUS_MANUAL_MOVE_DISTANCE;
      buttons->set.clear = true;
    }
  // ... same for down:
  } else if (future_buttons_direction == STATUS_COMMAND_DN && get.can_go_dn) {
    future_action = STATUS_COMMAND_DN;
    if (buttons->get.button_dn_hold) {
      move_by = -1 * STATUS_LARGE_MOVE_DISTANCE;
    } else if (buttons->get.button_dn_click) {
      move_by = -1 * STATUS_MANUAL_MOVE_DISTANCE;
      buttons->set.clear = true;
    }
  // clear buttons for future loop:
  } else {
      future_buttons_direction = STATUS_COMMAND_NOOP;
  }

  // if a button was just released, set quick stop:
  if (!future_action && get.buttons_direction) {
    move_by = STATUS_RAMP_MOVE_DISTANCE * get.buttons_direction;
    future_action = get.buttons_direction;  // should be the same as get.action
  }

  #endif  // #ifdef BUTTONS_H


  // action from setter (i.e. from protocol):
  if (future_action == STATUS_COMMAND_NOOP) {
    if (set.step_by) {
      move_by = set.step_by;
      // clear setter:
      set.step_by = 0;
      // decide future action:
      if (move_by > 0 && ! get.can_go_up) {
        move_by = 0;
      } else if (move_by < 0 && ! get.can_go_dn) {
        move_by = 0;
      } else {
        future_action = (set.step_by > 0) ? STATUS_COMMAND_UP : STATUS_COMMAND_DN;
      }
    }
  }

  // write move_by to motor:
  if (future_action) {
    motor->set.move_by = move_by;
  }

  // set flags for next loop:
  get.buttons_direction = future_buttons_direction;
  get.action = future_action;
  if (future_action) {
    get.intent_up = future_action > 0;
    get.intent_dn = future_action < 0;
  }

  #ifdef RAM_USAGE_H
  ram->measure(2);
  #endif
}


void Status::print(Print* printer)
{
  printer->print(F("position: "));
  printer->println(motor->get.position);
  printer->print(F("idle: "));
  printer->println(get.idle);
  printer->print(F("stopping_hard: "));
  printer->println(get.stopping_hard);
  printer->print(F("moving_up: "));
  printer->println(get.moving_up);
  printer->print(F("moving_dn: "));
  printer->println(get.moving_dn);
  printer->print(F("intent_up: "));
  printer->println(get.intent_up);
  printer->print(F("intent_dn: "));
  printer->println(get.intent_dn);
  printer->print(F("can_go_up: "));
  printer->println(get.can_go_up);
  printer->print(F("can_go_dn: "));
  printer->println(get.can_go_dn);
}
