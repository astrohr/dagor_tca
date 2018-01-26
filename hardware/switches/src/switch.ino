#define SERIAL_BAUD_RATE 9600

///// PINS /////

int relay_1 = 2;
int relay_2 = 3;
int relay_3 = 4;
int relay_4 = 5;

int ledGreen = 13;


///// CONSTS /////  
boolean ON = HIGH;
boolean OFF = LOW;


///// STATUS /////
typedef struct {
    boolean relay_1;
    boolean relay_2;
    boolean relay_3;
    boolean relay_4;
} Status;
Status status = {false, false, false, false};
// TODO make this an array

///// SERIAL /////
typedef struct {
    String data_received;
    bool reading_complete;
} SerialComm;
SerialComm _serial = {"", false};


void setup(){
    Serial.begin(SERIAL_BAUD_RATE);

    pinMode(relay_1, OUTPUT);
    digitalWrite(relay_1, HIGH);
    pinMode(relay_2, OUTPUT);
    digitalWrite(relay_2, HIGH);
    pinMode(relay_3, OUTPUT);
    digitalWrite(relay_3, HIGH);
    pinMode(relay_4, OUTPUT);
    digitalWrite(relay_4, HIGH);

    Serial.println("ready");
}

void pinInputPullup(int pin) {
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
}


void turn(int pin, bool state) {
    digitalWrite(pin, state);
}


void loop() {
    serial_loop();
    hardware_loop();
}


void hardware_loop() {
    turn(relay_1, !status.relay_1);
    turn(relay_2, !status.relay_2);
    turn(relay_3, !status.relay_3);
    turn(relay_4, !status.relay_4);
}


void softwareReboot()
{
    Serial.println("Cannot do that, sorry!");
}


void println_status(int relay_id) {
    switch (relay_id) {
        case 1:
          Serial.print(status.relay_1 ? "ON" : "OFF");        
          break;
        case 2:
          Serial.print(status.relay_2 ? "ON" : "OFF");        
          break;
        case 3:
          Serial.print(status.relay_3 ? "ON" : "OFF");        
          break;
        case 4:
          Serial.print(status.relay_4 ? "ON" : "OFF");        
          break;
        default:
          Serial.print("bad relay id");        
    }
    Serial.println("");    
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

        // status buffer:
        if (_serial.data_received  == "status") {
            Serial.println("status");
            Serial.println("4");
            println_status(1);
            println_status(2);
            println_status(3);
            println_status(4);
        } else if (_serial.data_received.substring(0, 6) == "switch") {
            String switch_str = _serial.data_received.substring(8, 6);
            boolean turn_on = (_serial.data_received.substring(8) == " on") || (_serial.data_received.substring(8) == " ON");
            boolean turn_off = (_serial.data_received.substring(8) == " off") || (_serial.data_received.substring(8) == " OFF");
            Serial.print("switch");
            Serial.println(switch_str);
            if (switch_str == " 1")  {
                if (turn_on) {
                    status.relay_1 = true;
                } else if (turn_off) {
                    status.relay_1 = false;
                }
                println_status(1);
            }
            if (switch_str == " 2")  {
                if (turn_on) {
                    status.relay_2 = true;
                } else if (turn_off) {
                    status.relay_2 = false;
                }
                println_status(2);
            }
            if (switch_str == " 3")  {
                if (turn_on) {
                    status.relay_3 = true;
                } else if (turn_off) {
                    status.relay_3 = false;
                }   
                println_status(3);
            }
            if (switch_str == " 4")  {
                if (turn_on) {
                    status.relay_4 = true;
                } else if (turn_off) {
                    status.relay_4 = false;
                }
                println_status(4);
           }
        } else if (_serial.data_received == "reset") {
            softwareReboot();
        }

        _serial.data_received = "";
        _serial.reading_complete = false;
    }
}
//*********************************************************************