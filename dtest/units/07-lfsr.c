// obj/utils/lfsr.o -lm
// 0

// TODO: "-lm" ?

#include "utils/lfsr.h"

#include "utils/debug.h"

// const LFSR_TYPE taps = 4322;
const LFSR_TYPE taps = 7172;
const LFSR_TYPE initialState = 12345;
LFSR_TYPE state = initialState;
 
void generate1M() {
  for (unsigned int i = 0; i < 1000000; i++)
    getNextLFSR(&state, taps);
}

int main() {
  DTEST_UNIT_START("LFSR test");
  DTEST_EVAL_TIME(generate1M());
  DTEST_UNIT_END
  return 0;
}
