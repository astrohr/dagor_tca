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

  #ifdef MOTOR_H
  if (deferred_reply_case == PROTOCOL_REPLY_SET_POSITION) {

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
    // make a blank line:
    reply_printer->print(F("\n"));
    // reset flags:
    get.busy = false;
    // also clear reference to the printer, for good measure:
    set.reply = NULL;
    reply_printer = NULL;
    set.command[0] = '\0';
    deferred_reply_case = 0;
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
    Serial.print(F("Got a command but no printer, discarding!\n"));
    set.command[0] = '\0';
    return;
  }
  // store reply_printer locally to avoid accidental overwrite by other modules:
  reply_printer = set.reply;


  // we'll need a fancy String object, for simplr parsing:
  String command = set.command;

  // set busy flag (just in case we have to handle this command over multiple cycles):
  // BTW, that's not used yet
  get.busy = true;
  deferred_reply_case = false;

  // status buffer:
  if (false){

  #ifdef STATUS_H
  } else if (command == "status") {
    // calculate how many lines are going to be in the reply:
    int status_reply_count = 9;
    // first line of the reply, with the number of lines to follow:
    reply_printer->print(F("status "));
    reply_printer->print(status_reply_count);
    reply_printer->print(F("\n"));
    status->print(reply_printer);

  } else if (command.substring(0, 8)  == "step by ") {
    reply_printer->print(F("ok 0\n"));
    String data = command.substring(8);
    char buf[data.length() + 1];
    data.toCharArray(buf, data.length() + 1);
    int value = atoi(buf);
    status->set.step_by = value;

  } else if (command.substring(0, 8)  == "step to ") {
    reply_printer->print(F("ok 0\n"));
    String data = command.substring(8);
    char buf[data.length() + 1];
    data.toCharArray(buf, data.length() + 1);
    int value = atoi(buf);
    status->set.step_by = value - motor->get.position;

  } else if (command == "stop") {
    status->set.hard_stop = true;
    reply_printer->print(F("stopping 0\n"));

  #endif  //  #ifdef STATUS_H

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
    deferred_reply_case = PROTOCOL_REPLY_SET_POSITION;
  #endif  //  #ifdef MOTOR_H

  } else if (command == "" || command == "help") {
    int help_reply_count = 6;
    reply_printer->print(F("help "));
    reply_printer->print(help_reply_count);
    reply_printer->print(F("\n"));
    reply_printer->print(F("available commands:\n"));
    reply_printer->print(F("status\n"));
    reply_printer->print(F("step by <N>\n"));
    reply_printer->print(F("step to <N>\n"));
    reply_printer->print(F("position\n"));
    reply_printer->print(F("position set <N>\n"));

  } else {
    reply_printer->print(F("error 2\n"));
    reply_printer->print(F("unknown command\n"));
    reply_printer->print(command);
    reply_printer->print(F("\n"));
  }

  // clear command:
  set.command[0] = '\0';

  // clear other flags if replied:
  if (!deferred_reply_case) {
    // make a blank line:
    reply_printer->print(F("\n"));
    // reset flags:
    get.busy = false;
    // also clear reference to the printer, for good measure:
    set.reply = NULL;
    reply_printer = NULL;
  }


  #ifdef RAM_USAGE_H
  ram->measure_now(4);
  #endif
}
