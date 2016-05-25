#include <Arduino.h>
#include "modules.h"

#include "_protocol.h"


#ifdef PROTOCOL_INO_H
#error The .ino.h file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define PROTOCOL_INO_H


// optional dependances (include them in main.ino before this module):

// TODO chould we check for CLOCK_H or CLOCK_INO_H here? (Ditto for other modules)

void Protocol::setup()
{}


void Protocol::loop()
{
  // we are only conserned with handling incomming commands:
  if (set.command[0] == '\0') {
    // (empty strings start with a null-character)
    return;
  }

  if (!set.reply) {
    // reply printer must also be set
    // TODO use Serial only in DEBUG mode!
    Serial.print(F("Got a command but no printer!\n"));
    return;
  }
  // store reply_printer locally to avoid accidental overwrite by other modules:
  reply_printer = set.reply;


  // we'll need a fancy String object, for simplr parsing:
  String command = set.command;

  // set busy flag (just in case we have to handle this command over multiple cycles):
  // BTW, that's not used yet
  get.busy = true;


  // status buffer:
  if (command == "status") {
    // calculate how many lines are going to be in the reply:
    int status_reply_count = 0;
    #ifdef RADIO_H
    status_reply_count += 2;
    #endif
    #ifdef CLOCK_H
    status_reply_count += 2;
    #endif
    #ifdef SENSORS_H
    status_reply_count += 1;
    #endif
    #ifdef SENSOR_RAIN_H
    status_reply_count += 1;
    #endif
    // first line of the reply, with the number of lines to follow:
    reply_printer->print(F("status "));
    reply_printer->print(status_reply_count);
    reply_printer->print(F("\n"));
    // radio module:
    #ifdef RADIO_H
    reply_printer->print(F("radio id "));
    reply_printer->print(radio->get.address);
    reply_printer->print(F("\n"));
    reply_printer->print(F("gateway id "));
    reply_printer->print(radio->get.gateway_address);
    reply_printer->print(F("\n"));
    #endif
    // clock module:
    #ifdef CLOCK_H
    reply_printer->print(F("clock ready "));
    reply_printer->print(clock->get.is_ready);
    reply_printer->print(F("\n"));
    reply_printer->print(F("timestamp "));
    reply_printer->print(clock->get.timestamp);
    reply_printer->print(F("\n"));
    #endif
    // sensors module:
    #ifdef SENSORS_H
    reply_printer->print(F("overflow "));
    reply_printer->print(sensors->get->overflow);
    reply_printer->print(F("\n"));
    #endif
    #ifdef SENSOR_RAIN_H
    reply_printer->print(F("rain sensor value: "));
    if (!sensor_rain->get.is_connected) {
      reply_printer->print(F("N/C"));
    } else {
      reply_printer->print(sensor_rain->get.value);
    }
    reply_printer->print(F("\n"));
    #endif
    // TODO more status....

  #ifdef CLOCK_H
  } else if (command.substring(0, 10) == "clock set ") {
    String timestamp = command.substring(10);
    char buff[timestamp.length() + 1];
    timestamp.toCharArray(buff, timestamp.length() + 1);
    clock->set.timestamp = strtoul(buff, NULL, 10);
    reply_printer->print(F("ok 1\n"));
    reply_printer->print(clock->set.timestamp);
    reply_printer->print(F("\n"));

  } else if (command == "clock") {
    if (clock->get.is_ready) {
      reply_printer->print(F("clock 1\n"));
      reply_printer->print(clock->get.timestamp);
      reply_printer->print(F("\n"));
    } else {
      reply_printer->print(F("error 1\n"));
      reply_printer->print(F("clock is disabled\n"));
    }

  } else if (command == "clock counter") {
    reply_printer->print(F("clock counter 1\n"));
    reply_printer->print(*clock->get.counter);
    reply_printer->print(F("\n"));

  } else if (command == "clock reset") {
    reply_printer->print(F("ok 0\n"));
    clock->set.reset_counter = true;
  #endif  // #ifdef CLOCK_H

  #ifdef RADIO_H
  } else if (command == "radio id set ") {
    String address = command.substring(13);
    char buff[address.length() + 1];
    address.toCharArray(buff, address.length() + 1);
    radio->set.address = (uint8_t) atoi(buff);
    reply_printer->print(F("ok 0\n"));

  } else if (command == "radio gateway set ") {
    String address = command.substring(18);
    char buff[address.length() + 1];
    address.toCharArray(buff, address.length() + 1);
    radio->set.gateway_address = (uint8_t) atoi(buff);
    reply_printer->print(F("ok 0\n"));
  #endif  // #ifdef RADIO_H

  #ifdef SENSORS_TEMP_H
  } else if (command == "temp") {
    if (sensor_temp.get->is_connected) {
      reply_printer->print(F("temp 1\n"));
      reply_printer->print(sensor_temp.get->value);
      reply_printer->print(F("\n"));
    } else {
      reply_printer->print(F("error 1\n"));
      reply_printer->print(F("sensor not connetced\n"));
    }
  #endif  // #ifdef SENSORS_TEMP_H

  #ifdef SENSORS_TIME_H
  } else if (command == "time") {
    if (sensor_time.get->is_connected) {
      reply_printer->print(F("time 1\n"));
      reply_printer->print(sensor_time.get->value);
      reply_printer->print(F("\n"));
    } else {
      reply_printer->print(F("error 1\n"));
      reply_printer->print(F("sensor not connetced\n"));
    }
  #endif  // #ifdef SENSORS_TIME_H

  #ifdef SENSORS_H
  } else if (command == "data counter") {
    reply_printer->print(F("data counter 1\n"));
    reply_printer->print(sensors->get->counter);
    reply_printer->print(F("\n"));

  } else if (command == "data") {
    unsigned char data_count = sensors->get->counter;
    if (sensors->get->overflow) {
      data_count = SENSORS_BUFFER_LEN;
    }
    reply_printer->print(F("data "));
    reply_printer->print(data_count);
    reply_printer->print(F("\n"));
    for (unsigned char i=0; i<data_count; i++) {
      reply_printer->print(clock->timestamp_from_counter(sensors->get->buffer[i].clock_counter));
      reply_printer->print(F("\t"));
      reply_printer->print(sensors->get->buffer[i].sensor_id);
      reply_printer->print(F("\t"));
      reply_printer->print(sensors->get->buffer[i].value);
      reply_printer->print(F("\n"));
    }

  } else if (command == "clear") {
    sensors->set->clear_buffer = true;
    reply_printer->print(F("ok 0\n"));
  #endif  // #ifdef SENSORS_H

  } else if (command == "" || command == "help") {
    int help_reply_count = 2;
    #ifdef CLOCK_H
    help_reply_count += 2;
    #endif
    #ifdef SENSORS_H
    help_reply_count += 1;
    #endif
    reply_printer->print(F("help "));
    reply_printer->print(help_reply_count);
    reply_printer->print(F("\n"));
    reply_printer->print(F("available commands:\n"));
    reply_printer->print(F("status\n"));
    #ifdef CLOCK_H
    reply_printer->print(F("clock\n"));
    reply_printer->print(F("clock set <unix_timestamp>\n"));
    #endif
    #ifdef SENSORS_H
    reply_printer->print(F("data\n"));
    #endif

  } else {
    reply_printer->print(F("error 2\n"));
    reply_printer->print(F("unknown command\n"));
    reply_printer->print(command);
    reply_printer->print(F("\n"));
  }

  // reset flags:
  set.command[0] = '\0';
  get.busy = false;

  // also clear reference to the printer, for good measure:
  set.reply = NULL;
  reply_printer = NULL;
}
