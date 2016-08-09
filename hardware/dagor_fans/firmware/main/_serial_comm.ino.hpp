#include <Arduino.h>
#include "modules.hpp"

#include "_serial_comm.hpp"

#ifdef SERIAL_COMM_INO_HPP
#error The .ino.hpp file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define SERIAL_COMM_INO_HPP



void SerialComm::setup()
{
  // other modules might use Serial, so this is called in main.ino:
  //Serial.begin(SERIAL_BAUD_RATE);

  // wait for serial port to become available before moving on:
  //while (!Serial) ;
}


void SerialComm::loop()
{

  // read incoming command, byte by byte:
  while (Serial.available() > 0) {

    // but only if previous command had already been submitted to protocol module:
    if (reading_complete) {
      // "reading_complete" is internal flag meaning "got a full command, haven't dumped it to protocol yet"
      break;
    }

    // read a char from serial:
    incoming_byte = (char) Serial.read();

    if (incoming_byte == '\n') {
      // terminate the string with a null character:
      data_received[data_received_len] = '\0';
      // set a flag:
      reading_complete = true;

    } else if (incoming_byte == '\r') {
      // ignore carrige returns

    } else {
      // append the char to string:
      data_received[data_received_len] = incoming_byte;
      // increment string length counter:
      data_received_len += 1;
    }
  }

  // send received string to protocol module:
  if (reading_complete) {

    #ifdef PROTOCOL_HPP
    if (protocol->busy) {
      // ... but wait until it becomes available:
      return;
    }
    #endif

    #ifdef PROTOCOL_HPP
    // Send command to protocol module:
    memcpy(protocol->setCommand, data_received, PROTOCOL_COMMAND_LEN);
    // Set printer for reply:
    Serial.flush();  // need to flush first, otherwise it might freeze (for whatever reason)
    protocol->setReplyPrinter = &Serial;

    #else  // #ifdef PROTOCOL_HPP
    Serial.print(F("Got command:\n"));
    Serial.print(data_received);
    Serial.print(F("\n"));
    Serial.print(F("! Compiled without protocol module, discarding command !\n"));

    #endif  // #ifdef PROTOCOL_HPP

    // reset internal flags:
    data_received[0] = '\0';
    data_received_len = 0;
    reading_complete = false;

    // ready to receive next command via Serial...

  }
}
