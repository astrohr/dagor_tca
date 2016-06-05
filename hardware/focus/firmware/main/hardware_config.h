

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
  #define RESET_PIN 50  // MISO

#elif HARDWARE == HARDWARE_PRO_MINI

  #error Not implemented

#elif HARDWARE == HARDWARE_1284

  #error Not implemented

#elif HARDWARE == HARDWARE_NANO

// TODO make use of

#else  // #if HARDWARE == HARDWARE_<something>

  // throw compiler error:
  #ifndef HARDWARE
    #error Define HARDWARE preprocessor var. See hardware_options.h for possible choices.
  #else
    #error Unknown value in HARDWARE. See hardware_options.h for possible choices.
  #endif

#endif  // #if HARDWARE == HARDWARE_<something>

#endif  // #ifndef HARDWARE_CONFIG_H
