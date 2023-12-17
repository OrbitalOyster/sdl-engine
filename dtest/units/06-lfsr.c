// obj/utils/lfsr.o
// 0

#include "utils/lfsr.h"

#include "utils/debug.h"

const LFSR_TYPE taps = 61680; // 1111000011110000
const LFSR_TYPE initialState = 12345;
LFSR_TYPE state = initialState;

void generate1M() {
  unsigned int i = 0;
  for (; i < 1000000; i++) {
    getNextLFSR(&state, taps);
    if (state == initialState) break;
  }
  INFO2F("Generated: %u", i);
}

void generateAll() {
  unsigned int n = 0;
  do {
    getNextLFSR(&state, taps);
    n++;
  } while (state != initialState);
  INFO2F("Generated: %u", n);
}

int main() {
  DTEST_UNIT_START("LFSR");
  INFO2F("%u", getNextLFSR(&state, taps));
  INFO2F("%u", getNextLFSR(&state, taps));
  INFO2F("%u", getNextLFSR(&state, taps));
  INFO2F("%u", getNextLFSR(&state, taps));
  INFO2F("%u", getNextLFSR(&state, taps));
  DTEST_UNIT_END;
}
