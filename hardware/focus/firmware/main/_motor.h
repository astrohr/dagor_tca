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


typedef struct {
  bool idle;
} MotorGet;


typedef struct {
  int64_t move_by;
  bool stop;
} MotorSet;


class Motor : public Module
{
private:
  uint32_t last_millis;
  int64_t current_position;
  int64_t target_position;
  AccelStepper stepper;

public:
  MotorGet get;
  MotorSet set;
  void setup();
  void loop();
//initializers:
  Motor():
    last_millis{0},
    stepper(AccelStepper::HALF4WIRE, MOTOR_1_1, MOTOR_1_2, MOTOR_2_1, MOTOR_2_2, false),
    get{
      true,  // idle
    },
    set{
      0,  // move_by
      false,  // stop
    }
  {
  };
};


Motor* motor = (Motor*) ModulesRegistry::add(new Motor());


#endif  // #ifndef MOTOR_H
