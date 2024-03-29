#include "utils/lfsr.h"

// Returns n'th bit of number
#define GET_BIT(number, i) !!(number & p2[i])

static const uint8_t SIZE = sizeof(LFSR_TYPE) * 8;
static const LFSR_TYPE LEFT_BIT = (LFSR_TYPE)1 << (SIZE - 1);

static const uint32_t p2[32] = {
    1,         2,         4,          8,         16,       32,       64,
    128,       256,       512,        1024,      2048,     4096,     8192,
    16384,     32768,     65536,      131072,    262144,   524288,   1048576,
    2097152,   4194304,   8388608,    16777216,  33554432, 67108864, 134217728,
    268435456, 536870912, 1073741824, 2147483648};

static uint8_t get_feedback(LFSR_TYPE state, LFSR_TYPE taps) {
  uint8_t result = 0;
  LFSR_TYPE x = state & taps;
  for (uint8_t i = 0; i < SIZE; i++)
    result ^= GET_BIT(x, i);
  return result;
}

LFSR_TYPE lfsr(LFSR_TYPE *state, LFSR_TYPE taps) {
  LFSR_TYPE result = 0;
  uint8_t feedback = 0;
  for (uint8_t i = 0; i < SIZE; i++) {
    result = (LFSR_TYPE)(result << 1) | (*state & 1);
    feedback = get_feedback(*state, taps);
    *state = (LFSR_TYPE)(*state >> 1 | (LEFT_BIT * feedback));
  }
  return result;
}
