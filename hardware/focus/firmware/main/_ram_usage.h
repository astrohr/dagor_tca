/** Ram usage module
 *
 */

// Start every module with at least these two includes:
#include <Arduino.h>
#include "modules.h"

#ifndef RAM_USAGE_H
#define RAM_USAGE_H



// Optionally enable RAM usage measurements:
#ifndef RAM_MEASUREMENTS_N
#define RAM_MEASUREMENTS_N MODULE_MAX_N  // default value for MODULE_MAX_N
#endif

#ifndef RAM_TOTAL
#define RAM_TOTAL 2048
#endif


// Module:
class Ram: public Module
{
public:
  uint16_t step;  // run on every "step-th" loop
  uint16_t step_i;
  int32_t measurements[RAM_MEASUREMENTS_N];
  void measure(uint8_t index);
  void measure_now(uint8_t index);
  void print_mesaurements();
  void setup();
  void loop();
  Ram():
   step(50000),
   step_i(0)
  {};
};


void Ram::measure(uint8_t index) {
  if (step_i % step != 0) return;

  extern int __heap_start, *__brkval;
  int v;
  Ram::measurements[index] = (int32_t) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
};

void Ram::measure_now(uint8_t index) {
  extern int __heap_start, *__brkval;
  int v;
  Ram::measurements[index] = (int32_t) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
};

void Ram::print_mesaurements() {
  extern int __heap_start, *__brkval;
  int v, r;
  uint8_t i=0;
  Serial.print(F("\nRAM: (used + free)\n"));
  for (i=0; i<RAM_MEASUREMENTS_N; i++) {
    Serial.print(i);
    Serial.print(F(" : "));
    if (Ram::measurements[i] > -1) {
      Serial.print(RAM_TOTAL - Ram::measurements[i]);
      Serial.flush();
      Serial.print(F(" + "));
      Serial.flush();
      Serial.print(Ram::measurements[i]);
      Serial.flush();
      Ram::measurements[i] = -1;
    }
    Serial.print(F("\n"));
    Serial.flush();
  }

  r = (int32_t) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  Serial.println(r);
  Serial.flush();

  Serial.print(F("\n"));
  Serial.flush();
};


// Instantiate the module class:
Ram* ram = (Ram*) ModulesRegistry::add(new Ram());


#endif  // #ifndef RAM_USAGE_H
