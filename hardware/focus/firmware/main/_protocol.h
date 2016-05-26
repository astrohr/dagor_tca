#include <Arduino.h>
#include "modules.h"


#ifndef PROTOCOL_H
#define PROTOCOL_H

#define PROTOCOL_COMMAND_LEN 40
#define PROTOCOL_ORIGIN_NONE 0
#define PROTOCOL_ORIGIN_SERIAL 1
#define PROTOCOL_ORIGIN_RADIO 2


typedef struct {
  bool busy;
} ProtocolGet;


typedef struct {
    char command[PROTOCOL_COMMAND_LEN];
    Print * reply;
} ProtocolSet;


class Protocol : public Module
{
private:
  Print * reply_printer;
public:
  ProtocolGet get;
  ProtocolSet set;
  void setup();
  void loop();
//initializers:
  Protocol():
    get{
      false,
    },
    set{
      "",
      NULL,
    }
  {
  };
};


Protocol* protocol = (Protocol*) ModulesRegistry::add(new Protocol());


#endif  // #ifndef PROTOCOL_H
