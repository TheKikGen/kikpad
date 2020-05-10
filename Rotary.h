/*
 * Rotary encoder library for Arduino.
 */

#ifndef Rotary_h
#define Rotary_h

#include "Arduino.h"

// Enable this to emit codes twice per step.
// #define HALF_STEP

class Rotary
{
  uint8_t _state;
  uint8_t _pin1;
  uint8_t _pin2;

#ifdef _R_HALF_STEP
  static const uint8_t _ttable[6][4];
#else
  static const uint8_t _ttable[7][4];
#endif

public:
  enum encoderStatusValues {
    noDir  = 0x00, // No complete step yet.
    dirCw  = 0x10, // Clockwise step.
    dirCCw = 0x20  // Counter-clockwise step.
  };

  enum encodeStateTableValues {
    start,
#ifdef _R_HALF_STEP
  // Use the half-step state table (emits a code at 00 and 11)
    ccwBegin, cwBegin, startM, cwBeginM, ccwBeginM
#else
  // Use the full-step state table (emits a code at 00 only)
    cwFinal, cwBegin, cwNext, ccwBegin, ccwFinal, ccwNext
#endif
};

  Rotary();
  void begin(uint8_t, uint8_t);
  uint8_t read();

};

#endif
