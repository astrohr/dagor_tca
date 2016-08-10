#include <Arduino.h>
#include "modules.hpp"

#ifndef SERIAL_COMM_HPP
#define SERIAL_COMM_HPP


#ifndef SERIAL_BAUD_RATE
#define SERIAL_BAUD_RATE 9600
#endif


// match buffer length to protocol command length, if protocol module is present:
#ifdef PROTOCOL_HPP
#define SERIAL_INPUT_BUFFER_LEN PROTOCOL_COMMAND_LEN
#else
#define SERIAL_INPUT_BUFFER_LEN 40
#endif  // #ifdef PROTOCOL_HPP


class SerialComm : public Module
{
private:
  // buffer for reading commands via serial bus:
  char data_received[SERIAL_INPUT_BUFFER_LEN] = "";
  // length of the received data in the buffer:
  uint8_t data_received_len = 0;
  // set to true when newline is received via serial bus:
  bool reading_complete = false;
  // 1-byte intermediary buffer, for reading serial bus 1 character at a time:
  char incoming_byte = 0;

public:
  void setup();
  void loop();
};


SerialComm* serial_comm = (SerialComm*) ModulesRegistry::add(new SerialComm());


#endif  // #ifndef SERIAL_COMM_HPP
