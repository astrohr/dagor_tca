#include <Arduino.h>
#include "modules.h"

#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#ifdef PROTOCOL_H
#include "_protocol.h"
#endif

// Configuration:

#ifndef SERIAL_BAUD_RATE
#define SERIAL_BAUD_RATE 9600
#endif


#ifdef PROTOCOL_H
#define SERIAL_INPUT_BUFFER_LEN PROTOCOL_COMMAND_LEN
#else
#define SERIAL_INPUT_BUFFER_LEN 40
#endif  // #ifdef PROTOCOL_H


class SerialComm : public Module
{
private:
  char data_received[SERIAL_INPUT_BUFFER_LEN];
  uint8_t data_received_len;
  bool reading_complete;
  char incoming_byte;
public:
  void setup();
  void loop();
// initializers:
  SerialComm():
    data_received{""},
    data_received_len{0},
    reading_complete{false},
    incoming_byte{'\0'}
  {};
};



SerialComm* serial_comm = (SerialComm*) ModulesRegistry::add(new SerialComm());


#endif  // #ifndef SERIAL_COMM_H
