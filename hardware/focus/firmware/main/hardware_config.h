

/*
 * Available choices for HARDWARE preprocessor variable:
 *
 * "micro"
 * - Arduino ProMini
 *
 * "mega"
 * - Arduino Mega
 */

#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H


// common:

#define SERIAL_BAUD_RATE 115200


// hardware-specific:

#if HARDWARE == HARDWARE_MEGA

  #error Not implemented

#elif HARDWARE == HARDWARE_PRO_MINI

  #error Not implemented

#elif HARDWARE == HARDWARE_1284

  #error Not implemented

#elif HARDWARE == HARDWARE_NANO

  // temp sensor
  #define SENSOR_TEMP_PIN_PRESENCE 15
  #define SENSOR_TEMP_PIN_DATA A0  // TODO WHAT this is supposed to be "A7"!?
  // rain sensor
  #define SENSOR_RAIN_PIN_PRESENCE 23
  #define SENSOR_RAIN_PIN_DATA 22

#else  // #if HARDWARE == HARDWARE_<something>

  // throw compiler error:
  #ifndef HARDWARE
    #error Define HARDWARE preprocessor var. See hardware_options.h for possible choices.
  #else
    #error Unknown value in HARDWARE. See hardware_options.h for possible choices.
  #endif

#endif  // #if HARDWARE == HARDWARE_<something>

#endif  // #ifndef HARDWARE_CONFIG_H
