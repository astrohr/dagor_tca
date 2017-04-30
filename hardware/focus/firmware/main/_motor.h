#include <Arduino.h>
#include "modules.h"


#ifndef MOTOR_H
#define MOTOR_H

#include <AccelStepper.h>

// don't check position too often, we must give stepper a chanse to actually move:
#define MOTOR_STEPPER_LOGIC_INTERVAL 100  // millis

// stepper coils:
#define MOTOR_1_1 10
#define MOTOR_1_2 9
#define MOTOR_2_1 11
#define MOTOR_2_2 8

// logic power:
#define MOTOR_LOGIC_POWER_1 A0
#define MOTOR_LOGIC_POWER_2 A1
#define MOTOR_LOGIC_POWER_3 A2


typedef struct {
  bool idle;
  int direction;
  int32_t position;
} MotorGet;


typedef struct {
  int32_t move_by;
  bool stop;
  bool position;
  int32_t position_value;
} MotorSet;


class Motor : public Module
{
private:
  uint32_t last_millis;
  int32_t current_position;
  int32_t target_position;
  bool stopping_hard;
  AccelStepper stepper;

public:
  MotorGet get;
  MotorSet set;
  void setup();
  void loop();
//initializers:
  Motor():
    last_millis{0},
    current_position{0},
    target_position{0},
    stopping_hard{false},
    stepper(AccelStepper::HALF4WIRE, MOTOR_1_1, MOTOR_1_2, MOTOR_2_1, MOTOR_2_2, false),
    get{
      true,  // idle
      0,  // direction
    },
    set{
      0,  // move_by
      false,  // stop
      false,  // position
      0,  // position_value
    }
  {
  };
};


Motor* motor = (Motor*) ModulesRegistry::add(new Motor());


#endif  // #ifndef MOTOR_H
