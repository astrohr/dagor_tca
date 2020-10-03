// main.ino
// -*- mode: C++ -*-


#include "hardware_options.h"
#define HARDWARE HARDWARE_NANO
#include "hardware_config.h"

#define MODULE_MAX_N 10
#include "modules.h"
//#include "_ram_usage.ino.h"

//#include "_blink.ino.h"
#include "_reset.ino.h"
#include "_buttons.ino.h"
#include "_motor.ino.h"
#include "_eeprom.ino.h"
#include "_status.ino.h"
#include "_protocol.ino.h"
#include "_serial_comm.ino.h"


void setup()
{
  // Call setup() for every module:
  unsigned int i;
  for (i = 0; i < ModulesRegistry::count; i++) {
    ModulesRegistry::modules[i]->setup();
  }
  delay(3000);
  Serial.print("\nready\n");
}


void loop()
{
  // Call loop() for every module:
  unsigned int i;
  for (i = 0; i < ModulesRegistry::count; i++) {
    ModulesRegistry::modules[i]->loop();
  }
}
