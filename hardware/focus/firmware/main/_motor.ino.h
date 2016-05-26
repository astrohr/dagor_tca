#include <Arduino.h>
#include "modules.h"

#include "_motor.h"


#ifdef MOTOR_INO_H
#error The .ino.h file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define MOTOR_INO_H


#include "_status.h"


#define MOTOR_MAX_SPEED 800
#define MOTOR_FULL_SPEED 250
#define MOTOR_ACC_MIN 150


void Motor::setup()
{
  pinMode(MOTOR_1_1, OUTPUT);
  pinMode(MOTOR_1_2, OUTPUT);
  pinMode(MOTOR_2_1, OUTPUT);
  pinMode(MOTOR_1_2, OUTPUT);


  // TODO EEPROM this, and configure via serial
  int acc;
  int speed = MOTOR_FULL_SPEED;
  acc = (int) 3 * speed - 1000;
  if (acc < MOTOR_ACC_MIN) acc = MOTOR_ACC_MIN;
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(acc);
}


void Motor::loop()
{
  // call AccepStepper library to do its thing:
  stepper.run();

  uint32_t millis_now = millis();
  current_position = stepper.currentPosition();
  get.position = current_position;

  // don't check position too often, we must give stepper a chanse to actually move:
  if (millis_now - last_millis < MOTOR_STEPPER_LOGIC_INTERVAL) {
    return;
  }
  last_millis = millis_now;

  if (set.position) {
    if (get.idle) {
      int32_t delta = set.position_value - current_position;
      current_position += delta;
      target_position += delta;
      stepper.setCurrentPosition(current_position);
      get.position = current_position;
Serial.println(F("New position: "));
Serial.println(current_position);
    }
    set.position = false;
    set.position_value = 0;
  }

  // check if need to stop hard (by a limit switch):
  bool flying_by = false;
  if (status->get.stopping_hard && ! stopping_hard) {
    stopping_hard = true;
    flying_by = true;
    target_position = current_position;
    // cancel any move_by that might be set:
    set.move_by = 0;
  }

  // translate "move_by":
  if (set.move_by) {
    target_position = current_position + set.move_by;
  }


  // do the actiual stepping if needed, or power down motor if not:
  if (target_position != current_position || flying_by) {
      stepper.enableOutputs();
      stepper.moveTo(target_position);
      get.idle = false;
  } else {
      stepper.disableOutputs();  // TODO Maybe this is not needed at all? Will driver board keep colis energised anyway? Should it? Do we care? 42?
      get.idle = true;
      stopping_hard = false;
  }

  // reset setters:
  set.move_by = 0;

  // set getters:
  long distance_to_go = stepper.distanceToGo();
  if (distance_to_go > 0) {
    get.direction = 1;
  } else if (distance_to_go < 0) {
    get.direction = -1;
  } else {
    get.direction = 0;
  }

}  // void Motor::loop()
