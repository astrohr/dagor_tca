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


#define STATUS_RAMP_MOVE_DISTANCE 300
#define STATUS_MANUAL_MOVE_DISTANCE 100
#define STATUS_LARGE_MOVE_DISTANCE 2000


typedef struct {
  int8_t buttons_direction;  // -1, 0, 1
  bool can_go_up;
  bool can_go_dn;
  int8_t action;
  bool idle;
} StatusGet;


typedef struct {
} StatusSet;


class Status : public Module
{
private:
  bool moving_single_manual_move;
  bool flying_up;
  bool flying_dn;
public:
  StatusGet get;
  StatusSet set;
  void setup();
  void loop();
//initializers:
  Status():
    moving_single_manual_move{false},
    flying_up{false},
    flying_dn{false},
    get{
      0,  // button_direction
      false,  // can_go_up
      false,  // can_go_dn
      STATUS_COMMAND_NOOP,
      true,  // idle
    },
    set{
    }
  {
  };
};


Status* status = (Status*) ModulesRegistry::add(new Status());


#endif  // #ifndef STATUS_H
