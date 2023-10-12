#include "utils/lfsr.h"

const uint8_t SIZE = sizeof(LFSR_TYPE) * 8;
const LFSR_TYPE LEFT_BIT = (LFSR_TYPE)1 << (SIZE - 1);

const unsigned int p2[32] = {
    1,         2,         4,          8,         16,       32,       64,
    128,       256,       512,        1024,      2048,     4096,     8192,
    16384,     32768,     65536,      131072,    262144,   524288,   1048576,
    2097152,   4194304,   8388608,    16777216,  33554432, 67108864, 134217728,
    268435456, 536870912, 1073741824, 2147483648};

uint8_t getBitAtPosition(LFSR_TYPE n, uint8_t p) { return !!(n & p2[p]); }

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
