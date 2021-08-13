

#define NULL_AZIMUTH 500
#define NOOP 0
#define DOWN 1
#define UP -1

// order includes from general to specific

#include "Arduino.h"
#include "utils.h"

#include "status.h"
#include "encoders.h"
#include "motors.h"
#include "calibration.h"
#include "controllers.h"
#include "serial.h"
#include "door.h"

#include "settings.h"
#include "pid_v1.h"


/***** PINS *****/

const int quadrature_L = 3; // encoder
const int quadrature_H = 2;

const int motor_up = 5;
const int motor_down = 4;
const int motor_speed = 6;

const int door_switch_open = A5; // door switch direct control
const int door_switch_close = A6;

const int controller_up = A1;
const int controller_down = A2;
const int controller_open = A3;
const int controller_close = A4;

const int do_open_pin = 10; // control for door
const int do_close_pin = 11;
const int is_open_pin = 51; // unused
const int is_closed_pin = 53; // unused

const int switch_one = 7; // 1st mehanical switch
const int switch_two = 8; // 2nd mehanical switch
const int switch_three = 9; // 3rd mehanical switch

const int led = 13;

double pid_input;
double pid_output;
double pid_setpoint;

const double kp = 0.27;
const double ki = 0.34;
const double kd = 0;
const int pid_sampletime = 500;

/***** INTERFACES *****/

#define OPEN 1
#define CLOSE -1
#define DOORS_STOP 3


// input_buffer
typedef struct {
    int direction; // 0 nothing    1 down    -1 up        (right hand)
    int doors; // 0 nothing    1 open    -1 close
    double target_azimuth; // target azimuth, default >360
    int dome_speed;
    bool start_calibration;
    bool start_goto;
    bool stop;
} InputBuffer;
InputBuffer INPUT_BUFFER_DEFAULTS = {NOOP, NOOP, NULL_AZIMUTH, 40, false, false, false};
InputBuffer input_buffer = INPUT_BUFFER_DEFAULTS;

PID dome_pid(&pid_input, &pid_output, &pid_setpoint, kp, ki, kd, DIRECT);
