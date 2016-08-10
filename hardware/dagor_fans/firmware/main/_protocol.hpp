#include <Arduino.h>
#include "modules.hpp"


#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#define PROTOCOL_COMMAND_LEN 40  // max command string length





class Protocol : public Module
{
private:
  Print * replyPrinter = nullptr;
public:
  bool busy = false;
  char setCommand[PROTOCOL_COMMAND_LEN] = "";
  Print * setReplyPrinter = nullptr;
  void setup();
  void loop();
};


Protocol* protocol = (Protocol*) ModulesRegistry::add(new Protocol());


#endif  // #ifndef PROTOCOL_HPP
