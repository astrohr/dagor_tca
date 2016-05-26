#include <Arduino.h>
#include "modules.h"

#ifndef STATUS_H
#define STATUS_H

// command codes:
#define STATUS_COMMAND_UP 1
#define STATUS_COMMAND_NOOP 0
#define STATUS_COMMAND_DN -1
#define STATUS_COMMAND_STOP 3

// limit switch pins:
#define STATUS_LIMIT_UP 2
#define STATUS_LIMIT_DN 3


#define STATUS_RAMP_MOVE_DISTANCE 200
#define STATUS_MANUAL_MOVE_DISTANCE 100
#define STATUS_LARGE_MOVE_DISTANCE 5000


#define STATUS_INTERVAL 1

typedef struct {
  int8_t action;  // new action issues in a loop
  int8_t buttons_direction;  // -1, 0, 1
  bool can_go_up;  // limit switches, verbatim
  bool can_go_dn;
  bool intent_up;  // intended motor movement
  bool intent_dn;
  bool moving_up;  // actual motor movement, because ramps
  bool moving_dn;
  bool stopping_soft;  // end of flying
  bool stopping_hard;  // end by limit switch, will return to limit position
  bool idle;  // motor stopped
} StatusGet;


typedef struct {
  int32_t step_by;
  bool hard_stop;
} StatusSet;


class Status : public Module
{
private:
  uint32_t last_millis;
public:
  StatusGet get;
  StatusSet set;
  void setup();
  void loop();
  void print(Print* printer);
//initializers:
  Status():
    last_millis{0},
    get{
      STATUS_COMMAND_NOOP,
      0,  // button_direction
      false,  // can_go_up
      false,  // can_go_dn
      false,  // intent_up
      false,  // intent_dn
      false,  // moving_up
      false,  // moving_dn
      false,  // stopping_soft
      false,  // stopping_hard
      true,  // idle
    },
    set{
      0,  // step_by
      false, // hard_stop
    }
  {
  };
};


Status* status = (Status*) ModulesRegistry::add(new Status());


#endif  // #ifndef STATUS_H
