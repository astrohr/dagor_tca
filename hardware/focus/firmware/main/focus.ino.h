#include <Arduino.h>



// import stepper library
#include <AccelStepper.h>

#define DEFAULT_SPEED 240
#define SERIAL_BAUD_RATE 9600
#define HUMAAN_LOOP_M 100000
#define LOGIC_LOOP_M 100
#define BLINK_INTERVAL_M 500000


/*

# code conventions

Suffix "_m" signifies "micros", i.e. microseconds

*/

///// PINS /////
typedef struct {
    int motor1_1;  // 10;
    int motor1_2;  // 9;
    int motor2_1;  // 11;
    int motor2_2;  // 8;

    int buttonNeg;  // 5;
    int buttonPos;  // 4;

    int stopNeg;  // 3;
    int stopPos;  // 2;

    int led;  // 13;
} Pin;
Pin pin = {10, 9, 11, 8, 5, 4, 3, 2, 13};


// init stepper library
AccelStepper stepper(AccelStepper::HALF4WIRE, pin.motor1_1, pin.motor1_2, pin.motor2_1, pin.motor2_2, false);


///// ACTION /////
typedef struct {
    byte NOOP;
    byte GO_POS;
    byte GO_NEG;
    byte STOP;
} Action;
Action action = {0, 1, 2, 3};


///// BUTTON /////
typedef struct {
    byte NONE;
    byte GO_POS;
    byte GO_NEG;
} Button;
Button button = {0, 1, 2};


///// STATUS /////
typedef struct {
    boolean in_range;
    boolean can_go_pos;
    boolean can_go_neg;
    boolean motor_enabled;
    long positionCurrent;
    long positionTarget;
    int action;  // TODO remove action fully, or use it for step and full_speed
    int button;
    unsigned int same_button_count;
    unsigned long max_delta_m;  // record the longest run-time of the main loop
} Status;
Status status_defaults = {true, true, true, false, 0, 0, action.NOOP, button.NONE, 0, 0};
Status status = status_defaults;
Status status_previous = status_defaults;


///// CONFIG /////
typedef struct {
    unsigned int speed;
    unsigned int acc;
    unsigned int range;  // steps between limit switches
} Config;
Config config_defaults = {DEFAULT_SPEED, 0, 0};
Config config = config_defaults;


///// TIMERS /////
typedef struct {
    unsigned long current_m;
    unsigned long last_m;
    unsigned long logic_m;
    unsigned long humaan_m;
    unsigned long blink_m;
} Timers;
Timers timers = {0, 0, 0, 0, 0};


///// SERIAL /////
typedef struct {
    String data_received;
    boolean reading_complete;
} SerialComm;
SerialComm _serial = {"", false};



/*
 * Setup on (re)boot
 */
void setup(){
	Serial.begin(SERIAL_BAUD_RATE);

    pinMode(pin.buttonNeg, INPUT);
    digitalWrite(pin.buttonNeg, HIGH);
    pinMode(pin.buttonPos, INPUT);
    digitalWrite(pin.buttonPos, HIGH);

    pinMode(pin.stopNeg, INPUT);
    digitalWrite(pin.stopNeg, HIGH);
    pinMode(pin.stopPos, INPUT);
    digitalWrite(pin.stopPos, HIGH);

	pinMode(pin.led, OUTPUT);

    stepperConfig(DEFAULT_SPEED);

    Serial.println("ready");
}


/*
 * Configure stepper max speed and accerelation ramp based on the speed
 */
void stepperConfig(unsigned int speed)
{
    int acc;
    config.speed = abs(speed);
    acc = (int) (500 - config.speed) * -3 + 500;
    if (acc < 200) acc = 200;
    config.acc = acc;
    stepper.setMaxSpeed(config.speed);
    stepper.setAcceleration(config.acc);
}



/*
 * Main loop
 * Triggers sub-loops at regular intervals
 */
void loop(){

    // timing;
    timers.current_m = micros();
    unsigned long delta_m = (unsigned long)(timers.current_m - timers.last_m);
    if (status.max_delta_m < delta_m) {
        status.max_delta_m = delta_m;
    }
    timers.last_m = timers.current_m;

    // humaan:
    if ((unsigned long)(timers.current_m - timers.humaan_m)  >= HUMAAN_LOOP_M) {
        timers.humaan_m = timers.current_m;
        humaan_loop();
    }

    // logic & serial:
    if ((unsigned long)(timers.current_m - timers.logic_m)  >= LOGIC_LOOP_M) {
        timers.logic_m = timers.current_m;
        logic_loop();
        serial_loop();
    }

    // stepper loop (always):
    stepper_loop();

}

/* Stepper loop
 * Take care of internal stepper logic.
 */
void stepper_loop()
{
    stepper.run();
}


/* Logic loop
 * Handle fast-interval logic
 */
void logic_loop()
{
    // BLINK
    if ((unsigned long)(timers.current_m - timers.blink_m) >= BLINK_INTERVAL_M) {
        timers.blink_m = timers.current_m;
        digitalWrite(pin.led, !digitalRead(pin.led));
    }

    status.positionCurrent = stepper.currentPosition();

    // LIMIT SWITCHES - READ
    status.can_go_neg = digitalRead(pin.stopNeg) == LOW;
    status.can_go_pos = digitalRead(pin.stopPos) == LOW;
    status.in_range = status.can_go_pos && status.can_go_neg;

    // TODO MOVE TO SEPARATE FOO
    long distance_to_go = status.positionTarget - status.positionCurrent;

    // prevent moving outside allowed range
    if (distance_to_go > 0 && !status.can_go_pos) {
        status.positionTarget = status.positionCurrent;
    } else if (distance_to_go < 0 && !status.can_go_neg) {
        status.positionTarget = status.positionCurrent;
    }


    // do the actiual stepping if needed
    // or power down motor if not
    if (status.positionTarget != status.positionCurrent) {
        motor_enable();
        stepper.moveTo(status.positionTarget);

    } else {
        // TODO prevent abrupt stop
        motor_disable();
    }

    status_previous.in_range = status.in_range;
}


/* Human loop
 * Handle human interaction
 */
void humaan_loop()
{
    status.button = readButtons();

    signed long position_delta = 0;

    if (status.button != button.NONE) {
        position_delta = config.speed / 1;
        if (status.button == button.GO_NEG) {
            position_delta = -1 * position_delta;
        }
    }
    if (((position_delta < 0) && (status.can_go_neg)) || ((position_delta > 0) && (status.can_go_pos))) {
        status.positionTarget = status.positionCurrent + position_delta;
    }

    // TODO enable short steps, maybe
    status_previous.button = status.button;
}


void motor_enable()
{
    if (!status.motor_enabled) {
        stepper.enableOutputs();
        status.motor_enabled = true;
    }
}


void motor_disable()
{
    if (status.motor_enabled) {
        stepper.disableOutputs();
        status.motor_enabled = false;
    }
}


/* Read buttons
 * simple negative logic
 */
int readButtons()
{
	if (digitalRead(pin.buttonPos) == LOW) {
		return button.GO_POS;
    } else if (digitalRead(pin.buttonNeg) == LOW) {
        return button.GO_NEG;
	}
	return button.NONE;
}



void serial_loop()
{
    while (Serial.available() > 0) {
        char incomingByte = (char)Serial.read();

        if (incomingByte == '\n') {
            _serial.reading_complete = true;
        } else {
            _serial.data_received += incomingByte;
        }
    }

    if (_serial.reading_complete) {

        Serial.println(_serial.data_received);
        // status buffer:
        if (_serial.data_received.substring(0, 13)  == "set_max_speed") {
            String data = _serial.data_received.substring(13);
            char buf[data.length() + 1];
            data.toCharArray(buf, data.length() + 1);
            int value = (int) atof(buf);  // TODO use atoi ?
            stepperConfig(value);
        } else if (_serial.data_received.substring(0, 7)  == "step_by") {
            String data = _serial.data_received.substring(7);
            char buf[data.length() + 1];
            data.toCharArray(buf, data.length() + 1);
            int value = (int) atof(buf);  // TODO use atoi ?
            status.positionTarget = status.positionCurrent + value;
        } else if (_serial.data_received  == "status") {
            printStatus();
        }

        _serial.data_received = "";
        _serial.reading_complete = false;
    }
}


void printStatus()
{
    Serial.print("enabled: ");
    Serial.println(status.motor_enabled ? "YES" : "NO");
    Serial.print("moving: ");
    Serial.println(stepper.distanceToGo() ? "YES" : "NO");
    Serial.print("can_go_neg: ");
    Serial.println(status.can_go_neg ? "YES" : "NO");
    Serial.print("can_go_pos: ");
    Serial.println(status.can_go_pos ? "YES" : "NO");
    Serial.print("max_speed: ");
    Serial.println(config.speed);
    Serial.print("distance_to_go: ");
    Serial.println(stepper.distanceToGo());
    Serial.print("positionTarget: ");
    Serial.println(status.positionCurrent);
    Serial.print("position: ");
    Serial.println(status.positionCurrent);
    Serial.print("button: ");
    if (status.button == button.NONE) {
        Serial.println("NONE");
    } else if (status.button == button.GO_NEG) {
        Serial.println("GO_NEG");
    } else if (status.button == button.GO_POS) {
        Serial.println("GO_POS");
    }
}
//*********************************************************************
