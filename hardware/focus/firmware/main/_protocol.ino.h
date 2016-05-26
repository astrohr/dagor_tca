#include <Arduino.h>
#include "modules.h"

#include "_protocol.h"


#ifdef PROTOCOL_INO_H
#error The .ino.h file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define PROTOCOL_INO_H



void Protocol::setup()
{}


void Protocol::loop()
{

  // handle reply from previous loops:
  bool replied = false;
  if (reply_case == PROTOCOL_REPLY_TEST) {
    reply_printer->print(F("deferred reply\n"));
    replied = true;
  }

  #ifdef MOTOR_H
  if (reply_case == PROTOCOL_REPLY_SET_POSITION) {

    if (motor->get.idle) {
      reply_printer->print(F("ok 1\n"));
      reply_printer->print(motor->get.position);
      reply_printer->print(F("\n"));
    } else {
      reply_printer->print(F("error 1\n"));
      reply_printer->print(F("Motor running\n"));
    }
    replied = true;
  }
  #endif  // #ifdef MOTOR_H

  if (replied) {
    Serial.println("Clearing printer 1");
    // reset flags:
    get.busy = false;
    // also clear reference to the printer, for good measure:
    //set.reply = NULL;
    //reply_printer = NULL;
    Serial.println("Cleared printer 1");
    set.command[0] = '\0';
    reply_case = 0;
    return;
  }


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

Serial.println(F("COMMAND:"));
Serial.println(command);

  // status buffer:
  if (false){

  #ifdef STATUS_G
  } else if (command == "status") {
    // calculate how many lines are going to be in the reply:
    int status_reply_count = 8;
    // first line of the reply, with the number of lines to follow:
    reply_printer->print(F("status "));
    reply_printer->print(status_reply_count);
    reply_printer->print(F("\n"));
    status->print(&Serial);  // TODO this is ok, but it's better to trigger prints via status->set.something

  } else if (command.substring(0, 8)  == "step by ") {
    String data = command.substring(8);
    char buf[data.length() + 1];
    data.toCharArray(buf, data.length() + 1);
    int value = atoi(buf);
    status->set.step_by = value;

  } else if (command == "stop") {
    status->set.hard_stop = true;
  #endif  //  #ifdef STATUS_G

  #ifdef MOTOR_H
  } else if (command == "position") {
    reply_printer->print(F("position 1\n"));
    reply_printer->print(motor->get.position);
    reply_printer->print(F("\n"));

  } else if (command.substring(0, 13)  == "position set ") {
    String data = command.substring(13);
    char buf[data.length() + 1];
    data.toCharArray(buf, data.length() + 1);
    int value = atoi(buf);
    motor->set.position_value = value;
    motor->set.position = true;
    reply_case = PROTOCOL_REPLY_SET_POSITION;
  #endif  //  #ifdef MOTOR_H

  } else if (command == "" || command == "help") {
    int help_reply_count = 5;
    reply_printer->print(F("help "));
    reply_printer->print(help_reply_count);
    reply_printer->print(F("\n"));
    reply_printer->print(F("available commands:\n"));
    reply_printer->print(F("status\n"));
    reply_printer->print(F("step by <number_of_steps>\n"));
    reply_printer->print(F("position\n"));
    reply_printer->print(F("position set <current_number_of_steps>\n"));


  } else if (command == "test") {
      reply_printer->print(F("test 2 "));
      reply_printer->print(F("initial reply\n"));
      reply_case = PROTOCOL_REPLY_TEST;

  } else {
    reply_printer->print(F("error 2\n"));
    reply_printer->print(F("unknown command\n"));
    reply_printer->print(command);
    reply_printer->print(F("\n"));
  }

  // clear command:
  set.command[0] = '\0';

  // clear other flags if replied:
  if (!reply_case) {
    // reset flags:
    get.busy = false;

    // also clear reference to the printer, for good measure:
    //set.reply = NULL;
    //reply_printer = NULL;
  } else {
  }

  #ifdef RAM_USAGE_H
  ram->measure_now(4);
  #endif
}
