#include "utils/lfsr.h"

#include <math.h>

const uint8_t SIZE = sizeof(LFSR_TYPE) * 8;
const LFSR_TYPE LEFT_BIT = (LFSR_TYPE)1 << (SIZE - 1);

uint8_t getBitAtPosition(LFSR_TYPE n, uint8_t p) {
  return !!(n & (LFSR_TYPE)pow(2, p));
}

uint8_t getFeedback(LFSR_TYPE state, LFSR_TYPE taps) {
  uint8_t result = 0;
  LFSR_TYPE x = state & taps;
  for (uint8_t i = 0; i < SIZE; i++)
    result ^= getBitAtPosition(x, i);
  return result;
}

LFSR_TYPE getNextLFSR(LFSR_TYPE *state, LFSR_TYPE taps) {
  LFSR_TYPE result = 0;
  uint8_t feedback = 0;
  for (uint8_t i = 0; i < SIZE; i++) {
    result = (LFSR_TYPE)(result << 1);
    result |= (*state & 1);
    feedback = getFeedback(*state, taps);
    *state >>= 1;
    *state = (LFSR_TYPE)(*state | (LEFT_BIT * feedback));
  }
  return result;
}
