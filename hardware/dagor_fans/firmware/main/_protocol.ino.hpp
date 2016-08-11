#include <Arduino.h>
#include "modules.hpp"

#include "_protocol.hpp"


#ifdef PROTOCOL_INO_HPP
#error The .ino.hpp file of a module can be included only once (and should be included from main.ino). Did you wanted to include the .h file instead?
#endif
#define PROTOCOL_INO_HPP


void Protocol::setup()
{}


void Protocol::loop()
{
  // we are only conserned with handling incomming commands:
  if (setCommand[0] == '\0') {
    // (empty strings start with a null-character)
    return;
  }

  if (!setReplyPrinter) {
    // reply printer must also be set
    // TODO use Serial only in DEBUG mode!
    Serial.print(F("Got a command but no printer!\n"));
    setCommand[0] = '\0';
    return;
  }
  // store replyPrinter locally to avoid accidental overwrite by other modules:
  replyPrinter = setReplyPrinter;
  // clear the public setter:
  setReplyPrinter = nullptr;


  // we'll need a fancy String object, for simple parsing:
  String command = setCommand;

  // set busy flag (just in case we have to handle this command over multiple cycles):
  busy = true;

  // handle actual commands:

  if (command == "status") {
    // calculate how many lines are going to be in the reply:
    int status_reply_count = 0;
    #ifdef FANS_HPP
      status_reply_count += 2;
    #endif
    #ifdef POWER_HPP
      status_reply_count += 1;
    #endif

    // first line of the reply, with the number of lines to follow:
    replyPrinter->print(F("status "));
    replyPrinter->print(status_reply_count);
    replyPrinter->print(F("\n"));

    // status from fans module:
    #ifdef FANS_HPP
      replyPrinter->print(F("fan 1: "));
      replyPrinter->print(fans->fan1);
      replyPrinter->print(F("\n"));
      replyPrinter->print(F("fan 2: "));
      replyPrinter->print(fans->fan2);
      replyPrinter->print(F("\n"));
    #endif  //#ifdef BLINK_HPP

    // status from power module:
    #ifdef POWER_HPP
      replyPrinter->print(F("power: "));
      replyPrinter->print(power->powerState);
      replyPrinter->print(F("\n"));
    #endif  //#ifdef POWER_HPP

  // finished with "status" command


#ifdef FANS_HPP

  // blink module can handle one command:
} else if (command.substring(0, 10) == "fan 1 set ") {
  String data = command.substring(10);
  char buf[data.length() + 1];
  data.toCharArray(buf, data.length() + 1);
  int value = atoi(buf);
  if (value >= 0 && value <= FANS_MAX) {
    fans->setFan1 = value;
    replyPrinter->print(F("ok 1\n"));
    replyPrinter->print(value);
    replyPrinter->print(F("\n"));
  } else {
    replyPrinter->print(F("error 1\n"));
    replyPrinter->print(F("value not in range\n"));
  }

} else if (command.substring(0, 10) == "fan 2 set ") {
  String data = command.substring(10);
  char buf[data.length() + 1];
  data.toCharArray(buf, data.length() + 1);
  int value = atoi(buf);
  if (value >= 0 && value <= FANS_MAX) {
    fans->setFan2 = value;
    replyPrinter->print(F("ok 1\n"));
    replyPrinter->print(value);
    replyPrinter->print(F("\n"));
  } else {
    replyPrinter->print(F("error 1\n"));
    replyPrinter->print(F("value not in range\n"));
  }

  } else if (command == "fan 1 get") {
    replyPrinter->print(F("ok 1\n"));
    replyPrinter->print(fans->fan1);
    replyPrinter->print(F("\n"));

  } else if (command == "fan 2 get") {
    replyPrinter->print(F("ok 1\n"));
    replyPrinter->print(fans->fan2);
    replyPrinter->print(F("\n"));

  } else if (command == "power get") {
    replyPrinter->print(F("ok 1\n"));
    replyPrinter->print(power->powerState);
    replyPrinter->print(F("\n"));

  #endif  // #ifdef FANS_HPP


  #ifdef RESET_HPP

  } else if (command == "reset") {
    reset->trigger = true;
    replyPrinter->print(F("ok 0\n\n"));
    delay(100);

  #endif  // #ifdef RESET_HPP

  } else {
    replyPrinter->print(F("error 2\n"));
    replyPrinter->print(F("unknown command\n"));
    replyPrinter->print(command);
    replyPrinter->print(F("\n"));
  }

  // add an extra new line, just in case:
  replyPrinter->print(F("\n"));

  // reset flags:
  setCommand[0] = '\0';
  busy = false;

  // also clear reference to the printer, for good measure:
  replyPrinter = nullptr;
}
