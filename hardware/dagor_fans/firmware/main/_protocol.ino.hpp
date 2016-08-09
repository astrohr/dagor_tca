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


  // we'll need a fancy String object, for simplr parsing:
  String command = setCommand;

  // set busy flag (just in case we have to handle this command over multiple cycles):
  busy = true;

  // handle actual commands:

  if (command == "status") {
    // calculate how many lines are going to be in the reply:
    int status_reply_count = 0;
    #ifdef BLINK_HPP
      status_reply_count += 2;
    #endif
    #ifdef BLINK_MODIFIER_HPP
      status_reply_count += 1;
    #endif
    #ifdef BLINK_COUNTER_HPP
      status_reply_count += 1;
    #endif
    #ifdef BUTTON_HPP
      status_reply_count += 1;
    #endif

    // first line of the reply, with the number of lines to follow:
    replyPrinter->print(F("status "));
    replyPrinter->print(status_reply_count);
    replyPrinter->print(F("\n"));

    // status from blink module:
    #ifdef BLINK_HPP
      replyPrinter->print(F("blink: "));
      replyPrinter->print(blink->ledState ? F("ON") : F("OFF"));
      replyPrinter->print(F("\n"));
      replyPrinter->print(F("interval: "));
      replyPrinter->print(blink->interval);
      replyPrinter->print(F("\n"));
    #endif  //#ifdef BLINK_HPP

    // status from blink_modifier module:
    #ifdef BLINK_MODIFIER_HPP
      replyPrinter->print(F("index: "));
      replyPrinter->print(blink_modifier->index);
      replyPrinter->print(F("\n"));
    #endif  //#ifdef BLINK_MODIFIER_HPP

    // status from blink module:
    #ifdef BLINK_COUNTER_HPP
      replyPrinter->print(F("count: "));
      replyPrinter->print(blink_counter->count);
      replyPrinter->print(F("\n"));
    #endif  //#ifdef BLINK_COUNTER_HPP

    // status from blink module:
    #ifdef BUTTON_HPP
      replyPrinter->print(F("button: "));
      replyPrinter->print( button->pressed ? F("closed") : F("open"));
      replyPrinter->print(F("\n"));
    #endif  //#ifdef BUTTON_HPP

  // finished with "status" command


  #ifdef BLINK_MODIFIER_HPP

  // blink module can handle one command:
  } else if (command == "interval get") {
    replyPrinter->print(F("ok 1\n"));
    replyPrinter->print(blink->interval);
    replyPrinter->print(F("\n"));

  #endif  // #ifdef BLINK_MODIFIER_HPP


  #ifdef BLINK_MODIFIER_HPP
  // blink_modifier can handle three commands:

  } else if (command == "index get") {
    replyPrinter->print(F("ok 1\n"));
    replyPrinter->print(blink_modifier->index);
    replyPrinter->print(F("\n"));

  } else if (command == "index next") {
    blink_modifier->setNextInterval = true;
    replyPrinter->print(F("ok 0\n"));

  } else if (command.substring(0, 10) == "index set ") {
    // parse the rest of command variable for int value:
    String data = command.substring(10);
    char buf[data.length() + 1];
    data.toCharArray(buf, data.length() + 1);
    int value = atoi(buf);
    // make sure the value is valid:
    if (value < 0 || value >= BLINK_MODIFIER_INTERVALS_N) {
      replyPrinter->print("error 3\n");
      replyPrinter->print("value out of range\n");
      replyPrinter->print("got value: ");
      replyPrinter->print(value);
      replyPrinter->print("\n");
      replyPrinter->print("range: 0 to ");
      replyPrinter->print(BLINK_MODIFIER_INTERVALS_N - 1);
      replyPrinter->print("\n");
    } else {
      // the value is valid, use it:
      blink_modifier->setIntervalTo = value;
      replyPrinter->print("ok 0\n");
    }

  #endif  // #ifdef BLINK_MODIFIER_HPP


  #ifdef BLINK_COUNTER_HPP
  // blink_counter can handle two commands:

  } else if (command == "count get") {
    replyPrinter->print(F("ok 1\n"));
    replyPrinter->print(blink_counter->count);
    replyPrinter->print(F("\n"));

  } else if (command == "count reset") {
      replyPrinter->print(F("ok 0\n"));
      blink_counter->setReset = true;

  #endif  // #ifdef BLINK_COUNTER_HPP


  // unknown command:

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
