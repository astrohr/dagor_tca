#include <Arduino.h>
#include "modules.h"

#include "_protocol.h"


#ifdef PROTOCOL_INO_H
#error The .ino.h file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define PROTOCOL_INO_H


#include "_status.h"


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
    int status_reply_count = 8;
    // first line of the reply, with the number of lines to follow:
    reply_printer->print(F("status "));
    reply_printer->print(status_reply_count);
    reply_printer->print(F("\n"));
    status->print(&Serial);  // TODO this is ok, but it's better to trigger prints via status->set.something

  } else if (command.substring(0, 7)  == "step_by") {
    String data = command.substring(7);
    char buf[data.length() + 1];
    data.toCharArray(buf, data.length() + 1);
    int value = (int) atof(buf);  // TODO use atoi ?
    status->set.step_by = value;

  } else if (command.substring(0, 7)  == "stop") {
    status->set.hard_stop = true;

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
