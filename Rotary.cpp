/* Rotary encoder handler for arduino.
 *
 * Copyright 2011 Ben Buxton. Licenced under the GNU GPL Version 3.
 * Contact: bb@cactii.net
 *
 */

#include "Arduino.h"
#include "Rotary.h"

/*
 * The below state table has, for each state (row), the new state
 * to set based on the next encoder output. From left to right in,
 * the table, the encoder outputs are 00, 01, 10, 11, and the value
 * in that position is the new state to set.
 */
#ifdef _R_HALF_STEP
// Use the half-step state table (emits a code at 00 and 11)
const uint8_t Rotary::_ttable[6][4] = {
  {startM         , cwBegin  , ccwBegin, start          }, // R_START (00)
  {startM | dirCCw, start    , ccwbegin, start          }, // R_CCW_BEGIN
  {startM |  dirCw, cwBegin  , start   , start          }, // R_CW_BEGIN
  {startM         , ccwBeginM, cwBeginM, start          }, // R_START_M (11)
  {startM         , startM   , cwBeginM, start | dirCw  }, // R_CW_BEGIN_M
  {startM         , ccwBeginM, startM  , start | dirCCw },// R_CCW_BEGIN_M
};
#else
// Use the full-step state table (emits a code at 00 only)
const uint8_t Rotary::_ttable[7][4] = {
  {start  , cwBegin , ccwBegin, start          }, // R_START
  {cwNext , start   , cwFinal , start | dirCw  }, // R_CW_FINAL
  {cwNext , cwBegin , start   , start          }, // cwBegin
  {cwNext , cwBegin , cwFinal , start          }, // cwNext
  {ccwNext, start   , ccwBegin, start          }, // ccwBegin
  {ccwNext, ccwFinal, start   , start | dirCCw }, // R_CCW_FINAL
  {ccwNext, ccwFinal, ccwBegin, start          }, // R_CCW_NEXT
};
#endif

Rotary::Rotary() {
}

void Rotary::begin(uint8_t pin1, uint8_t pin2 ) {
  // Assign variables.
  _pin1 = pin1;
  _pin2 = pin2;

  // Set pins to input.
  pinMode(_pin1, INPUT);
  pinMode(_pin2, INPUT);

  // Initialise state.
  _state = start;
}

uint8_t Rotary::read() {
  // Grab state of input pins.
  uint8_t pinstate = (digitalRead(_pin2) << 1) | digitalRead(_pin1);
  // Determine new state from the pins and state table.
  _state = _ttable[_state & 0xf][pinstate];
  // Return emit bits, ie the generated event.
  return _state & 0x30;
}
