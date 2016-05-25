#include <Arduino.h>
#include "modules.h"

#include "_motor.h"


#ifdef MOTOR_INO_H
#error The .ino.h file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define MOTOR_INO_H


#define MOTOR_MAX_SPEED 1000


void Motor::setup()
{
  pinMode(MOTOR_1_1, OUTPUT);
  pinMode(MOTOR_1_2, OUTPUT);
  pinMode(MOTOR_2_1, OUTPUT);
  pinMode(MOTOR_1_2, OUTPUT);

  int acc;
  int speed = 500;
  acc = (int) (500 - speed) * -3 + 500;
  if (acc < 200) acc = 200;
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(acc);
}


void Motor::loop()
{
  // call AccepStepper library to do its thing:
  stepper.run();

  uint32_t millis_now = millis();
  current_position = stepper.currentPosition();

  // don't check position too often, we must give stepper a chanse to actually move:
  if (millis_now - last_millis < MOTOR_STEPPER_LOGIC_INTERVAL) {
    return;
  }
  last_millis = millis_now;


  // translate "move_by":
  target_position = current_position + set.move_by;
  set.move_by = 0;

  // figure out where to stop if so commanded:
  bool flying = false;
  if (set.stop) {
    flying = true;  // prevent abrupt stop, because target and current position match, but motor is possibly going fast
    target_position = current_position;
  }

  // do the actiual stepping if needed, or power down motor if not:
  if (target_position != current_position || flying) {
      stepper.enableOutputs();
      stepper.moveTo(target_position);

  } else {
      stepper.disableOutputs();  // TODO Maybe this is not needed at all? Will driver board keep colis energised anyway? Should it? Do we care? 42?
  }

}  // void Motor::loop()
